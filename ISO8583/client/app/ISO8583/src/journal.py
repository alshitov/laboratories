import os
from .helpers import DateTime


class ClientLogger:
    template = "[DateTime: %s] " \
               "[Terminal: %s] " \
               "Transaction (%s)" \
               "~> Server Response: %s"

    @classmethod
    def _file(cls, terminal_id):
        return "./log/terminal_%s.txt" % terminal_id

    @classmethod
    def create(cls, terminal_id):
        open(cls._file(terminal_id), 'a').close()

    @classmethod
    def log(cls, data: dict):
        print('LOG', data)
        with open(cls._file(data['terminal_id']), 'a') as fin:
            fin.write(cls.template % (
                DateTime.datetime(),
                data['terminal_id'],
                data['transaction'],
                data['response']
            ) + '\n')


    @classmethod
    def get(cls, terminal_id):
        with open(cls._file(terminal_id), 'r') as fout:
            return fout.read()
