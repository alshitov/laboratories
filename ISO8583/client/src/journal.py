from helpers import DateTime


class ClientLogger:
    template = "[DateTime: %s] " \
               "[Terminal: %s] " \
               "Transaction (%s)" \
               "~> Server Response: %s"

    @classmethod
    def _file(cls, terminal_uuid):
        return "../log/terminal_%s.txt" % terminal_uuid

    @classmethod
    def log(cls, data: dict):
        with open(cls._file(data['terminal_uuid']), 'a') as fin:
            fin.write(cls.template % (
                DateTime.datetime(),
                data['terminal_uuid'],
                data['transaction'],
                data['response']
            ))

    @classmethod
    def get(cls, terminal_uuid):
        with open(cls._file(terminal_uuid), 'r') as fout:
            return {'text': fout.read()}
