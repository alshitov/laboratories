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
05. Expiry Date     [v25] |          +   =    +   =    +   =     +   =
05. PIN             [n4]  |          +        +        +
07. Amount          [n12] |          +   =    +   =        +     +   =
08. Transaction No  [n10] |          +   =    +   =              +   =
09. RRN             [n12] |              +        +              +   =
10. Text Data       [v255]|                                               +  +
11. Terminal ID     [v10] |  +   =   +   =    +   =    +   =     +   =    +  =
12. RC              [n2]  |      +       +        +        +         +       +
13. Date Time       [n10] |  +   +   +   +    +   +    +   +     +   +    +  +
14. CRC             [b4]  |  +   +   +   +    +   +    +   +     +   +    +  +
-----------------------------------------------------------------------------------

Transaction ID values:
-----------------------------------------------------------
                Financial       Other
                Sale   Refund   Test   Balance   Settlement
-----------------------------------------------------------
First:
            RQ  0110   0130     0010   0210      0530
            RS  0120   0140     0020   0220      0540
--------------
Morths into
Reversal:
            RQ  0111   0131
            RS  0121   0141
--------------
Morths into
Upload:
            RQ  0510   0530
            RS  0520   0540
-----------------------------------------------------------
"""


class Bitmap:
    @classmethod
    def bitmap(cls):
        pass
