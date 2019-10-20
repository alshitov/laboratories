#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>

#include <sessionsmanager.h>
#include <matricescalculator.h>
#include <treescene.h>
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

signals:
    void w_resized(int, int);
    void m_added(NumericMatrix&);

public slots:
    void calc_over(MList&);

private slots:
    void on_addMatrixRowSpinBox_valueChanged(int arg1);
    void on_addMatrixColSpinBox_valueChanged(int arg1);
    void on_addMatrixPushButton_clicked();

private:
    Ui::MainWindow *ui;
    TreeScene *tree_scene;
    QGraphicsView *tree_scene_view;

    QTableWidget *m_input_t;
    QGraphicsEllipseItem *ellipse;
    MatricesCalculator *matr_calc;
    SessionsManager *sess_m;

    void set_up_matrix_input_table();
    void set_table_cell_edit(int i, int j);
    void init_new_matrix(NumericMatrix &m);
    void add_new_matrix_to_existing(NumericMatrix &m);
    void load_previous_session();

protected:
    virtual void resizeEvent(QResizeEvent *e);

};
#endif // MAINWINDOW_H
