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
    iso = "%m%d%H%M%S"
    exp_date = "%y%m"
    formatted = "%m/%d %H:%M:%S"

    @classmethod
    def datetime(cls):
        return dt.strftime(dt.now(), cls.iso)

    @classmethod
    def from_datetime(cls, t):
        stamp = dt.strptime(t, cls.iso)
        return dt.strftime(stamp, cls.formated)

    @classmethod
    def is_card_expired(cls, t):
        return dt.strftime(t, cls.exp_date) < dt.now()

    @classmethod
    def is_timeout(cls, t1, t2):
        return (dt.strptime(t2, cls.iso) - dt.strptime(t1, cls.iso)).total_seconds() > 5
