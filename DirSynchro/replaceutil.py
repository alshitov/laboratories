import os
import re
from PyQt5 import QtWidgets, QtCore, QtGui
import replace_ui


class ReplaceUtil(QtWidgets.QDialog, replace_ui.Ui_Frame):

    def __init__(self):
        super().__init__()

        self.setupUi(self)
        self.setWindowModality(QtCore.Qt.WindowModal)

        self.state = {
            'mask': r'',
            'text_to_replace': r'',
            'text_to_insert': r'',
            'directory': '/home/alexander',
            'files': [],
            'chosen_files': []
        }

        self.null_bar()
        self.connect_signals()
        self.configure_table()

        self.maskComboBox.addItems([
            r'\w*.cpp',
            r'\d*.cpp'
        ])

        self.exec()

    def connect_signals(self):
        self.chooseDirButton.clicked.connect(self.choose_directory)
        self.replaceButton.clicked.connect(self.replace)
        self.findButton.clicked.connect(self.search)
        self.quitButton.clicked.connect(self.close)

    def configure_table(self):
        header = self.filesTable.horizontalHeader()
        header.setSectionResizeMode(0, QtWidgets.QHeaderView.ResizeToContents)
        header.setSectionResizeMode(1, QtWidgets.QHeaderView.Stretch)
        header.setSectionResizeMode(2, QtWidgets.QHeaderView.ResizeToContents)

    def choose_directory(self):
        dirname = QtWidgets.QFileDialog.getExistingDirectoryUrl(
            self, "Choose directory", QtCore.QUrl("/home/alexander"))
        abs_path = QtCore.QUrl.toString(dirname)

        if abs_path:
            path = abs_path.split("file://")[1]
            self.state['directory'] = path
            self.chooseDirComboBox.setCurrentText(path)
            self.chooseDirComboBox.addItem(path)

    def get_settings(self):
        self.state.update({
            'mask': self.maskComboBox.currentText(),
            'text_to_replace': self.textComboBox.currentText(),
            'directory': self.chooseDirComboBox.currentText(),
            'text_to_insert': self.replaceLineEdit.text(),
            'chosen_files': self.get_selected_files()
        })
        print(self.state)

    def get_selected_files(self):
        selected = []

        for i in range(self.filesTable.rowCount()):
            if self.filesTable.cellWidget(i, 0).layout().itemAt(0).widget().isChecked():
                selected.append(self.filesTable.item(i, 1).text())

        return selected

    def search(self):
        self.get_settings()

        for file in os.listdir(self.state['directory']):
            file_path = os.path.join(self.state['directory'], file)

            if os.path.isfile(file_path) and re.fullmatch(self.state['mask'], file):
                self.state['files'].append({
                    'name': file,
                    'size': '%s KB' % format(round(os.path.getsize(file_path) / 1024, 3), '.3f')
                })

        self.list_found()

    def list_found(self):
        self.filesTable.setRowCount(len(self.state['files']))

        for index, file in enumerate(self.state['files']):
            widget = QtWidgets.QWidget()
            radio = QtWidgets.QCheckBox()
            layout = QtWidgets.QHBoxLayout()
            layout.setAlignment(QtCore.Qt.AlignCenter)
            layout.setContentsMargins(0, 0, 0, 0)
            layout.addWidget(radio)
            widget.setLayout(layout)
            self.filesTable.setCellWidget(
                index, 0, widget)
            self.filesTable.setItem(
                index, 1, QtWidgets.QTableWidgetItem(file['name']))
            self.filesTable.setItem(
                index, 2, QtWidgets.QTableWidgetItem(file['size']))

    def null_bar(self):
        self.replaceProgressBar.setValue(0)

    def replace(self):
        self.get_settings()
        self.null_bar()

        chunk = int(100 / len(self.state['chosen_files']))

        for index, file in enumerate(self.state['chosen_files']):

            file_path = os.path.join(self.state['directory'], file).replace(' ', r'\ ').replace(r'\\', '\\')
            print(file_path)

            with open(file_path, 'w+') as f:
                text = f.read()
                print(text)
            #     f.write(
            #         text.replace(
            #             self.state['text_to_replace'],
            #             self.state['text_to_insert']
            #         )
            #     )

            if index == len(self.state['chosen_files']) - 1:
                self.replaceProgressBar.setValue(100)
            else:
                self.replaceProgressBar.setValue(chunk * (index + 1))
