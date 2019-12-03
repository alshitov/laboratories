import urllib.parse
import json

from .journal import ClientLogger
from .request import Request
from .transaction import Transaction, action_codes, to_log
from .bitmap import Bitmap
from .helpers import UUID


class Terminal:
    storage = []
    host = 'http://127.0.0.1:5000'

    def __init__(self):
        self.id = UUID.uuid(self.storage)
        self.transaction_no = 0
        self.sales_count = 0
        self.sales_amount = 0
        self.refunds_count = 0
        self.refunds_amount = 0
        # Create session
        self.make_transaction('test')
        # Create log file
        ClientLogger.create(self.id)

    def get_uuid(self):
        return self.uuid

    def log(self, data):
        ClientLogger.log(data)

    def get_log(self):
        ClientLogger.get(self.uuid)

    def query(self, data):
        return self.host + '/transaction?data=' + urllib.parse.quote(data)

    def make_transaction(self, action, data=None, from_table=False):
        self.transaction_no += 1

        bitmap = Bitmap.from_action(action)
        transaction_id = action_codes[action]

        if data is None:
            transaction_data = Transaction.transaction(
                bitmap=bitmap,
                transaction_id=transaction_id,
                terminal_id=self.id
            )
        else:
            if not from_table:
                transaction_data = Transaction.transaction(
                    bitmap=bitmap,
                    transaction_id=transaction_id,
                    terminal_id=self.id,
                    PAN=data.get('PAN', ''),
                    cardholder_name=data.get('cardholder_name', ''),
                    expiry_date=data.get('expiry_date', ''),
                    PIN=data.get('PIN', ''),
                    amount=data.get('amount', ''),
                    transaction_no=data.get('transaction_no', ''),
                    RRN=data.get('RRN', ''),
                    text_data=data.get('text_data', ''),
                )
            else:
                transaction_data = data

        request = Request.get(
            self.id,
            self.query(transaction_data)
        )
        response = request.text[:-1]
        data = json.loads(response)

        print(data)

        if transaction_id in to_log:
            self.log({
                'terminal_uuid': self.get_uuid(),
                'transaction': transaction_data,
                'response': response
            })
