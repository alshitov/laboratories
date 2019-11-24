from flask import jsonify
from server import app, database, logger
import exceptions as e


def goods():
    return database.get_goods()


def good(good_uuid):
    good = database.get_good(good_uuid)
    return good.to_dict()


def sell(good_uuid, args):
    pass


def refund(transaction_uuid):
    pass


def transaction(transaction_uuid):
    transaction = database.get_transaction(transaction_uuid)
    return transaction.to_dict()


def card(card_uuid):
    card = database.get_card(card_uuid)
    return card.to_dict()


def balance(card_uuid):
    card = database.get_card(card_uuid)
    return {'balance': card.balance}


def session(terminal_uuid, args):
    pass


def settlement(terminal_uuid):
    pass


def test_terminal_connection(terminal_uuid):
    session = database.get_session(terminal_uuid)
    return {
        'session': session.to_dict(),
        'status': 'active'
    }
