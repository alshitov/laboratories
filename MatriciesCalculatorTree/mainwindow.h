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
#include <matrixrepr.h>
#include <treeview.h>
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

private slots:
    void on_addMatrixRowSpinBox_valueChanged(int arg1);
    void on_addMatrixColSpinBox_valueChanged(int arg1);
    void on_addMatrixPushButton_clicked();

    void on_saveMatrixButton_clicked();

private:
    Ui::MainWindow *ui;
    TreeScene *tree_scene;
    TreeView *tree_view;

    QTableWidget *m_input_t;
    MatricesCalculator *matr_calc;
    SessionsManager *sess_m;
    NumericMatrix *active_m_p;
    QListWidgetItem *active_item_p;
    MatrixRepr *active_repr_p;

    int matrix_counter = 0;

    void set_up_matrix_input_table(int rows, int cols);
    void set_table_cell_edit(int i, int j);
    void init_new_matrix(NumericMatrix &m);
    void add_new_matrix_to_existing(NumericMatrix &m);
    void set_active_m(NumericMatrix* m);
    void edit_m(NumericMatrix&);
    void prepare_m_edit(NumericMatrix& m);
    void remove_m(int index);
    void render_result();
    void load_previous_session();

protected:
    virtual void resizeEvent(QResizeEvent *e);

};
#endif // MAINWINDOW_H
