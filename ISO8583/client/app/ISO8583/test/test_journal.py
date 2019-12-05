import unittest
import tempfile
import shutil
from unittest.mock import patch, mock_open, MagicMock
import os
from src.journal import ClientLogger
from src.helpers import DateTime


class TestClientLogger(unittest.TestCase):
    template = "[DateTime: %s] " \
               "[Terminal: %s] " \
               "Transaction (%s)" \
               "~> Server Response: %s"

    data = {
        'terminal_id': '1234eda560',
        'transaction': '00020160160010c8981069c612041207115821896a0003',
        'response': '000201601e002000000000001375b1841851001204222218a7d0d650003',
    }

    content = template %(
        '1204222218',
        data['terminal_id'],
        data['transaction'],
        data['response']
    )

    def test__file(self):
        terminal_id = '1234eda560'
        self.assertEqual(ClientLogger._file(terminal_id), './log/terminal_1234eda560.txt')

    def test_create(self):
        terminal_id = '1234eda560'
        ClientLogger._file = MagicMock(return_value='terminal_1234eda560.txt')
        file = ClientLogger._file()

        open_mock = mock_open()
        with patch("src.journal.ClientLogger.create", open_mock, create=True):
            ClientLogger.create(file, 'a')

        open_mock.assert_called_once_with(file, 'a')

    def test_log(self):
        ClientLogger._file = MagicMock(return_value='terminal_1234eda560.txt')
        file = ClientLogger._file()

        with patch("src.journal.ClientLogger.log", mock_open()) as mocked_file:
            ClientLogger.log(file, 'a').write(self.content)
            mocked_file.assert_called_once_with(file, 'a')
            mocked_file().write.assert_called_once_with(self.content)


if __name__ == '__main__':
    unittest.main()
