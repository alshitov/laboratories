#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
from PyQt5 import QtWidgets
import iso8583


def main():
    app = QtWidgets.QApplication(sys.argv)
    window = iso8583.Model()
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()
