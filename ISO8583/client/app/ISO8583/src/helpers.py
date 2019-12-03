from datetime import datetime as dt
import random


class DateTime:
    iso = "%m%d%H%M%S"
    formatted = "%m/%d %H:%M:%S"

    @classmethod
    def datetime(cls):
        return str(dt.strftime(dt.now(), cls.iso))

    @classmethod
    def from_datetime(cls, t):
        stamp = dt.strptime(t, cls.iso)
        return dt.strftime(stamp, cls.formated)

    @classmethod
    def is_later(cls, t2, t1):
        return (
            dt.strptime(t2, cls.iso)
            - dt.strptime(t1, cls.iso)
        ).total_seconds() > 0


class UUID:
    @classmethod
    def uuid(cls, storage: list):
        uuid = hex(random.getrandbits(40))[2:]

        if uuid in storage:
            return cls.uuid()
        else:
            storage.append(uuid)
            return uuid
