#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <sessionsmanager.h>
#include <matricescalculator.h>
#include <tree.h>
#include <vector>

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
    MatricesCalculator *mc;
    SessionsManager *sm;
    Tree *t;

    std::vector<NumericMatrix*> *ms = new std::vector<NumericMatrix*>();

    void init_new_matrix(NumericMatrix &matrix);
    void add_new_matrix_to_existing(NumericMatrix &matrix);
    void load_previous_session();
};
#endif // MAINWINDOW_H
