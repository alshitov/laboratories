from helpers import UUID, DateTime


class Session:
    storage = []

    def __init__(self, _terminal_id):
        self.uuid = UUID.uuid(self.storage)
        self.sell_counter = 0
        self.sell_total = 0
        self.refund_count = 0
        self.refund_total = 0
        self.terminal_id = _terminal_id
        self.datetime = DateTime.datetime()

    def get_uuid(self):
        return self.uuid

    def get_terminal_id(self):
        return self.terminal_id

    def add_sell(self, amount):
        self.sell_counter += 1
        self.sell_total += amount

    def add_refund(self, amount):
        self.refund_counter += 1
        self.refund_total -= amount

    def to_dict(self):
        return {
            'sell_counter': self.sell_counter,
            'sell_total': self.sell_total,
            'refund_count': self.refund_count,
            'refund_total': self.refund_total,
            'terminal_uuid': self.terminal_uuid,
            'datetime': self.datetime
        }


class Transaction:
    storage = []

    def __init__(self, _type: str, _bitmap: str, _pan: str, _card,
                 _number: int, _rrn: int, _text: str, _terminal_uuid: str,
                 _rc: int, _crc: str, _session_uuid: str):
        self.uuid = UUID.uuid(self.storage)
        self.session_uuid = _session_uuid
        self.type_ = _type
        self.bitmap = _bitmap
        self.card = _card
        self.number = _number
        self.rrn = _rrn
        self.text = _text
        self.terminal_uuid = _terminal_uuid
        self.rc = _rc
        self.datetime = DateTime.datetime()
        self.crc = _crc

    def get_uuid(self):
        return self.uuid


class Card:
    storage = []

    def __init__(self, _PAN: int, _exp_date: str, _PIN: int,
                 _cardholder_name: str, _balance: float):
        self.uuid = UUID.uuid(self.storage)
        self.PAN = _PAN
        self.exp_date = _exp_date
        self.PIN = _PIN
        self.cardholder_name = _cardholder_name
        self.balance = _balance
