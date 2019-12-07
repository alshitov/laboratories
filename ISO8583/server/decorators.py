from flask import jsonify
import exceptions
import functools


def response_decorator(f):
    @functools.wraps(f)
    def wrapper(*args, **kwargs):
        try:
            result = f(*args, **kwargs)
            if isinstance(result, tuple):
                code, status = 400, "Not Found"
            else:
                code, status = 200, 'SUCCESS'

            return jsonify({
                'result': result,
                'code': code,
                'status': status,
            })

        except exceptions.AuthError as ae:
            return jsonify({
                'code': ae.code,
                'status': ae.status,
                'message': ae.message
            })

        except exceptions.ServerError as se:
            return jsonify({
                'code': se.code,
                'status': se.status,
                'message': se.message
            })

        except exceptions.DBError as dbe:
            return jsonify({
                'code': dbe.code,
                'status': dbe.status,
                'message': dbe.message
            })

        # except Exception as e:
        #     return jsonify({
        #         'error': e.args,
        #         'code': 500,
        #         'status': "EXCEPTION"
        #     })

    return wrapper
