class ServerError(Exception):
    def __init__(self, status="ERROR", message=None, code=400, stack_trace=None):
        super().__init__()
        self.status = status
        self.message = message
        self.code = code


class AuthError(ServerError):
    def __init__(self, status="AUTH_REQUIRED", message=None, code=None):

        super().__init__(
            status=status,
            message=message
        )

        if code is None:
            if status == "FORBIDDEN":
                self.code = 403
            else:
                self.code = 401
        else:
            self.code = code


class ParserArgsError(ServerError):
    def __init__(self, status="ARGS_ERROR", message=None):

        if message is not None and 'message' in message:
            message = message['message']

        super().__init__(
            status=status,
            message=message,
            code=400
        )


class DBError(ServerError):
    def __init__(self, status="DB_ERROR", message=None):

        if message is not None and 'message' in message:
            message = message['message']

        super().__init__(
            status=status,
            message=message,
            code=400
        )
