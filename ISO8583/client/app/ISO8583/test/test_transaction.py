import unittest
from src.transaction import Transaction, action_codes, to_log


class TestTransaction(unittest.TestCase):
    def test_format_cardholder_name(self):
        cardholder_name = 'LEONARDODAVINCI'
        cardholder_name_w_spaces = 'LEONARDO DA VINCI'
        result = '154c454f4e4152444f444156494e4349'

        self.assertEqual(
            Transaction.format_cardholder_name(cardholder_name),
            result
        )

        self.assertEqual(
            Transaction.format_cardholder_name(cardholder_name_w_spaces),
            result
        )

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
