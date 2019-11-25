import os
import shutil
import subprocess
import stat
from stat import \
    S_IRUSR, S_IWUSR, S_IXUSR, \
    S_IRGRP, S_IWGRP, S_IXGRP, \
    S_IROTH, S_IWOTH, S_IXOTH

from PyQt5 import QtWidgets, QtCore, QtGui
import dirutil


class DirList(QtWidgets.QListWidget):

    to_other_dirlist = QtCore.pyqtSignal(str, str, str)

    def __init__(self, _id):
        super().__init__()

        self.state = {
            # From main window
            'left_to_right': True,
            'record': True,

            # Self
            'id': _id,
            'other': None,              # opposite DirList

            'base_dir': '',             # first chosen directory
            'cur_dir': '',              # current directory

            'files': [],                # list of files to be displayed
            'dirs': [],                 # list of directories to be displayed

            'chosen_file': '',          # active file item
            'chosen_dir': ''            # active directory item
        }

        self.dir_util = dirutil.DirUtil()

        self.itemClicked.connect(self.select_item)
        self.itemDoubleClicked.connect(self.go_to)

        # self.set_base_and_cur_dirs(
        #     '/Users/ympo2oo2/Documents/from' if _id == 'right' else '/Users/ympo2oo2/Documents/to')
        # self.list_directory()

    def pull_main_window_state(self, state):
        self.state.update(state)

    def set_other(self, dir_proc):
        self.state['other'] = dir_proc

    def get_other(self):
        return self.state['other']

    def get_files(self):
        return self.state['files']

    def set_files(self, files):
        self.state['files'] = files

    def get_chosen_file(self):
        return self.state['chosen_file']

    def set_chosen_file(self, file):
        self.state['chosen_file'] = file

    def get_dirs(self):
        return self.state['dirs']

    def set_dirs(self, dirs):
        self.state['dirs'] = dirs

    def get_chosen_dir(self):
        return self.state['chosen_dir']

    def set_chosen_dir(self, dir_):
        self.state['chosen_dir'] = dir_

    def set_base_and_cur_dirs(self, dir_):
        self.state['cur_dir'] = self.state['base_dir'] = dir_
        self.list_directory()

    def set_cur_dir(self, dir_):
        self.state['cur_dir'] = dir_

    def get_cur_dir(self):
        return self.state['cur_dir']

    def get_base_dir(self):
        return self.state['base_dir']

    def list_directory(self):
        self.clear()
        files, dirs, list_items = self.dir_util.list_directory(
            self.get_cur_dir()
        )
        self.state['files'] = files
        self.state['dirs'] = dirs
        self.addItems(sorted(list_items))

    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            self.state['chosen_dir'] = ''
            self.state['chosen_file'] = ''

        if event.button() == QtCore.Qt.RightButton:
            if self.state['chosen_file'] or self.state['chosen_dir']:
                self.call_file_context_menu(event)
            else:
                if self.state['cur_dir']:
                    self.call_context_menu(event)

        super().mousePressEvent(event)

    def select_item(self, item):
        item_name = item.text().split('\t')[0].replace('/', '')

        if item_name not in ['.', '..']:
            if item_name in self.get_files():
                self.set_chosen_file(item_name)
                self.set_chosen_dir('')
            elif item_name in self.state['dirs']:
                self.set_chosen_dir(item_name)
                self.set_chosen_file('')
        else:
            self.set_chosen_dir(item_name)

    def go_to(self):
        if self.state['chosen_file']:
            res = subprocess.call([
                'open',
                self.dir_util.full_path(
                    self.get_cur_dir(),
                    self.get_chosen_file()
                )
            ])
            if res != 0:
                self.raise_error_message(
                    'File not found!',
                    'File {0} has been removed'.format(
                        self.state['chosen_file'])
                )

        else:
            self.set_cur_dir(
                self.dir_util.full_path(
                    self.get_cur_dir(),
                    self.get_chosen_dir()
                )
            )
            self.list_directory()

    def call_file_context_menu(self, event):
        menu = QtWidgets.QMenu(self)
        remove_action = QtWidgets.QAction("Удалить", self)

        if self.get_chosen_file():
            print('file')
            remove_action.triggered.connect(self.remove_file)

        elif self.get_chosen_dir():
            print('dir')
            remove_action.triggered.connect(self.remove_directory)

        menu.addAction(remove_action)
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

    def input_dialog(self, title, text):
        name, ok = QtWidgets.QInputDialog.getText(self, title, text)
        return name, ok

    def add_file(self):
        name, ok = self.input_dialog('New file', 'Entry name:')

        if not ok:
            return

        if name in self.state['files']:
            self.raise_error_message(
                'Conflict!',
                'File: "{0}" already exists!'.format(name),
                self.add_file
            )
        else:
            self.dir_util.add_file(
                self.dir_util.full_path(
                    self.get_cur_dir(),
                    name
                )
            )
            self.list_directory()

            other = self.get_other().dir_list

            rel = self.dir_util.rel_path(
                self.get_cur_dir(),
                self.get_base_dir(),
                other.get_base_dir()
            )

            if rel[-1] == '/':
                rel = rel[:-1]

            if other.get_cur_dir() != rel:
                rel = rel[1:]

            new_dir_other = self.dir_util.full_path(
                other.get_base_dir(),
                rel
            )

            if not self.dir_util.dir_exists(new_dir_other):
                self.dir_util.add_directory(new_dir_other)

            new_full_dir_other = self.dir_util.full_path(
                other.get_base_dir(),
                new_dir_other
            )

            other.set_cur_dir(new_dir_other)

            self.to_other_dirlist.emit(
                '',
                'add_file',
                self.dir_util.full_path(
                    new_full_dir_other,
                    name
                )
            )

    def add_directory(self):
        name, ok = self.input_dialog('New entry', 'Entry name:')

        if not ok:
            return

        if name in self.state['dirs']:
            self.raise_error_message(
                'Conflict!',
                'Directory: "{0}" already exists!'.format(name),
                self.add_directory
            )
        else:
            new_dir_path = self.dir_util.full_path(
                self.get_cur_dir(),
                name
            )

            self.dir_util.add_directory(new_dir_path)

            self.to_other_dirlist.emit(
                '',
                'add_directory',
                self.dir_util.full_path(
                    self.get_other().dir_list.get_base_dir(),
                    self.dir_util.rel_path(
                        new_dir_path,
                        self.get_base_dir(),
                        self.get_other().dir_list.get_base_dir()
                    )[1:]
                )
            )
            self.list_directory()

    def remove_file(self):
        file_path = self.dir_util.full_path(
            self.get_cur_dir(),
            self.get_chosen_file()
        )

        code = self.dir_util.remove_file(file_path)
        if code == 1:
            self.raise_error_message(
                'File no found',
                'File {0} may have been removed'.format(file_path)
            )
        elif code == 2:
            self.raise_error_message(
                'Operation restricted',
                'Unfortunatelly, you do not have enough permissions to continue the action'
            )
        else:
            self.list_directory()

            if self.get_chosen_file() not in self.get_other().dir_list.get_files():
                return

            self.to_other_dirlist.emit(
                self.dir_util.full_path(
                    self.get_other().dir_list.get_base_dir(),
                    self.dir_util.rel_path(
                        file_path,
                        self.get_base_dir(),
                        self.get_other().dir_list.get_base_dir()
                    )[1:]
                ),
                'remove_file',
                ''
            )

    def remove_directory(self):
        rel_dir_path = self.dir_util.full_path(
            self.get_cur_dir(),
            self.get_chosen_dir()
        )

        dir_path = self.dir_util.full_path(
            self.get_base_dir(),
            rel_dir_path
        )

        other_rel_dir_path = self.dir_util.rel_path(
            rel_dir_path,
            self.get_base_dir(),
            self.get_other().dir_list.get_base_dir()
        )[1:]

        other_dir_path = self.dir_util.full_path(
            self.get_other().dir_list.get_base_dir(),
            other_rel_dir_path
        )

        code = self.dir_util.remove_directory(dir_path)
        if code == 1:
            self.raise_error_message(
                'Not found',
                'Directory {0} does not exist or have been removed'.format(
                    dir_path
                )
            )
        elif code == 2:
            self.raise_error_message(
                'Operation restricted',
                'Unfortunatelly, you do not have enough permissions to continue the action'
            )
        elif code == 3:
            self.raise_error_message(
                'Operation restricted',
                'Directory {0} is not empty'.format(dir_path)
            )

        self.list_directory()

        if self.get_chosen_dir() not in self.get_other().dir_list.get_dirs():
            return

        self.to_other_dirlist.emit(
            other_dir_path,
            'remove_directory',
            ''
        )

    def raise_error_message(self, title, text, callback=None):
        error_message = QtWidgets.QMessageBox(self)
        error_message.setWindowTitle(title)
        error_message.setText(text)
        error_message.setWindowModality(QtCore.Qt.WindowModal)
        error_message.exec_()

        self.list_directory()

        if callback:
            callback()

    def resolve_conflict(self, orig, copy):
        reply = QtWidgets.QMessageBox.question(
            self,
            "Conflict",
            "Replace {0} with {1}?".format(orig, copy),
            QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No,
            QtWidgets.QMessageBox.No
        )
        return reply == QtWidgets.QMessageBox.Yes

    @QtCore.pyqtSlot(str, str, str)
    def from_other_dirlist(self, entry, action, user_input):
        if not (self.state['cur_dir'] and self.state['record']):
            return

        if not ((self.state['left_to_right'] and self.state['id'] == 'left')
                or ((not self.state['left_to_right']) and self.state['id'] == 'right')):
            return

        if action == 'add_file':
            code = self.dir_util.add_file(user_input)
            if code == 1:
                self.raise_error_message(
                    'File exists',
                    'File {0} already exists!'.format(user_input)
                )
            elif code == 2:
                self.raise_error_message(
                    'Operation restricted',
                    'Unfortunatelly, you do not have enough permissions to continue the action'
                )

        if action == 'add_directory':
            code = self.dir_util.add_directory(user_input)
            self.set_cur_dir(self.dir_util.full_path(user_input, '..'))
            if code == 1:
                self.raise_error_message(
                    'File exists',
                    'File {0} already exists!'.format(user_input)
                )
            elif code == 2:
                self.raise_error_message(
                    'Operation restricted',
                    'Unfortunatelly, you do not have enough permissions to continue the action'
                )

        if action == 'remove_file':
            code = self.dir_util.remove_file(entry)
            if code == 1:
                self.raise_error_message(
                    'File not found',
                    'File {0} does not exist or has been removed'.format(entry)
                )
            elif code == 2:
                self.raise_error_message(
                    'Operation restricted',
                    'Unfortunatelly, you do not have enough permissions to continue the action'
                )

        if action == 'remove_directory':
            code = self.dir_util.remove_directory(entry)
            if code == 1:
                self.raise_error_message(
                    'Not found',
                    'Directory {0} does not exist or have been removed'.format(
                        entry
                    )
                )
            elif code == 2:
                self.raise_error_message(
                    'Operation restricted',
                    'Unfortunatelly, you do not have enough permissions to continue the action'
                )
            elif code == 3:
                self.raise_error_message(
                    'Operation restricted',
                    'Directory {0} is not empty'.format(entry)
                )

        if action == 'rename':
            code = self.dir_util.rename(entry, user_input)
            if code == 1:
                self.raise_error_message(
                    'Conflict',
                    'File {0} already exist!'.format(user_input)
                )
            elif code == 2:
                self.raise_error_message(
                    'Operation restricted',
                    'Unfortunatelly, you do not have enough permissions to continue the action'
                )
            elif code == 3:
                self.raise_error_message(
                    'Entry not found',
                    'Entry {0} does not exist or has been removed'.format(
                        entry)
                )

        self.list_directory()
