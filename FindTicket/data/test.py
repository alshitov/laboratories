import abc


class Aggregate(abc.ABC):
    __slots__ = []

    @abc.abstractmethod
    def mk_iterator(self):
        """ Returs iterator """
        pass


class Iterator(abc.ABC):
    __slots__ = []
    
    def __init__(self, collection, cursor):
        self._collection = collection
        self._cursor = cursor

    @abc.abstractmethod
    def first(self):
        """ Restarts iterator """
        pass

    @abc.abstractmethod
    def next(self):
        """ Returns next elem of collection.
        Raises StopIteration when exhausted. """
        pass

    @abc.abstractmethod
    def current(self):
        """ Returns current elem of collection by cursor """
        pass


class ListIterator(Iterator):
    def __init__(self, collection, cursor):
        assert -1 <= cursor < len(collection)
        super().__init__(collection, cursor)

    def first(self):
        self._cursor = -1

    def next(self):
        if self._cursor + 1 >= len(self._collection):
            # Endless
            self.first()
            raise StopIteration()
        self._cursor += 1

    def current(self):
        return self._collection[self._cursor]


class ListCollection(Aggregate):
    def __init__(self, collection):
        self._collection = collection

    def mk_iterator(self):
        return ListIterator(self._collection, -1)

if __name__ == '__main__':
    collection = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    aggregate = ListCollection(collection)
    iterator = aggregate.mk_iterator()

    while True:
        try:
            iterator.next()
            print(iterator.current())
        except StopIteration:
            break
