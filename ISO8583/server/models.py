import random
from datetime import datetime as dt


class UUID:
    @classmethod
    def uuid(cls, storage: list):
        uuid = hex(random.getrandbits(128))
        if uuid in storage:
            return cls.uuid()
        else:
            storage.append(uuid)
            return uuid


class DateTime:
    iso = "%d %B %Y - %I:%M:%S%p"

    @classmethod
    def datetime(cls):
        return dt.strftime(dt.now(), cls.iso)

    @classmethod
    def is_later(cls, t2, t1):
        return (
            dt.strptime(t2, cls.iso)
            - dt.strptime(t1, cls.iso)
        ).total_seconds() > 0


class Good:
    storage = []

    def __init__(self, _price: float, _name: str):
        self.uuid = UUID.uuid(self.storage)
        self.price = _price
        self.name = _name

    def get_uuid(self):
        return self.uuid

    def to_dict(self):
        return {
            'uuid': self.uuid,
            'price': self.price,
            'name': self.name,
        }


class Session:
    storage = []

    def __init__(self, _terminal_uuid):
        self.uuid = UUID.uuid(self.storage)
        self.sell_counter = 0
        self.sell_total = 0
        self.refund_count = 0
        self.refund_total = 0
        self.terminal_uuid = _terminal_uuid
        self.datetime = DateTime.datetime()

    def get_uuid(self):
        return self.uuid

    def get_terminal_uuid(self):
        return self.terminal_uuid

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

    def to_dict(self):
        return {
            'uuid': self.uuid,
            'session_uuid': self.session_uuid,
            'type': self.type_,
            'bitmap': self.bitmap,
            'card': self.card.to_dict(),
            'number': self.number,
            'rrn': self.rrn,
            'text': self.text,
            'terminal_uuid': self.terminal_uuid,
            'rc': self.rc,
            'datetime': self.datetime,
            'crc': self.crc
        }


class Card:
    storage = []

    def __init__(self, _pan: int, _exp_date: str, _cvv: int, _pin: int,
                 _holder_fcs: str, _balance: float, _type: str):
        self.uuid = UUID.uuid(self.storage)
        self.pan = _pan
        self.exp_date = _exp_date
        self.cvv = _cvv
        self.pin = _pin
        self.holder_fcs = _holder_fcs
        self.balance = _balance
        self.type_ = _type

    def valid(self):
        return DateTime.is_later(
            self.exp_date,
            DateTime.datetime()
        )

    def to_dict(self):
        return {
            'uuid': self.uuid,
            'pan': self.pan,
            'exp_date': self.exp_date,
            'cvc': self.cvc,
            'pin': self.pin,
            'holder_fcs': self.holder_fcs,
            'balance': self.balance,
            'type': self.type_
        }
