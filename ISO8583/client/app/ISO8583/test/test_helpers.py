import unittest
from src.helpers import DateTime


class TestDateTime(unittest.TestCase):
    def test_is_later(self):
        truthy_set = ('1231235959', '1231235958')
        falsy_set = ('0101000000', '0101000001')

        self.assertEqual(DateTime.is_later(*truthy_set), True)
        self.assertEqual(DateTime.is_later(*falsy_set), False)
