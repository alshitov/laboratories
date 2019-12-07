from .bitmap import Bitmap
from .transaction import Transaction
from . import version


CARDHOLDER_NAME_LENGTH = 0
TEXT_DATA_LENGTH = 0


def parse(transaction):
    transaction_str = transaction[6:-4]
    bitmap = Bitmap.unhexlify(transaction_str[:version.BITMAP_LENGTH])
    fields = _get_transaction_fields(bitmap)
    return _get_RC(fields, transaction_str)


def process(action, rs_transaction):
    print('RC', parse(rs_transaction))
    if action in ['sale', 'refund', 'settlement']:
        return {
            'sale96': 'sale_reversal',
            'sale99': 'sale_upload',
            'refund96': 'refund_reversal',
            'refund99': 'refund_upload',
            'settlement99': 'upload',
        }[action + parse(rs_transaction)]
    else:
        return ''


def _get_transaction_fields(bitmap):
    bits = [int(b) for b in bitmap]
    transaction_fields = [index for index, bit in enumerate(bits[3:11]) if bit]
    transaction_fields.extend([2, 11, 12, 13, 14])
    return transaction_fields


def copy_transaction_data(transaction_data):
    return Transaction.transaction(
        PAN=transaction_data.get('PAN', ''),
        cardholder_name=transaction_data.get('cardholder_name', ''),
        expiry_date=transaction_data.get('expiry_date', ''),
        PIN=transaction_data.get('PIN', ''),
        amount=transaction_data.get('amount', ''),
        transaction_no=transaction_data.get('transaction_no', '')
    )


def _to_data_offset(fields_present, field_index):
    # Offset for bitmap
    common_offset = 4
    for field_no in fields_present:
        if field_no < field_index \
            and field_no in list(version.__lengths__.keys()):
            common_offset += version.__lengths__[field_no]
            print(field_no, '->', version.__lengths__[field_no], '=>', common_offset )

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


def _get_RC(fields, transaction_str):
    print(version.RC_INDEX)
    offset = _to_data_offset(fields, version.RC_INDEX)
    RC = transaction_str[offset: offset + version.RC_LENGTH]
    return RC
