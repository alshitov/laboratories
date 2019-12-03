import model_ui
from PyQt5 import QtWidgets, QtCore, QtGui
import src
import random


class Model(QtWidgets.QMainWindow, model_ui.Ui_Model):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.from_fields = True
        self.toggle_method()
        self.terminal = src.Terminal()
        self.connect_signals()

    def connect_signals(self):
        self.methodRadioButton.toggled.connect(self.toggle_method)
        self.transactionDataTable.cellChanged.connect(self.print_data)

        self.testTransactionButton.clicked.connect(
            lambda: self.make_transaction('test'))
        self.saleTransactionButton.clicked.connect(
            lambda: self.make_transaction('sale'))
        self.refundTransactionButton.clicked.connect(
            lambda: self.make_transaction('refund'))
        self.balanceTransactionButton.clicked.connect(
            lambda: self.make_transaction('balance'))
        self.settlementTransactionButton.clicked.connect(
            lambda: self.make_transaction('settlement'))

    def print_data(self):
        self.tableDataLabel.setText(self.table_values())

    def list_log(self):
        try:
            log = src.ClientLogger.get(self.terminal.id)
            if log:
                for entry in log.split('\n'):
                    self.terminalLogList.addItem(entry)
        except FileNotFoundError:
            pass

    def toggle_method(self):
        active = not self.from_fields
        self.transactionDataTable.setEnabled(not active)
        self.from_fields = active
        self.startSignComboBox.setEnabled(active)
        self.endSignComboBox.setEnabled(active)
        self.protocolVersionComboBox.setEnabled(active)
        self.panLineEdit.setEnabled(active)
        self.carhholderNameEdit.setEnabled(active)
        self.pinLineEdit.setEnabled(active)
        self.expDateEdit.setEnabled(active)
        self.textDataPlainTextEdit.setEnabled(active)

    def table_values(self):
        data = ''
        for i in range(self.transactionDataTable.rowCount()):
            for j in range(self.transactionDataTable.columnCount()):
                cell = self.transactionDataTable.item(i, j)
                if cell is not None and cell.text() != '':
                    text = cell.text()
                    char = chr(int(text, 16))
                    data += char
        return data

    def make_transaction(self, action):
        # 2 ways of forming transaction data: ...
        if self.from_fields:
            # ... 1) from fields
            data = {
                'start_sign': str(self.startSignComboBox.currentText()),
                'protocol_version': str(self.protocolVersionComboBox.currentText()),
                'end_sign': str(self.endSignComboBox.currentText())
            }

            expiry_date = self.expDateEdit.date()
            exp_date_year = str(self.expDateEdit.date().year())[2:]
            exp_date_month = str(self.expDateEdit.date().month())
            if len(exp_date_month) == 1:
                exp_date_month = '0' + exp_date_month

            if action == 'test':
                data.update({
                    'bitmap': '0010',
                })
            elif action == 'sale':
                amount = str(random.randint(1000, 1000000))
                amount = '0' * (12 - len(amount)) + amount
                data.update({
                    'PAN': str(self.panLineEdit.text()),
                    'cardholder_name': str(self.carhholderNameEdit.text()),
                    'PIN': str(self.pinLineEdit.text()),
                    'expiry_date': exp_date_year + exp_date_month,
                    'amount': amount
                })
            elif action == 'balance':
                data.update({
                    'PAN': str(self.panLineEdit.text()),
                    'cardholder_name': str(self.carhholderNameEdit.text()),
                    'PIN': str(self.pinLineEdit.text()),
                    'expiry_date': exp_date_year + exp_date_month,
                })
            elif action == 'refund':
                amount = str(random.randint(1000, 1000000))
                amount = '0' * (12 - len(amount)) + amount
                data.update({
                    'PAN': str(self.panLineEdit.text()),
                    'cardholder_name': str(self.carhholderNameEdit.text()),
                    'PIN': str(self.pinLineEdit.text()),
                    'expiry_date': exp_date_year + exp_date_month,
                    'amount': amount
                })
            elif action == 'settlement':
                data.update({
                    'text_data': str(self.textDataPlainTextEdit.toPlainText()),
                })
            response = self.terminal.make_transaction(action, data)
        else:
            # ... 2) from data table
            data = self.table_values()
            response = self.terminal.make_transaction(action, data, from_table=True)

        self.list_log()
