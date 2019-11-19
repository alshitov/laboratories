from PyQt5 import QtWidgets, QtCore, QtGui
import os


class DirList(QtWidgets.QListWidget):
    def __init__(self):

        super().__init__()

        self.files = []
        self.directories = []

    def list_directory(self, directory):
        entries = os.listdir(directory)
        items = [
            entry + '/' if os.path.isdir(os.path.join(directory, entry))
            else entry
            for entry in entries
        ]

        items.extend(['./', '../'])
        self.addItems(sorted(items))

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
