#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <numericmatrix.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int M = 3, N = 3;
    NumericMatrix m(M, N);
    m.set_v(0, 0, -1);
    m.set_v(0, 1, 2);
    m.set_v(0, 2, 4);

    m.set_v(1, 0, 3);
    m.set_v(1, 1, -5);
    m.set_v(1, 2, 24);

    m.set_v(2, 0, -10);
    m.set_v(2, 1, -8);
    m.set_v(2, 2, -2);
    m.show();
    qreal D = NumericMatrix::determinant(m);
    std::cout << "D[m] = " << D << std::endl;

    int M1 = 4, N1 = 4;
    NumericMatrix m1(M1, N1);
    m1.set_v(0, 0, -1);
    m1.set_v(0, 1, 2);
    m1.set_v(0, 2, 4);
    m1.set_v(0, 3, 15);

    m1.set_v(1, 0, 3);
    m1.set_v(1, 1, -5);
    m1.set_v(1, 2, 24);
    m1.set_v(1, 3, -1);

    m1.set_v(2, 0, -10);
    m1.set_v(2, 1, -8);
    m1.set_v(2, 2, -2);
    m1.set_v(2, 3, 5);

    m1.set_v(3, 0, 24);
    m1.set_v(3, 1, -10);
    m1.set_v(3, 2, -8);
    m1.set_v(3, 3, -2);
    m1.show();
    qreal D1 = NumericMatrix::determinant(m1);
    std::cout << "D[m1] = " << D1 << std::endl;

    int M2 = 5, N2 = 5;
    NumericMatrix m2(M2, N2);
    m2.set_v(0, 0, -1);
    m2.set_v(0, 1, 2);
    m2.set_v(0, 2, 4);
    m2.set_v(0, 3, 15);
    m2.set_v(0, 4, 15);

    m2.set_v(1, 0, 3);
    m2.set_v(1, 1, -5);
    m2.set_v(1, 2, 24);
    m2.set_v(1, 3, -1);
    m2.set_v(1, 4, -11);

    m2.set_v(2, 0, -10);
    m2.set_v(2, 1, -8);
    m2.set_v(2, 2, -2);
    m2.set_v(2, 3, 5);
    m2.set_v(2, 4, 0);

    m2.set_v(3, 0, 24);
    m2.set_v(3, 1, -10);
    m2.set_v(3, 2, -8);
    m2.set_v(3, 3, -2);
    m2.set_v(3, 4, -20);

    m2.set_v(4, 0, 5);
    m2.set_v(4, 1, 0);
    m2.set_v(4, 2, 5);
    m2.set_v(4, 3, 9);
    m2.set_v(4, 4, -10);
    m2.show();

    qreal D2 = NumericMatrix::determinant(m2);
    std::cout << "D[m2] = " << D2 << std::endl;

    int M3 = 6, N3 = 6;
    NumericMatrix m3(M3, N3);
    m3.set_v(0, 0, -1);
    m3.set_v(0, 1, 2);
    m3.set_v(0, 2, 4);
    m3.set_v(0, 3, -8);
    m3.set_v(0, 4, 10);
    m3.set_v(0, 5, -2);

    m3.set_v(1, 0, 3);
    m3.set_v(1, 1, -5);
    m3.set_v(1, 2, 24);
    m3.set_v(1, 3, -1);
    m3.set_v(1, 4, -11);
    m3.set_v(1, 5, 11);

    m3.set_v(2, 0, -10);
    m3.set_v(2, 1, -8);
    m3.set_v(2, 2, -2);
    m3.set_v(2, 3, 5);
    m3.set_v(2, 4, 0);
    m3.set_v(2, 5, 2);

    m3.set_v(3, 0, 24);
    m3.set_v(3, 1, -10);
    m3.set_v(3, 2, -8);
    m3.set_v(3, 3, -2);
    m3.set_v(3, 4, -20);
    m3.set_v(3, 5, -56);

    m3.set_v(4, 0, 5);
    m3.set_v(4, 1, 0);
    m3.set_v(4, 2, 5);
    m3.set_v(4, 3, 9);
    m3.set_v(4, 4, -10);
    m3.set_v(4, 5, 14);

    m3.set_v(5, 0, 5);
    m3.set_v(5, 1, 0);
    m3.set_v(5, 2, 5);
    m3.set_v(5, 3, 9);
    m3.set_v(5, 4, -10);
    m3.set_v(5, 5, 1);
    m3.show();

    qreal D3 = NumericMatrix::determinant(m3);
    std::cout << "D[m3] = " << D3 << std::endl;

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
