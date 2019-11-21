from PyQt5 import QtWidgets, QtCore, QtGui
import os
import subprocess
import stat
from stat import \
    S_IRUSR, S_IWUSR, S_IXUSR, \
    S_IRGRP, S_IWGRP, S_IXGRP, \
    S_IROTH, S_IWOTH, S_IXOTH


class DirList(QtWidgets.QListWidget):

    to_other_dirlist = QtCore.pyqtSignal(str, str, str)

    def __init__(self, id_):
        super().__init__()

        self.state = {
            'id': id_,
            # From main windoww
            'left_to_right': True,
            'record': True,
            # Self
            'other': None,
            'directory': '/Users/ympo2oo2/Documents/from' if id_ == 'left' else '/Users/ympo2oo2/Documents/to',
            'files': [],
            'directories': [],
            'chosen_file': '',
            'chosen_directory': ''
        }
        self.itemClicked.connect(self.select_item)
        self.itemDoubleClicked.connect(self.go_to)

        self.list_directory()

    def pull_main_window_state(self, state):
        self.state.update(state)

    def set_other(self, dir_proc):
        self.state['other'] = dir_proc

    def get_dir(self):
        return self.state['directory']

    def set_dir(self, dir_):
        self.state['directory'] = dir_
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
                    self.get_access_rights(self.full_path(entry), is_dir)
                )
            )

        items.extend(['./', '../'])
        self.addItems(sorted(items))

    def get_access_rights(self, entry, is_dir):
        mode = os.stat(entry).st_mode
        return ('d' if is_dir else '') + ''.join([
            'r' if bool(mode & S_IRUSR) else '-',
            'w' if bool(mode & S_IWUSR) else '-',
            'x' if bool(mode & S_IXUSR) else '-',
            'r' if bool(mode & S_IRGRP) else '-',
            'w' if bool(mode & S_IWGRP) else '-',
            'x' if bool(mode & S_IXGRP) else '-',
            'r' if bool(mode & S_IROTH) else '-',
            'w' if bool(mode & S_IWOTH) else '-',
            'x' if bool(mode & S_IXOTH) else '-'
        ])

    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            # Fires before itemClicked so click on free space will reset chosen files
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
        item_name = item.text().split('\t')[0].replace('/', '')

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
            res = subprocess.call([
                'open',
                self.full_path_from_dir(
                    self.state['directory'],
                    self.state['chosen_file']
                )
            ])
            if res != 0:
                self.raise_error_message(
                    'File not found!',
                    'File {0} has been removed'.format(
                        self.state['chosen_file'])
                )

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
            self,
            'New entry',
            'Entry name:'
        )
        if ok:
            if name in self.state['files']:
                self.raise_error_message(
                    'Conflict!',
                    'File: "{0}" already exists!'.format(name),
                    self.add_file
                )
            else:
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
            self,
            'New entry',
            'Entry name:'
        )
        if ok:
            if name in self.state['directories']:
                self.raise_error_message(
                    'Conflict!',
                    'Directory: "{0}" already exists!'.format(name),
                    self.add_directory
                )
            else:
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
        try:
            os.remove(self.full_path(self.state['chosen_file']))
        except FileNotFoundError:
            self.raise_error_message(
                'File not found!',
                'File may have been removed outside application'
            )

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
            # First check if new name is not in existing files
            if name in self.state['files']:
                self.raise_error_message(
                    'Conflict!',
                    'File: "{0}" already exists!'.format(name),
                    self.add_file
                )
            else:
                # Second check whether file still exists
                try:
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
                except FileNotFoundError:
                    self.raise_error_message(
                        'File not found!',
                        'File may have been removed outside application'
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

    def raise_error_message(self, title, text, callback=None):
        error_message = QtWidgets.QMessageBox(self)
        error_message.setWindowTitle(title)
        error_message.setText(text)
        error_message.setWindowModality(QtCore.Qt.WindowModal)
        error_message.exec_()

        self.list_directory()

        if callback:
            callback()

    def resolve_conflict(self):
        reply = QtWidgets.QMessageBox.question(
            self,
            "Conflict",
            "Replace {0} with {1}?",
            QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No,
            QtWidgets.QMessageBox.No
        ).exec()

        return reply == QtWidgets.QMessageBox.Yes

    def full_path(self, file):
        return os.path.join(self.state['directory'], file)

    def full_path_from_dir(self, dir_, file):
        return os.path.join(dir_, file)

    @QtCore.pyqtSlot(str, str, str)
    def from_other_dirlist(self, entry, action, user_input):
        # If directory is set and record in on
        if self.state['directory'] and self.state['record']:

            if (self.state['left_to_right'] and self.state['id'] == 'left') \
                    or ((not self.state['left_to_right']) and self.state['id'] == 'right'):

                if action == 'add_file':
                    open(self.full_path(user_input), 'a').close()

                if action == 'add_directory':
                    os.mkdir(self.full_path(user_input))

                if action == 'remove_file':
                    os.remove(entry)

                if action == 'remove_directory':
                    os.rmdir(entry)

                if action in ['rename_file', 'move_file',
                              'rename_directory', 'move_directory']:
                    os.rename(entry, user_input)

            else:
                print('Skip action due to settings...')

        self.list_directory()
