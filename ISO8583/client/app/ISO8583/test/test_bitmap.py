import unittest
from src.bitmap import Bitmap


class TestBitmap(unittest.TestCase):
    def test_hexlify(self):
        bits = [
            [0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0],
            [0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0],
            [0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0],
            [0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0],
            [0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0],
            [0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0],
        ]

        results = [
            '6016',
            '7f96',
            '7f96',
            '7e16',
            '7dd6',
            '6036',
        ]

        for i, bitset in enumerate(bits):
            result = Bitmap.hexlify(bitset)
            self.assertEqual(result, results[i])

    def test_unhexlify(self):
        bitmaps = [
            '6016',
            '7f96',
            '7f96',
            '7e16',
            '7dd6',
            '6036',
        ]

        bits = [
            '0110000000010110',
            '0111111110010110',
            '0111111110010110',
            '0111111000010110',
            '0111110111010110',
            '0110000000110110',
        ]

        for i, bitmap in enumerate(bitmaps):
            result = Bitmap.unhexlify(bitmap)
            self.assertEqual(result, bits[i])

    def test_from_action(self):
        actions = [
            'test',
            'sale',
            'refund',
            'balance',
            'upload',
            'settlement',
        ]
        bitmaps = [
            '6016',
            '7f96',
            '7f96',
            '7e16',
            '7dd6',
            '6036',
        ]

        for i, action in enumerate(actions):
            result = Bitmap.from_action(action)
            self.assertEqual(result, bitmaps[i])

    def test_test_rs(self):
        result = Bitmap.test_rs()
        self.assertEqual(result, '6016')

    def test_sale_rs(self):
        result = Bitmap.sale_rs()
        self.assertEqual(result, '7f96')

    def test_refund_rs(self):
        result = Bitmap.refund_rs()
        self.assertEqual(result, '7f96')

    def test_balance_rs(self):
        result = Bitmap.balance_rs()
        self.assertEqual(result, '7e16')

    def test_upload_rs(self):
        result = Bitmap.upload_rs()
        self.assertEqual(result, '7dd6')

    def test_settlement_rs(self):
        result = Bitmap.settlement_rs()
        self.assertEqual(result, '6036')


if __name__ == '__main__':
    unittest.main()
