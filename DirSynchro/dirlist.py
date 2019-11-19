from PyQt5 import QtWidgets, QtCore, QtGui


class DirList(QtWidgets.QListWidget):
    def __init__(self):

        super().__init__()

        self.files = []
        self.directories = []

    def add_file(self):
        pass

    def remove_file(self):
        pass

    def rename_file(self):
        pass

    def add_directory(self):
        pass

    def remove_directory(self):
        pass

    def rename_directory(self):
        pass
