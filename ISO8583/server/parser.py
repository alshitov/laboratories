from flask_restful import reqparse
from werkzeug.exceptions import BadRequest
import exceptions
import models


class Parser:
    def sell(self):
        try:
            parser = reqparse.RequestParser(bundle_errors=True)
            parser.add_argument('good_uuid', type=str, location='args', required=True,
                                help='good_uuid must be a 128-bit hex')
            parser.add_argument('quantity', type=int, location='args', required=True,
                                help='quantity must be a positive integer', default=1)
            parser.add_argument('session_uuid', type=str, location='args', required=True,
                                help='session_uuid must be a 128-bit hex')
            args = parser.parse_args()
        except BadRequest as bre:
            raise exceptions.ArgsParseError(message=bre.data)
        return args

    def session(self):
        try:
            parser = reqparse.RequestParser(bundle_errors=True)
            parser.add_argument('terminal_uuid', type=str, location='args', reqired=True,
                                help='terminal_uuid must be a 128-bit hex')
            args = parser.parse_args()
        except BadRequest as bre:
            raise exceptions.ArgsParseError(message=bre.data)

        return args

    def settlement(self):
        try:
            parser = reqparse.RequestParser(bundle_errors=True)
            parser.add_argument('sell_counter', type=int, location='args', reqired=True,
                                help='sell_counter must be a positive integer')
            parser.add_argument('sell_total', type=float, location='args', reqired=True,
                                help='sell_total must be a positive float')
            parser.add_argument('refund_count', type=float, location='args', reqired=True,
                                help='refund_count must be a positive integer')
            parser.add_argument('refund_total', type=float, location='args', reqired=True,
                                help='refund_total must be a positive float')
            args = parser.parse_args()
        except BadRequest as bre:
            raise exceptions.ArgsParseError(message=bre.data)

        return args
