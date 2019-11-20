from PyQt5 import QtWidgets, QtCore, QtGui
import os
import shutil


class DirList(QtWidgets.QListWidget):

    to_other_dirlist = QtCore.pyqtSignal(str, str, str)

    def __init__(self):
        super().__init__()

        self.state = {
            'record': True,
            'other': None,
            'directory': '',
            'files': [],
            'directories': [],
            'chosen_file': '',
            'chosen_directory': ''
        }
        self.itemClicked.connect(self.select_item)
        self.itemDoubleClicked.connect(self.go_to)

    def set_record(self, record):
        self.state['record'] = record

    def set_other(self, dir_proc):
        self.state['other'] = dir_proc

    def get_dir(self):
        return self.state['directory']

    def set_directory(self, dirname):
        self.state['directory'] = dirname
        self.list_directory()

    def list_directory(self):
        self.clear()

        items = []
        entries = os.listdir(self.state['directory'])

        for entry in entries:
            is_dir = False

            if os.path.isdir(os.path.join(self.state['directory'], entry)):
                self.state['directories'].append(entry)
                is_dir = True

            if os.path.isfile(os.path.join(self.state['directory'], entry)):
                self.state['files'].append(entry)

            entry_str = '{0}\t{1}\t{2}'
            items.append(
                entry_str.format(
                    entry + '/' if is_dir else entry,
                    '%s KB' % format(round(os.path.getsize(
                        self.full_path(entry)) / 1024, 3), '.3f'),
                    oct(os.stat(self.full_path(entry)).st_mode)[-3:]
                )
            )

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
                if self.state['directory']:
                    self.call_context_menu(event)

        super().mousePressEvent(event)

    def select_item(self, item):
        item_name = item.text().split('/')[0].replace('/', '')

        if item_name not in ['.', '..']:
            if item_name in self.state['files']:
                self.state['chosen_file'] = item_name
                self.state['chosen_directory'] = ''
            elif item_name in self.state['directories']:
                self.state['chosen_directory'] = item_name
                self.state['chosen_file'] = ''
        else:
            self.state['chosen_directory'] = item_name

    def go_to(self):
        if self.state['chosen_file']:
            os.system('open {0}'.format(
                self.full_path_from_dir(
                    self.state['directory'],
                    self.state['chosen_file']
                )
            ))
        else:
            self.state['directory'] = self.full_path(
                self.state['chosen_directory'])

            self.list_directory()

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
        name, ok = QtWidgets.QInputDialog.getText(
            self, 'New entry', 'Entry name:')
        if ok:
            open(self.full_path(name), 'a').close()
            self.to_other_dirlist.emit(
                '',
                'add_file',
                self.full_path_from_dir(
                    self.state['other'].dir_list.get_dir(),
                    name
                )
            )
            self.list_directory()

    def add_directory(self):
        name, ok = QtWidgets.QInputDialog.getText(
            self, 'New entry', 'Entry name:')
        if ok:
            os.mkdir(self.full_path(name))
            self.to_other_dirlist.emit(
                '',
                'add_directory',
                self.full_path_from_dir(
                    self.state['other'].dir_list.get_dir(),
                    name
                )
            )
            self.list_directory()

    def remove_file(self):
        os.remove(self.full_path(self.state['chosen_file']))
        self.to_other_dirlist.emit(
            self.full_path_from_dir(
                self.state['other'].dir_list.get_dir(),
                self.state['chosen_file']
            ),
            'remove_file',
            ''
        )
        self.list_directory()

    def rename_file(self):
        name, ok = QtWidgets.QInputDialog.getText(
            self, 'Rename file', 'New file name:')
        if ok:
            os.rename(
                self.full_path(self.state['chosen_file']),
                self.full_path(name)
            )
            self.to_other_dirlist.emit(
                self.full_path_from_dir(
                    self.state['other'].dir_list.get_dir(),
                    self.state['chosen_file']
                ),
                'rename_file',
                self.full_path(name)
            )
            self.list_directory()

    def move_file(self):
        dirname = QtWidgets.QFileDialog.getExistingDirectoryUrl(
            self, "Choose directory", QtCore.QUrl("/home/alexander"))
        abs_path = QtCore.QUrl.toString(dirname)

        if abs_path:
            path = abs_path.split("file://")[1]
            os.rename(
                self.full_path(self.state['chosen_file']),
                os.path.join(path, self.state['chosen_file'])
            )
            self.to_other_dirlist.emit(
                self.full_path_from_dir(
                    self.state['other'].dir_list.get_dir(),
                    self.state['chosen_file']
                ),
                'move_file',
                os.path.join(path, self.state['chosen_file'])
            )
            self.list_directory()

    def remove_directory(self):
        os.rmdir(self.full_path(self.state['chosen_directory']))
        self.to_other_dirlist.emit(
            self.full_path_from_dir(
                self.state['other'].dir_list.get_dir(),
                self.state['chosen_directory']
            ),
            'remove_directory',
            ''
        )
        self.list_directory()

    def rename_directory(self):
        name, ok = QtWidgets.QInputDialog.getText(
            self, 'Rename file', 'New directory name:')
        if ok:
            os.rename(
                self.full_path(self.state['chosen_directory']),
                self.full_path(name)
            )
            self.to_other_dirlist.emit(
                self.full_path_from_dir(
                    self.state['other'].dir_list.get_dir(),
                    self.state['chosen_directory']
                ),
                'rename_directory',
                self.full_path(name)
            )
            self.list_directory()

    def move_directory(self):
        dirname = QtWidgets.QFileDialog.getExistingDirectoryUrl(
            self, "Choose directory", QtCore.QUrl("/home/alexander"))
        abs_path = QtCore.QUrl.toString(dirname)

        if abs_path:
            path = abs_path.split("file://")[1]
            os.rename(
                self.full_path(self.state['chosen_directory']),
                os.path.join(path, self.state['chosen_directory'])
            )
            self.to_other_dirlist.emit(
                self.full_path_from_dir(
                    self.state['other'].dir_list.get_dir(),
                    self.state['chosen_directory']
                ),
                'move_directory',
                os.path.join(path, self.state['chosen_directory'])
            )
            self.list_directory()

    def full_path(self, file):
        return os.path.join(self.state['directory'], file)

    def full_path_from_dir(self, dir_, file):
        return os.path.join(dir_, file)

    @QtCore.pyqtSlot(str, str, str)
    def from_other_dirlist(self, entry, action, user_input):
        print(entry, action, user_input)
        if self.state['record']:
            if action == 'add_file':
                open(self.full_path(user_input), 'a').close()

            if action == 'add_directory':
                os.mkdir(self.full_path(user_input))

            if action in ['remove_file', 'remove_directory']:
                os.remove(entry)

            if action in ['rename_file', 'move_file',
                          'rename_directory', 'move_directory']:
                os.rename(entry, user_input)

        self.list_directory()
