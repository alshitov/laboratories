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
    qDebug() << matrix.get_size();

    matrix.show();

    matrix.set_r_count(2);
    qDebug() << matrix.get_size();
    matrix.show();

    matrix.set_r_count(5);

    matrix.set_v(1, 0, 7);
    matrix.set_v(3, 4, 24);
    matrix.set_v(0, 2, 124);
    matrix.set_v(2, 4, 225);
    matrix.set_v(4, 0, 225);

    qDebug() << matrix.get_size();
    matrix.show();

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
