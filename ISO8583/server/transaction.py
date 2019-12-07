"""
Transaction ID values:
-----------------------------------------------------------
                Financial       Other
                Sale   Refund   Test   Balance   Settlement
-----------------------------------------------------------
First:
            RQ  0110   0130     0010   0210      0430
            RS  0120   0140     0020   0220      0440
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

Transaction Fields according to Protocol v1:
+-------------+------------------+--------------------+----------+
| Start Sign  | Protocol Version | Transaction Values | End Sign |
| 00 02       | 01               | ...                | 00 03    |
+-------------+------------------+--------------------+----------+
"""

from binascii import crc32
import helpers

action_codes = {
    'test': '0020',
    'balance': '0220',
    'settlement': '0440',
    'sale': '0120',
    'refund': '0140'
    # 'sale_reversal': '0121',
    # 'refund_reversal': '0141',
    # 'sale_upload': '0520',
    # 'refund_upload': '0540'
}

to_log = ['0120', '0140', '0121', '0141', '0520', '0540']


class Transaction:
    start_sign = '0002'
    protocol_version = '01'
    end_sign = '0003'

    template = "%(start_sign)s" \
               "%(protocol_version)s" \
               "%(data)s" \
               "%(end_sign)s"

    data_template = "%(bitmap)s" \
                    "%(transaction_id)s" \
                    "%(PAN)s" \
                    "%(cardholder_name)s" \
                    "%(expiry_date)s" \
                    "%(PIN)s" \
                    "%(amount)s" \
                    "%(transaction_no)s" \
                    "%(RRN)s" \
                    "%(text_data)s" \
                    "%(terminal_id)s" \
                    "%(RC)s" \
                    "%(datetime)s" \

    @classmethod
    def format_amount(cls, amount):
        amount_str = str(amount).replace('.', '')
        return '{0}{1}'.format('0' * (12 - len(amount_str)), amount_str)

    @classmethod
    def format_RRN(cls, RRN):
        RRN_str = str(RRN)
        return '{0}{1}'.format('0' * (12 - len(RRN_str)), RRN_str)

    @classmethod
    def format_CRC(cls, transaction):
        return hex(crc32(bytes(transaction, 'utf-8')))[2:]

    @classmethod
    def format_cardholder_name(cls, cardholder_name):
        cardholder_name = cardholder_name.replace(' ', '')
        return str(len(cardholder_name) * 2) + \
            ''.join([hex(ord(l))[2:] for l in cardholder_name])

    @classmethod
    def transaction(cls,
                    bitmap,
                    transaction_id,
                    terminal_id,
                    PAN=None,
                    cardholder_name=None,
                    expiry_date=None,
                    PIN=None,
                    amount=None,
                    transaction_no=None,
                    RRN=None,
                    text_data=None,
                    RC=None):

        print('TO TERMINAL', {
            'bitmap': bitmap,
            'transaction_id': transaction_id,
            'PAN': PAN if PAN else '',
            'cardholder_name': cls.format_cardholder_name(cardholder_name) if cardholder_name else '',
            'expiry_date': expiry_date if expiry_date else '',
            'PIN': PIN if PIN else '',
            'amount': cls.format_amount(amount) if amount else '',
            'transaction_no': cls.format_transaction_no(transaction_no) if transaction_no else '',
            'RRN': cls.format_RRN(RRN) if RRN else '',
            'text_data': cls.format_text_data(text_data) if text_data else '',
            'terminal_id': terminal_id,
            'RC': RC if RC else '',
            'datetime': helpers.DateTime.datetime()
        }, '\n')

        data = cls.data_template % {
            'bitmap': bitmap,
            'transaction_id': transaction_id,
            'PAN': PAN if PAN else '',
            'cardholder_name': cls.format_cardholder_name(cardholder_name) if cardholder_name else '',
            'expiry_date': expiry_date if expiry_date else '',
            'PIN': PIN if PIN else '',
            'amount': cls.format_amount(amount) if amount else '',
            'transaction_no': cls.format_transaction_no(transaction_no) if transaction_no else '',
            'RRN': cls.format_RRN(RRN) if RRN else '',
            'text_data': cls.format_text_data(text_data) if text_data else '',
            'terminal_id': terminal_id,
            'RC': RC if RC else '',
            'datetime': helpers.DateTime.datetime()
        }

        data += cls.format_CRC(data)

        return cls.template % {
            'start_sign': cls.start_sign,
            'protocol_version': cls.protocol_version,
            'data': data.upper(),
            'end_sign': cls.end_sign
        }
