#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtWidgets>
#include <QtGui>

#include <numericmatrix.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    matr_calc = new MatricesCalculator();

    tree_scene = new TreeScene();
    tree_scene_view = ui->treeView;
    tree_scene_view->setScene(tree_scene->get_scene());
    tree_scene_view->setRenderHint(QPainter::Antialiasing);
    tree_scene_view->setBackgroundBrush(QBrush(QColor(19, 19, 19), Qt::SolidPattern));
    m_input_t = ui->addNewMatrixTable;
    set_up_matrix_input_table();

    connect(this, SIGNAL(w_resized(int, int)), tree_scene, SLOT(view_scaled(int, int)));
    connect(this, SIGNAL(m_added(NumericMatrix&)), matr_calc, SLOT(m_received(NumericMatrix&)));
    connect(matr_calc, SIGNAL(calc_done(MList&)), this, SLOT(calc_over(MList&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    int w = e->size().width();
    int h = e->size().height();
    emit w_resized(w, h);
}

void MainWindow::calc_over(MList& ms)
{
    // Get matrices from calculator and pass to treeView for rendering
    std::cout << "Received from calculator:" << &ms;
    std::cout << "will render now bra...";
}

void MainWindow::set_up_matrix_input_table()
{
    m_input_t->setRowCount(2);
    m_input_t->setColumnCount(2);

    for (int i = 0; i < 2; ++i)
    {
        m_input_t->setRowHeight(i, 95);
        m_input_t->setColumnWidth(i, 120);

        for (int j = 0; j < 2; ++j)
            set_table_cell_edit(i, j);
    }
}

void MainWindow::set_table_cell_edit(int i, int j)
{
    QLineEdit *edit = new QLineEdit();
    QDoubleValidator *d_validator = new QDoubleValidator();
    edit->setFrame(false);
    d_validator->setParent(edit);
    edit->setValidator(d_validator);
    edit->setAlignment(Qt::AlignCenter);
    edit->setText("0");
    m_input_t->setCellWidget(i, j, edit);
}

void MainWindow::on_addMatrixRowSpinBox_valueChanged(int arg1)
{
    m_input_t->setRowCount(arg1);
    for (int i = 0; i < arg1; ++i)
        m_input_t->setRowHeight(i, int(190 / arg1));

    for (int j = 0; j < m_input_t->columnCount(); ++j)
        set_table_cell_edit(arg1 - 1, j);
}

void MainWindow::on_addMatrixColSpinBox_valueChanged(int arg1)
{
    m_input_t->setColumnCount(arg1);
    for (int j = 0; j < arg1; ++j)
        m_input_t->setColumnWidth(j, int(240 / arg1));

    for (int i = 0; i < m_input_t->rowCount(); ++i)
        set_table_cell_edit(i, arg1 - 1);
}

void MainWindow::on_addMatrixPushButton_clicked()
{
    int rows = m_input_t->rowCount(),
        cols = m_input_t->columnCount();

    NumericMatrix *m = new NumericMatrix(rows, cols);
    QLineEdit *edit;

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < rows; ++j)
        {
            edit = qobject_cast<QLineEdit*>(m_input_t->cellWidget(i, j));
            m->set_v(i, j, edit->text().toDouble());
        }
    emit m_added(*m);
}
