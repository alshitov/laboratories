#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
from PyQt5 import QtWidgets
import dirsynchro


def main():
    app = QtWidgets.QApplication(sys.argv)
    window = dirsynchro.DirSynchro()
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()

