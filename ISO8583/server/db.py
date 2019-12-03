import json
import models as m
import exceptions as e


class DB:
    cards = []
    sessions = []

    def __init__(self):
        self.load_test_data()

    def post_card(self, _card: m.Card):
        self.cards.append(_card)

    def get_card(self, PAN: str):
        for card in self.cards:
            if card.PAN == PAN:
                return card
        else:
            return None

    def patch_card(self, _card: m.Card):
        for card in self.cards:
            if card.get_uuid() == _card.uuid():
                card.update(_card)
        else:
            self.post_card(_card)

    def post_session(self, _session: m.Session):
        self.sessions.append(_session)

    def get_session(self, terminal_id: str):
        for session in self.sessions:
            if session.get_terminal_id() == terminal_id:
                return session
        else:
            return None

    def is_session_active(self, terminal_uuid):
        return terminal_uuid in [
            session.get_terminal_uuid() for session in self.sessions]

    def patch_session(self, _session: m.Session):
        for session in self.session:
            if session.get_uuid() == _session.uuid():
                session.update(_session)
        else:
            self.post_session(_session)

    def load_test_data(self):
        with open('test_data.json', 'r') as fout:
            data = json.load(fout)

        for card_json_key in data['cards']:
            card_json = data['cards'][card_json_key]
            card = m.Card(
                card_json['pan'],
                card_json['exp_date'],
                card_json['cvv'],
                card_json['pin'],
                card_json['holder_fcs'],
                card_json['balance'],
                card_json['type']
            )
            self.post_card(card)


