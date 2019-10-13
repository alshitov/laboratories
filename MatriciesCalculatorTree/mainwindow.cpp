#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <numericmatrix.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int M = 5, N = 6;
    NumericMatrix matrix(M, N);
    matrix.setValue(0, 0, 1.488);
    qDebug() << matrix(0, 0);
    qDebug() << matrix.getValue(0, 0);

    calculator = new MatricesCalculator();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addMatrixRowSpinBox_valueChanged(int arg1)
{

}

void MainWindow::on_addMatrixColSpinBox_valueChanged(int arg1)
{

}

void MainWindow::on_addMatrixPushButton_clicked()
{

}
