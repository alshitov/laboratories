import unittest
from src.transaction import Transaction, action_codes, to_log


class TestTransaction(unittest.TestCase):
    def test_pad(self):
        strings = [
            '1', '12', '123', '1234', '12345', '123456', '1234567', '12345678',
            '123456789', '1234567890', '12345678901', '123456789012'
        ]

        desired_lengths = [
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
        ]

        results = [
            [
                '1'
            ],
            [
                '01',
                '12'
            ],
            [
                '001',
                '012',
                '123'
            ],
            [
                '0001',
                '0012',
                '0123',
                '1234'
            ],
            [   # 5
                '00001',
                '00012',
                '00123',
                '01234',
                '12345'
            ],
            [   # 6
                '000001',
                '000012',
                '000123',
                '001234',
                '012345',
                '123456'
            ],
            [   # 7
                '0000001',
                '0000012',
                '0000123',
                '0001234',
                '0012345',
                '0123456',
                '1234567'
            ],
            [   # 8
                '00000001',
                '00000012',
                '00000123',
                '00001234',
                '00012345',
                '00123456',
                '01234567',
                '12345678'
            ],
            [   # 9
                '000000001',
                '000000012',
                '000000123',
                '000001234',
                '000012345',
                '000123456',
                '001234567',
                '012345678',
                '123456789'
            ],
            [   # 10
                '0000000001',
                '0000000012',
                '0000000123',
                '0000001234',
                '0000012345',
                '0000123456',
                '0001234567',
                '0012345678',
                '0123456789',
                '1234567890'
            ],
            [   # 11
                '00000000001',
                '00000000012',
                '00000000123',
                '00000001234',
                '00000012345',
                '00000123456',
                '00001234567',
                '00012345678',
                '00123456789',
                '01234567890',
                '12345678901'

            ],
            [   # 12
                '000000000001',
                '000000000012',
                '000000000123',
                '000000001234',
                '000000012345',
                '000000123456',
                '000001234567',
                '000012345678',
                '000123456789',
                '001234567890',
                '012345678901',
                '123456789012'
            ],
        ]

        for i, length in enumerate(desired_lengths):
            for j in range(length):
                string = strings[j]
                result = Transaction.pad(string, length)
                self.assertEqual(result, results[i][j])

    # As long as Transaction.pad is tested, it does not make
    # big sense in testing all the pad variants in further methods

    def test_format_cardholder_name(self):
        cardholder_name = 'LEONARDODAVINCI'
        cardholder_name_w_spaces = 'LEONARDO DA VINCI'
        result = '304c454f4e4152444f444156494e4349'

        self.assertEqual(
            Transaction.format_cardholder_name(cardholder_name),
            result
        )

        self.assertEqual(
            Transaction.format_cardholder_name(cardholder_name_w_spaces),
            result
        )

    def test_format_amount(self):
        amount = '123462'
        result = '000000123462'
        self.assertEqual(Transaction.format_amount(amount), result)

    def test_format_transaction_no(self):
        transaction_no = '123'
        result = '0000000123'
        self.assertEqual(Transaction.format_transaction_no(transaction_no), result)

    def test_format_RRN(self):
        RRN = '542'
        result = '000000000542'
        self.assertEqual(Transaction.format_RRN(RRN), result)

    def test_format_text_data(self):
        sales_count = 13
        sales_sum   = 12341.55
        refund_count = 3
        refund_sum = 3253.12

        result = '013000001234155003000000325312'
        self.assertEqual(
            Transaction.format_text_data(
                sales_count,
                sales_sum,
                refund_count,
                refund_sum
            ),
            result
        )

    def test_transaction(self):
        # -----
        # Positional set (bitmap, transaction_id, terminal_id)
        bitmap = '6016'
        transaction_id = '0020'
        terminal_id = '7aedf2340a'

        # Common financial set (PAN, cardholder_name, expiry_date, PIN)
        PAN = "54024372342320346"
        cardholder_name = "ERICBTHEPRESIDENT"
        expiry_date = "2205"
        PIN = "9873"
        # -----

        # Test set ()
        result = Transaction.transaction(
            bitmap=bitmap,
            transaction_id=transaction_id,
            terminal_id=terminal_id
        )
        self.assertEqual(result[:-22], "000201601600207aedf2340a")

        # Sale/refund set (...positional set, ...common financial set, amount, transaction_no)
        amount = 1325.43
        transaction_no = 123

        result = Transaction.transaction(
            bitmap=bitmap,
            transaction_id=transaction_id,
            PAN=PAN,
            cardholder_name=cardholder_name,
            expiry_date=expiry_date,
            PIN=PIN,
            amount=amount,
            transaction_no=transaction_no,
            terminal_id=terminal_id
        )
        self.assertEqual(result[:-22], "0002016016002054024372342320346344552494342544845505245534944454e542205987300000013254300000001237aedf2340a")

        # Balance set (...positional_set, ...common financial set)
        result = Transaction.transaction(
            bitmap=bitmap,
            transaction_id=transaction_id,
            PAN=PAN,
            cardholder_name=cardholder_name,
            expiry_date=expiry_date,
            PIN=PIN,
            terminal_id=terminal_id
        )
        self.assertEqual(result[:-22], "0002016016002054024372342320346344552494342544845505245534944454e54220598737aedf2340a")

        # Upload set (...positional set, ...common financial set, transaction_no, RRN)
        RRN = 3
        result = Transaction.transaction(
            bitmap=bitmap,
            transaction_id=transaction_id,
            PAN=PAN,
            cardholder_name=cardholder_name,
            expiry_date=expiry_date,
            transaction_no=transaction_no,
            RRN=RRN,
            terminal_id=terminal_id
        )
        self.assertEqual(result[:-22], "0002016016002054024372342320346344552494342544845505245534944454e54220500000001230000000000037aedf2340a")

        # Settlement set (text_data)
        text_data = '01200003242760010000005672'
        result = Transaction.transaction(
            bitmap=bitmap,
            transaction_id=transaction_id,
            text_data=text_data,
            terminal_id=terminal_id
        )
        self.assertEqual(result[:-22], "00020160160020012000032427600100000056727aedf2340a")
