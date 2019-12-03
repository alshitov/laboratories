import requests


class Request:
    user_agent = 'POS-Terminal; UUID: {0}'
    base_headers = {
        'Host': '127.0.0.1:5000',
        'Origin': 'http://127.0.0.1:5000/',
        'User-Agent': 'POS-Terminal; UUID: {0}',
        'Accept': 'application/json, text/plain',
        'Accept-Language': 'en-US,en;q=0.5',
    }
    host_max_timeout = 5

    @classmethod
    def compose_headers(cls, terminal_id):
        return cls.base_headers.update({
            'User-Agent': cls.user_agent.format(terminal_id)
        })

    @classmethod
    def get(cls, terminal_id, url):
        return requests.get(
            url=url,
            headers=cls.compose_headers(terminal_id),
        )

    @classmethod
    def post(cls, terminal_id, url, body):
        return requests.post(
            url=url,
            headers=cls.compose_headers(terminal_id),
            json=body,
        )

    @classmethod
    def patch(cls, terminal_id, url, body):
        return requests.patch(
            url=url,
            headers=cls.compose_headers(terminal_id),
            json=body,
        )

