"""
Transaction fields:
-----------------------------------------------------------------------------------
                             TEST    SALE     REFUND   BALANCE   UPLOAD   SETTLEMENT
#   NAME            TYPE     RQ  RS  RQ  RS   RQ  RS   RQ  RS    RQ  RS   RQ RS
-----------------------------------------------------------------------------------
01. Bitmap          [b2]  |  +   +   +   +    +   +    +   +     +   +    +  +
02. Transaction ID  [n4]  |  +   +   +   +    +   +    +   +     +   +    +  +
03. PAN             [n16] |          +   =    +   =    +   =     +   =
04. Cardholder FCs  [v25] |          +   =    +   =    +   =     +   =
05. Expiry Date     [n4]  |          +   =    +   =    +   =     +   =
06. PIN             [n4]  |          +        +        +
07. Amount          [n12] |          +   =    +   =        +     +   =
08. Transaction No  [n10] |          +   =    +   =              +   =
09. RRN             [n12] |              +        +              +   =
10. Text Data       [v255]|                                               +  +
11. Terminal ID     [v10] |  +   =   +   =    +   =    +   =     +   =    +  =
12. RC              [n2]  |      +       +        +        +         +       +
13. Date Time       [n10] |  +   +   +   +    +   +    +   +     +   +    +  +
14. CRC             [b4]  |  +   +   +   +    +   +    +   +     +   +    +  +
-----------------------------------------------------------------------------------
"""


class Bitmap:
    test_rq_bits       = [0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0]
    sale_rq_bits       = [0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0]
    refund_rq_bits     = [0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0]
    balance_rq_bits    = [0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0]
    upload_rq_bits     = [0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0]
    settlement_rq_bits = [0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0]

    @classmethod
    def hexlify(cls, bits):
        assert len(bits) == 16
        chunks = [''.join(str(bit) for bit in bits[(4 * i): (4 * i + 4)]) for i in range(4)]
        return ''.join([hex(int(ch, 2)).replace('0x', '') for ch in chunks])

    @classmethod
    def unhexlify(cls, bitmap):
        return '0' + bin(int(bitmap, 16))[2:]

    @classmethod
    def from_action(cls, action):
        if action == 'test':
            return cls.test_rq()
        elif action == 'sale':
            return cls.sale_rq()
        elif action == 'refund':
            return cls.refund_rq()
        elif action == 'balance':
            return cls.balance_rq()
        elif action == 'settlement':
            return cls.settlement_rq()
        elif action == 'upload':
            return cls.upload_rq()

    @classmethod
    def test_rq(cls):
        return cls.hexlify(cls.test_rq_bits)

    @classmethod
    def sale_rq(cls):
        return cls.hexlify(cls.sale_rq_bits)

    @classmethod
    def refund_rq(cls):
        return cls.hexlify(cls.refund_rq_bits)

    @classmethod
    def balance_rq(cls):
        return cls.hexlify(cls.balance_rq_bits)

    @classmethod
    def upload_rq(cls):
        return cls.hexlify(cls.upload_rq_bits)

    @classmethod
    def settlement_rq(cls):
        return cls.hexlify(cls.settlement_rq_bits)
