from .bitmap import Bitmap
from .transaction import Transaction
from . import version


CARDHOLDER_NAME_LENGTH = 0


def process(action, rs_transaction):
    if action in ['sale', 'refund', 'balance', 'upload']:
        _handle_cardholder_name(rs_transaction)

    RC = rs_transaction[-24:-22]
    print('Response Code is', RC)

    if action not in ['sale', 'refund', 'settlement']:
        return ''

    try:
        next_action = {
            'sale96': 'sale_reversal',
            'sale99': 'sale_upload',
            'refund96': 'refund_reversal',
            'refund99': 'refund_upload',
            'settlement99': 'upload',
        }[action + RC]
        print("Further action for", action, "with RC:", RC, ":", next_action)
    except KeyError:
        print("No further action for", action, "with RC:", RC)
        next_action = ''

    return next_action if next_action else ''


def copy_transaction_data(transaction_data):
    return Transaction.transaction(
        PAN=transaction_data.get('PAN', ''),
        cardholder_name=transaction_data.get('cardholder_name', ''),
        expiry_date=transaction_data.get('expiry_date', ''),
        PIN=transaction_data.get('PIN', ''),
        amount=transaction_data.get('amount', ''),
        transaction_no=transaction_data.get('transaction_no', '')
    )


def _handle_cardholder_name(transaction_str):
    cardholder_name_length = int(transaction_str[30:32])
    version.__lengths__.update({
        version.CARDHOLDER_NAME_INDEX: cardholder_name_length
    })
