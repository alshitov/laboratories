import requests


class Request:
    base_headers = {
        'Host': '127.0.0.1:5000',
        'Origin': 'http://127.0.0.1:5000/',
        'User-Agent': 'POS-Terminal; UUID: {0}',
        'Accept': 'application/json, text/plain',
        'Accept-Language': 'en-US,en;q=0.5',
    }

    @classmethod
    def make_headers(cls, terminal_uuid):
        return cls.base_headers.format(terminal_uuid)

    @classmethod
    def get(cls, url, headers):
        return requests.get(url, headers)

    @classmethod
    def post(cls, url, headers, body):
        return requests.post(url, headers, body)

    @classmethod
    def patch(cls, url, headers, body):
        return requests.patch(url, headers, body)
