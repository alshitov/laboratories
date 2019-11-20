from PyQt5 import QtWidgets, QtCore, QtGui
import os


class DirList(QtWidgets.QListWidget):
    def __init__(self):

        super().__init__()

        self.state = {
            'files': [],
            'directories': [],
            'chosen_file': '',
            'chosen_directory': ''
        }
        self.itemClicked.connect(self.select_item)

    def list_directory(self, directory):
        self.clear()

        items = []
        entries = os.listdir(directory)

        for entry in entries:
            if os.path.isdir(os.path.join(directory, entry)):
                items.append(entry + '/')        
                self.state['directories'].append(entry)
            elif os.path.isfile(os.path.join(directory, entry)):
                items.append(entry)        
                self.state['files'].append(entry)

        items.extend(['./', '../'])
        self.addItems(sorted(items))

    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            self.state['chosen_directory'] = ''
            self.state['chosen_file'] = ''

        if event.button() == QtCore.Qt.RightButton:
            if self.state['chosen_file'] or self.state['chosen_directory']:
                self.call_file_context_menu(event)
            else:
                self.call_context_menu(event)

        super().mousePressEvent(event)

    def select_item(self, item):
        item_name = item.text().replace('/', '')

        if item_name not in ['.', '..']:
            if item_name in self.state['files']:
                self.state['chosen_file'] = item_name
                self.state['chosen_directory'] = ''
            elif item_name in self.state['directories']:
                self.state['chosen_directory'] = item_name
                self.state['chosen_file'] = ''

    def call_file_context_menu(self, event):
        menu = QtWidgets.QMenu(self)
        remove_action = QtWidgets.QAction("Удалить", self)
        rename_action = QtWidgets.QAction("Переименовать", self)
        move_action = QtWidgets.QAction("Переместить", self)

        if self.state['chosen_file']:
            remove_action.triggered.connect(self.remove_file)
            rename_action.triggered.connect(self.rename_file)
            move_action.triggered.connect(self.move_file)

        elif self.state['chosen_directory']:
            remove_action.triggered.connect(self.remove_directory)
            rename_action.triggered.connect(self.rename_directory)
            move_action.triggered.connect(self.move_directory)

        menu.addAction(remove_action)
        menu.addAction(rename_action)
        menu.addAction(move_action)
        menu.popup(QtCore.QPoint(self.viewport().mapToGlobal(
            QtCore.QPoint(event.x(), event.y()))))

    def call_context_menu(self, event):
        menu = QtWidgets.QMenu(self)
        new_file_action = QtWidgets.QAction("Новый файл", self)
        new_file_action.triggered.connect(self.add_file)
        new_dir_action = QtWidgets.QAction("Новая директория", self)
        new_dir_action.triggered.connect(self.add_directory)
        menu.addAction(new_file_action)
        menu.addAction(new_dir_action)
        menu.popup(QtCore.QPoint(self.viewport().mapToGlobal(
            QtCore.QPoint(event.x(), event.y()))))

    def add_file(self):
        print('add_file')

    def add_directory(self):
        print('add_directory')

    def remove_file(self):
        print('remove_file')

    def rename_file(self):
        print('rename_file')

    def move_file(self):
        print('move_file')

    def remove_directory(self):
        print('remove_directory')

    def rename_directory(self):
        print('rename_directory')

    def move_directory(self):
        print('move_directory')
