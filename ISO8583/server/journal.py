from flask import jsonify
from models import DateTime


class JournalLogger:
    template = "[DateTime: %s] " \
               "[Terminal: %s] " \
               "[Transaction: %s] " \
               "~> Data: %s"

    def _file(self, termial_uuid):
        return "log/sniffer_terminal_%s.bin" % termial_uuid

    def log(self, data: dict):
        with open(self._file(data['terminal_uuid']), 'ab') as fin:
            fin.write(bytes(self.template % (
                DateTime.datetime(),
                data['termminal_uuid'],
                data['transaction_uuid'],
                data['data']
            )))

    def get(self, terminal_uuid: str):
        with open(self._file(terminal_uuid), 'rb') as fout:
            return jsonify({
                'journal': fout.read().decode('utf-8')
            })
