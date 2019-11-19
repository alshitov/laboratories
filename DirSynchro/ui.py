# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'dirsynchro.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_DirSynchro(object):
    def setupUi(self, DirSynchro):
        DirSynchro.setObjectName("DirSynchro")
        DirSynchro.resize(800, 600)
        DirSynchro.setStyleSheet(
            "QPushButton{\n"
            "    height: 25px;\n"
            "    width: 70px;\n"
            "    background-color: white;\n"
            "    border-radius: 2px;\n"
            "    border: 1px solid #cccccc;\n"
            "}\n"
            "QPushButton:hover{\n"
            "    opacity: 0.8;\n"
            "}"
        )
        self.centralwidget = QtWidgets.QWidget(DirSynchro)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.mainLayout = QtWidgets.QHBoxLayout()
        self.mainLayout.setObjectName("mainLayout")
        self.rightDirProcLayout = QtWidgets.QVBoxLayout()
        self.rightDirProcLayout.setObjectName("rightDirProcLayout")
        self.mainLayout.addLayout(self.rightDirProcLayout)
        self.buttonsLayout = QtWidgets.QVBoxLayout()
        self.buttonsLayout.setSpacing(3)
        self.buttonsLayout.setObjectName("buttonsLayout")
        self.leftToRightButton = QtWidgets.QPushButton(self.centralwidget)
        self.leftToRightButton.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.leftToRightButton.setObjectName("leftToRightButton")
        self.buttonsLayout.addWidget(self.leftToRightButton)
        self.rightToLeftButton = QtWidgets.QPushButton(self.centralwidget)
        self.rightToLeftButton.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.rightToLeftButton.setObjectName("rightToLeftButton")
        self.buttonsLayout.addWidget(self.rightToLeftButton)
        self.recordButton = QtWidgets.QPushButton(self.centralwidget)
        self.recordButton.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.recordButton.setObjectName("recordButton")
        self.buttonsLayout.addWidget(self.recordButton)
        self.stopButton = QtWidgets.QPushButton(self.centralwidget)
        self.stopButton.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.stopButton.setObjectName("stopButton")
        self.buttonsLayout.addWidget(self.stopButton)
        self.textReplaceButton = QtWidgets.QPushButton(self.centralwidget)
        self.textReplaceButton.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.textReplaceButton.setObjectName("textReplaceButton")
        self.buttonsLayout.addWidget(self.textReplaceButton)
        self.mainLayout.addLayout(self.buttonsLayout)
        self.leftDirProcLayout = QtWidgets.QVBoxLayout()
        self.leftDirProcLayout.setObjectName("leftDirProcLayout")
        self.mainLayout.addLayout(self.leftDirProcLayout)
        self.horizontalLayout_2.addLayout(self.mainLayout)
        DirSynchro.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(DirSynchro)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 25))
        self.menubar.setObjectName("menubar")
        DirSynchro.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(DirSynchro)
        self.statusbar.setObjectName("statusbar")
        DirSynchro.setStatusBar(self.statusbar)

        self.retranslateUi(DirSynchro)
        QtCore.QMetaObject.connectSlotsByName(DirSynchro)

    def retranslateUi(self, DirSynchro):
        _translate = QtCore.QCoreApplication.translate
        DirSynchro.setWindowTitle(_translate("DirSynchro", "DirSynchro"))
        self.leftToRightButton.setToolTip(_translate("DirSynchro", "<html><head/><body><p>Change synchronization direction to &quot;left-to-right&quot;</p></body></html>"))
        self.leftToRightButton.setText(_translate("DirSynchro", ">>"))
        self.rightToLeftButton.setToolTip(_translate("DirSynchro", "<html><head/><body><p>Change synchronization direction to &quot;right-to-left&quot;</p></body></html>"))
        self.rightToLeftButton.setText(_translate("DirSynchro", "<<"))
        self.recordButton.setText(_translate("DirSynchro", "Record"))
        self.stopButton.setText(_translate("DirSynchro", "Stop"))
        self.textReplaceButton.setText(_translate("DirSynchro", "Text"))

