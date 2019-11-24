"""
Transaction Fields:
+-------------+------------------+--------------------+----------+
| Start Sign  | Protocol Version | Transaction Values | End Sign |
| 00 02       | 01               | ...                | 00 03    |
+-------------+------------------+--------------------+----------+
"""


class Transaction:
    def __init__(self):
        pass

    @classmethod
    def transaction(cls,
                    data,
                    start_sign='0002',
                    protocol_version='01',
                    end_sign='0003'):
        return
        pass
