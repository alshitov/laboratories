import journal
import request
import helpers
from transaction import Transaction


class Terminal:
    # Cls var
    storage = []
    host = 'http://127.0.0.1:5000/'
    protocol = {
        'start_sign': '0002',
        'protocol_version': '01',
        'end_sign': '0003'
    }

    # Session vars
    session_transactions = []
    active_card = {
        'uuid': '',
        'pan': -1,
        'exp_date': '',
        'cvv': -1,
        'pin': -1,
        'holder_fcs': '',
        'balance': -1,
        'type': ''
    }

    def __init__(self):
        self.uuid = helpers.UUID.uuid(self.storage)
        self.sales_count = 0
        self.sales_amount = 0
        self.refunds_count = 0
        self.refunds_amount = 0

    def log(self, data):
        journal.ClientLogger.log(data)

    def get_log(self):
        journal.ClientLogger.get(self.uuid)

    def read_card(self, card_data):
        self.active_card = card_data

    def decode_data(self, data):
        return bytes(data, 'utf-8')

    def prepare_transaction(self, transaction_str):
        return Transaction.transaction(
            transaction_str,
            self.protocol['start_sign'],
            self.protocol['protocol_version'],
            self.protocol['end_sign'],
        )

    def make_transaction(self, method, data):
        if method == 'GET':
            journal.ClientLogger.get(self.host)
        elif method == 'POST':
            journal.ClientLogger.post(self.host, data)
        elif method == 'PATCH':
            journal.ClientLogger.patch(self.host, data)

    def null_sales(self):
        self.sales_count = 0
        self.sales_amount = 0
        self.make_transaction('POST',
            {'data': 'asdasdasd'}
        )

    def null_refunds(self):
        self.refunds_count = 0
        self.refunds_amount = 0
        transaction_str = '000000123456'
        host_response = self.make_transaction('POST',
            {'data': self.prepare_transaction(transaction_str)}
        )
        self.log({
            'uuid': self.uuid,
            'transaction': transaction_str,
            'response': host_response
        })

    def null(self):
        self.null_sales()
        self.null_refunds()

    def increment_sales(self, amount):
        self.log()
        self.sales_count += 1
        self.sales_amount += amount

    def increment_refunds(self):
        pass

    def cheque(self):
        pass

    def server_timeout(self):
        pass

    def _reversal(self):
        pass

    def _settlement(self):
        pass

    def _upload(self):
        pass
