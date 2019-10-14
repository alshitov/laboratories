#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <matricescalculator.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addMatrixRowSpinBox_valueChanged(int arg1);
    void on_addMatrixColSpinBox_valueChanged(int arg1);
    void on_addMatrixPushButton_clicked();

private:
    Ui::MainWindow *ui;
    MatricesCalculator *calculator;
    NumericMatrix **int_Matrices = new NumericMatrix*();

    void addMatrix(NumericMatrix &matrix);
    void loadPreviousCalculations();
    void buildResultsTable();
};
#endif // MAINWINDOW_H
