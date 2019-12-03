#!/usr/bin/env python3
from flask import request
from server import app
import decorators
import service
import parser


@app.route('/', methods=['GET'])
@decorators.response_decorator
def index():
    return {'host': 'POS-terminals host on 127.0.0.1:5000'}


@app.route('/transaction', methods=['GET'])
@decorators.response_decorator
def transaction():
    transaction_data = request.args.get('data')
    return service.transaction(transaction_data)
