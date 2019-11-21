from PyQt5 import QtWidgets, QtCore, QtGui
import dirlist


class DirProc(QtWidgets.QFrame):

    def __init__(self, id_):
        super().__init__()

        self.layout = QtWidgets.QVBoxLayout()
        self.dir_choose_layout = QtWidgets.QHBoxLayout()
        self.dir_label = QtWidgets.QLabel('Choose directory')
        self.dir_choose_button = QtWidgets.QPushButton("...")
        self.dir_list = dirlist.DirList(id_)
        self.cur_dir = '/home/alexander'

        self.make_ui()
        self.connect_signals()
        self.connect_slots()

    def resizeEvent(self, event):
        width = event.size().width()
        self.dir_label.setFixedWidth(width * 0.9)

    def make_ui(self):
        self.layout.setAlignment(QtCore.Qt.AlignTop)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.dir_choose_layout.addWidget(self.dir_label)
        self.dir_choose_layout.addWidget(self.dir_choose_button)
        self.dir_choose_button.setFixedWidth(25)
        self.dir_choose_button.setCursor(QtCore.Qt.PointingHandCursor)
        self.layout.addLayout(self.dir_choose_layout)
        self.layout.addWidget(self.dir_list)
        self.setLayout(self.layout)

    def connect_signals(self):
        self.dir_choose_button.clicked.connect(self.on_dir_button_click)

    def connect_slots(self):
        pass

    def on_dir_button_click(self):
        dirname = QtWidgets.QFileDialog.getExistingDirectoryUrl(
            self, "Choose directory", QtCore.QUrl("/home/alexander"))
        abs_path = QtCore.QUrl.toString(dirname)

        if abs_path:
            self.cur_dir = abs_path
            formatted = abs_path.split("file://")[1]
            self.dir_label.setText(self.pretty_label(formatted))
            self.dir_list.set_dir(formatted)

    @staticmethod
    def pretty_label(string):
        pieces = string.split('/')
        max_ = max(pieces, key=len)
        idx = pieces.index(max_)
        pieces[idx] = max_[0: 2] + "..." + max_[-3: -1]
        return '/'.join(pieces)
