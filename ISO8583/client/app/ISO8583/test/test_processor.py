import unittest
from unittest.mock import MagicMock, Mock

from src.processor import parse
from src.transaction import Transaction, action_codes, to_log


class TestProcessor(unittest.TestCase):
    # --------- TRANSACTION PIECES ---------
    ACCEPTED_AND_IMPLEMENTED = '00'
    TRANSACTION_NOT_FOUND = '12'
    INVALID_AMOUNT = '13'
    INVALID_PAN = '14'
    CARD_EXPIRED = '33'
    PIN_TRY_EXCEED = '38'
    CARD_LOST = '41'
    CARD_STOLEN = '43'
    INSUFFICIENT_FUNDS = '51'
    INVALID_PIN = '55'
    TIMEOUT = '96'
    UNREGISTERED_TERMINAL_ID = '98'
    RESUME_REVISE_ERROR = '99'

    financial_common_transaction_data = {
        'PAN': '5420592894149290',
        'cardholder_name': 'PRODIGYMOBBDEEP',
        'expiry_date': '1122',
        'PIN': '2368',
        'amount': '000000123456',
    }

    common_transaction_data = {
        'transaction_no': '0000000123',
        'RRN': '000000000003',
        'terminal_id': '7aedf2340a',
        'datetime': '1206081106',
        'CRC': '3856731e'
    }
    # --------------------------------------
    test_transaction = {
        'bitmap': '6016',
        'transaction_id': '0020'
    }

    balance_transaction = {
        'bitmap': '7e16',
        'transaction_id': '0220'
    }

    sale_transaction = {
        'bitmap': '7f96',
        'transaction_id': '0120'
    }

    refund_transaction = {
        'bitmap': '7f96',
        'transaction_id': '0130'
    }

    sale_reversal_transaction = {
        'bitmap': '7f96',
        'transaction_id': '0121'
    }

    refund_reversal_transaction = {
        'bitmap': '7f96',
        'transaction_id': '0141'
    }

    sale_upload_transaction = {
        'bitmap': '7f96',
        'transaction_id': '0520'
    }

    refund_upload_transaction = {
        'bitmap': '7f96',
        'transaction_id': '0540'
    }

    settlement_transaction = {
        'bitmap': '6036',
        'transaction_id': '0440',
        'text_data': '01200003242760010000005672'
    }

    def make(self, transaction, RC, financial=False):
        copy = transaction.copy()
        copy.update(self.common_transaction_data)
        copy.update({'RC': RC})
        if financial:
            copy.update(self.financial_common_transaction_data)
        return ''.join([
            copy.get('bitmap', ''),
            copy.get('transaction_id', ''),
            copy.get('PAN', ''),
            copy.get('cardholder_name', ''),
            copy.get('expiry_date', ''),
            copy.get('PIN', ''),
            copy.get('amount', ''),
            copy.get('transaction_no', ''),
            copy.get('RRN', ''),
            copy.get('text_data', ''),
            copy.get('terminal_id', ''),
            copy.get('RC', ''),
            copy.get('datetime', ''),
            copy.get('CRC', '')
        ]), copy

    def test_parse(self):
        str_tr, tr = self.make(
            self.test_transaction,
            self.ACCEPTED_AND_IMPLEMENTED
        )
        print(str_tr)
        print(tr)
        self.assertEqual(
            parse(str_tr),
            tr
        )

