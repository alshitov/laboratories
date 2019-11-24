import json
import models as m
import exceptions as e


class DB:
    goods = []
    transactions = []
    cards = []
    sessions = []

    def __init__(self):
        self.load_test_data()

    def post_good(self, _good: m.Good):
        self.goods.append(_good)

    def get_good(self, good_uuid: str):
        for good in self.goods:
            if good.get_uuid() == good_uuid:
                return good
        else:
            raise e.DBError(message=str(
                {'error': 'good with uuid: {0} not found'.format(good_uuid)}
            ))

    def get_goods(self):
        return [good.to_dict() for good in self.goods]

    def post_transaction(self, _transaction: m.Transaction):
        self.transactions.append(_transaction)

    def get_transaction(self, transaction_uuid: str):
        for transaction in self.transactions:
            if transaction.get_uuid() == transaction_uuid:
                return transaction
        else:
            raise e.DBError(message=str(
                {'error': 'transaction with uuid: {0} not found'.format(
                    transaction_uuid)}
            ))

    def patch_transaction(self, _transaction: m.Transaction):
        for transaction in self.transactions:
            if transaction.get_uuid() == _transaction.uuid():
                transaction.update(_transaction)
        else:
            self.post_transaction(_transaction)

    def post_card(self, _card: m.Card):
        self.cards.append(_card)

    def get_card(self, card_uuid: str):
        for card in self.cards:
            if card.get_uuid() == card_uuid:
                return card
        else:
            raise e.DBError(message=str(
                {'error': 'card with uuid: {0} not found'.format(card_uuid)}
            ))
        pass

    def patch_card(self, _card: m.Card):
        for card in self.cards:
            if card.get_uuid() == _card.uuid():
                card.update(_card)
        else:
            self.post_card(_card)

    def post_session(self, _session: m.Session):
        self.sessions.append(_session)

    def get_session(self, terminal_uuid: str):
        for session in self.sessions:
            if session.get_terminal_uuid() == terminal_uuid:
                return session
        else:
            raise e.DBError(message=str(
                {'error': 'session with terminal_uuid: {0} not found'.format(
                    terminal_uuid)}
            ))
        pass

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

        for good_json_key in data['goods']:
            good_json = data['goods'][good_json_key]
            good = m.Good(
                good_json['price'],
                good_json['name']
            )
            self.post_good(good)

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


