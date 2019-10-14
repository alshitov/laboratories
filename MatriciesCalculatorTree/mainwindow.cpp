#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <numericmatrix.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int M = 3, N = 5;
    NumericMatrix m(M, N);
    m.set_v(0, 0, -1);
    m.set_v(0, 1, 2);
    m.set_v(0, 2, 4);
    m.set_v(0, 3, 0);
    m.set_v(0, 4, 7);
    m.set_v(1, 0, 3);
    m.set_v(1, 1, -5);
    m.set_v(1, 2, 24);
    m.set_v(1, 3, 9);
    m.set_v(1, 4, -3);
    m.set_v(2, 0, -10);
    m.set_v(2, 1, -8);
    m.set_v(2, 2, -2);
    m.set_v(2, 3, -4);
    m.set_v(2, 4, 11);
    m.show();
    m.remove_c(4);
    m.show();

//    mc = new MatricesCalculator();
//    sm = new SessionsManager();
//    t = new Tree();
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
    // Collect values from table
}
