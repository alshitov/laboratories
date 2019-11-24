#!/usr/bin/env python3

from server import app
import decorators
import service
import parser


@app.route('/', methods=['GET'])
@decorators.response_decorator
def index():
    return {'host': 'POS-terminals host on 127.0.0.1:5000'}


@app.route('/good/<good_uuid>', methods=['GET'])
@decorators.response_decorator
def good(good_uuid):
    return service.good(good_uuid)


@app.route('/good/goods', methods=['GET'])
@decorators.response_decorator
def goods():
    return service.goods()


@app.route('/goods/<good_uuid>/buy', methods=['GET'])
@decorators.response_decorator
def sell(good_uuid):
    args = parser.buy()
    return service.buy(good_uuid, args)


@app.route('/goods/refund/<transaction_uuid>', methods=['POST'])
@decorators.response_decorator
def refund(transaction_uuid):
    return service.refund(transaction_uuid)


@app.route('/transactions/<transaction_uuid>', methods=['GET'])
@decorators.response_decorator
def transaction(transaction_uuid):
    return service.transaction(transaction_uuid)


@app.route('/cards/<card_uuid>', methods=['GET'])
@decorators.response_decorator
def card(card_uuid):
    return service.card(card_uuid)


@app.route('/cards/<card_uuid>/balance', methods=['GET'])
@decorators.response_decorator
def balance(card_uuid):
    return service.balance(card_uuid)


@app.route('/terminal/<terminal_uuid>/session', methods=['POST'])
@decorators.response_decorator
def session(terminal_uuid):
    args = parser.session()
    return service.session(terminal_uuid, args)


@app.route('/terminal/<terminal_uuid>/settlement', methods=['GET'])
@decorators.response_decorator
def settlement(terminal_uuid):
    return service.settlement(terminal_uuid)


@app.route('/terminal/<terminal_uuid>/test-connection', methods=['GET'])
def test_terminal_connection(terminal_uuid):
    return service.test_terminal_connection(terminal_uuid)
