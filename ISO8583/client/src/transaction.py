"""
Transaction Fields according to Protocol v1:
+-------------+------------------+--------------------+----------+
| Start Sign  | Protocol Version | Transaction Values | End Sign |
| 00 02       | 01               | ...                | 00 03    |
+-------------+------------------+--------------------+----------+
"""


class Transaction:
    template = b"%(ss)s%(pv)s%(tv)s%(es)s"

    @classmethod
    def transaction(cls,
                    data,
                    start_sign=b'0002',
                    protocol_version=b'01',
                    end_sign=b'0003'):
        return bytes(template % (
            start_sign,
            protocol_version,
            data,
            end_sign
        ), 'utf-8')
