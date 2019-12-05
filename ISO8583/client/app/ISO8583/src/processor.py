from .bitmap import Bitmap
from .transaction import Transaction, action_codes, to_log
from .helpers import DateTime
from . import version


CARDHOLDER_NAME_LENGTH = 0
TEXT_DATA_LENGTH = 0


# ------- MAIN -------
def parse(transaction):
    bitmap = Bitmap.unhexlify(transaction[:version.BITMAP_LENGTH])
    fields = _get_transaction_fields(bitmap)
    transaction_data = {
        'transaction_id': _get_transaction_id(fields, transaction),
        'terminal_id': _get_terminal_id(fields, transaction),
        'datetime': _get_datetime(fields, transaction),
        'CRC': _get_CRC(fields, transaction)
    }

    if 3 in fields:
        transaction_data['PAN'] = _get_PAN(fields, transaction)
    if 4 in fields:
        transaction_data['cardholder_name'] = _get_cardholder_name(fields, transaction)
    if 5 in fields:
        transaction_data['expiry_date'] = _get_expiry_date(fields, transaction)
    if 7 in fields:
        transaction_data['amount'] = _get_amount(fields, transaction)
    if 8 in fields:
        transaction_data['transaction_no'] = _get_transaction_no(fields, transaction)
    if 9 in fields:
        transaction_data['RRN'] = _get_RRN(fields, transaction)
    if 10 in fields:
        transaction_data['text_data'] = _get_text_data(fields, transaction)
    if 12 in fields:
        transaction_data['RC'] = _get_RC(fields, transaction)

    return transaction_data


def dispatch(transaction_data):
    mapping = {
        '0020': _receive_test_rs,
        '0220': _receive_balance_rs,
        '0440': _receive_settlement_rs,
        '0120': _receive_sale_rs
        # '0140': _process_refund_rs,
        # '0121': _process_sale_reversal_rs,
        # '0141': _process_refund_reversal_rs,
        # '0520': _process_sale_upload_rs,
        # '0540': _process_refund_upload_rs,
    }
    return mapping[transaction_data['transaction_id']](transaction_data)


def process(rs_transaction):
    transaction_data = parse(rs_transaction[6:-4])
    print('Parsed transaction: ', transaction_data)
    needs_answer = False
    return needs_answer, transaction_data


def respond(transaction_data):
    return dispatch(transaction_data)


def _copy_transaction(transaction_data):
    return Transaction.transaction(
        bitmap=transaction_data['bitmap'],
        transaction_id=transaction_data['transaction_id'],
        terminal_id=transaction_data['terminal_id'],
        PAN=transaction_data.get('PAN', ''),
        cardholder_name=transaction_data.get('cardholder_name', ''),
        expiry_date=transaction_data.get('expiry_date', ''),
        PIN=transaction_data.get('PIN', ''),
        amount=transaction_data.get('amount', ''),
        transaction_no=transaction_data.get('transaction_no', ''),
        text_data=transaction_data.get('text_data', ''),
        RC=transaction_data.get('RC', '')
    )
# ------- MAIN END -------


# ------- DATA GETTERS -------
def _get_transaction_fields(bitmap):
    """ Get fields presence from bitmap
    Note:
        -> 0 bit is never used
        -> 1, 2, 9, 11, 12, 13 and 14 bits are always true
        -> 6 bit is never present
    """
    bits = [int(b) for b in bitmap]
    transaction_fields = [index for index, bit in enumerate(bits[3:11]) if bit]
    transaction_fields.extend([2, 9, 11, 12, 13, 14])
    return transaction_fields


def _to_data_offset(fields_present, field_index):
    """ According fields_present, calculate the
    offset to the field with given index """

    common_offset = 4
    for field_no in fields_present:
        if field_no < field_index \
            and field_no in list(version.__lengths__.keys()):
            common_offset += version.__lengths__[field_no]

    if field_index <= 5:
        # Do not take CARDHOLDER_NAME_LENGTH into account
        return common_offset

    if 5 < field_index <= 10:
        # Do take CARDHOLDER_NAME_LENGTH into account
        return common_offset + CARDHOLDER_NAME_LENGTH

    else:
        # Do take both CARDHOLDER_NAME_LENGTH
        # & TEXT_DATA_LENGTH into account
        return common_offset + CARDHOLDER_NAME_LENGTH + TEXT_DATA_LENGTH


def _get_transaction_id(fields, transaction_str):
    offset = _to_data_offset(fields, version.TRANSACTION_ID_INDEX)
    transaction_id = transaction_str[offset: offset + version.TRANSACTION_ID_LENGTH]
    return transaction_id


def _get_terminal_id(fields, transaction_str):
    offset = _to_data_offset(fields, version.TERMINAL_ID_INDEX)
    terminal_id = transaction_str[offset: offset + version.TERMINAL_ID_LENGTH]
    return terminal_id


def _get_datetime(fields, transaction_str):
    offset = _to_data_offset(fields, version.DATETIME_INDEX)
    datetime = transaction_str[offset: offset + version.DATETIME_LENGTH]
    return datetime


def _get_CRC(fields, transaction_str):
    offset = _to_data_offset(fields, version.CRC_INDEX)
    CRC = transaction_str[offset: offset + version.CRC_LENGTH]
    return CRC


def _get_PAN(fields, transaction_str):
    offset = _to_data_offset(fields, version.PAN_INDEX)
    PAN = transaction_str[offset: offset + version.PAN_LENGTH]
    return PAN


def _get_cardholder_name(fields, transaction_str):
    global CARDHOLDER_NAME_LENGTH
    # 2 digits number in hex starting at 24 position
    CARDHOLDER_FCS_LENGTH = int(transaction_str[24:26], base=16)

    length = CARDHOLDER_FCS_LENGTH * 2
    offset = _to_data_offset(fields, version.CARDHOLDER_NAME_INDEX)
    cardholder_name = transaction_str[offset: offset + length]
    return cardholder_name


def _get_expiry_date(fields, transaction_str):
    offset = _to_data_offset(fields, version.EXPIRY_DATE_INDEX)
    expiry_date = transaction_str[offset: offset + version.EXPIRY_DATE_LENGTH]
    return expiry_date


def _get_PIN(fields, transaction_str):
    offset = _to_data_offset(fields, version.PIN_INDEX)
    PIN = transaction_str[offset: offset + version.PIN_LENGTH]
    return PIN


def _get_amount(fields, transaction_str):
    offset = _to_data_offset(fields, version.AMOUNT_INDEX)
    amount = transaction_str[offset: offset + version.AMOUNT_LENGTH]
    return amount


def _get_transaction_no(fields, transaction_str):
    offset = _to_data_offset(fields, version.TRANSACTION_NO_INDEX)
    amount = transaction_str[offset: offset + version.TRANSACTION_NO_LENGTH]
    return amount


def _get_RRN(fields, transaction_str):
    offset = _to_data_offset(fields, version.RRN_INDEX)
    RRN = transaction_str[offset: offset + version.RRN_LENGTH]
    return RRN


def _get_text_data(fields, transaction_str):
    text_data_length_offest = _to_data_offset(fields, version.TEXT_DATA_INDEX)
    global TEXT_DATA_LENGTH
    # 3 digits number in hex
    TEXT_DATA_LENGTH = \
        transaction_str[text_data_length_offest: text_data_length_offest + 3]

    offset = text_data_length_offest + 3
    length = TEXT_DATA_LENGTH * 2
    text_data = transaction_str[offset: offset + length]
    return text_data

def _get_RC(fields, transaction_str):
    offset = _to_data_offset(fields, version.RC_INDEX)
    RC = transaction_str[offset: offset + version.RC_LENGTH]
    return RC
# ------- DATA GETTERS END -------

# ------- PROCESSORS -------
def _receive_test_rs(transaction_data):
    pass


def _receive_balance_rs(transaction_data):
    pass


def _receive_settlement_rs(transaction_data):
    pass


def _receive_sale_rs(transaction_data):
    terminal_id = transaction_data['terminal_id']

    transaction_data['bitmap'] = Bitmap.sale_rs()
    transaction_data['transaction_id'] = action_codes['sale']
# ------- PROCESSORS END -------
