from .bitmap import Bitmap
from .transaction import Transaction
from . import version


CARDHOLDER_NAME_LENGTH = 0


def parse(action, transaction):
    transaction_str = transaction[6:-4]
    bitmap = Bitmap.unhexlify(transaction_str[:version.BITMAP_LENGTH])
    fields = _get_transaction_fields(bitmap)
    return _get_RC(fields, transaction_str)


def process(action, rs_transaction):
    if action in ['sale', 'refund', 'balance', 'upload']:
        _handle_cardholder_name(rs_transaction)

    if action in ['sale', 'refund', 'settlement']:
        return {
            'sale96': 'sale_reversal',
            'sale99': 'sale_upload',
            'refund96': 'refund_reversal',
            'refund99': 'refund_upload',
            'settlement99': 'upload',
        }[action + parse(action, rs_transaction)]
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
    offset = 4
    for field_no in fields_present:
        if field_no < field_index \
            and field_no in list(version.__lengths__.keys()):
            offset += version.__lengths__[field_no]
    return offset


def _handle_cardholder_name(transaction_str):
    cardholder_name_length = int(transaction_str[30:32])
    version.__lengths__.update({
        version.CARDHOLDER_NAME_INDEX: cardholder_name_length
    })


def _get_RC(fields, transaction_str):
    _handle_cardholder_name(fields, transaction_str)
    offset = _to_data_offset(fields, version.RC_INDEX)
    RC = transaction_str[offset: offset + version.RC_LENGTH]
    return RC
