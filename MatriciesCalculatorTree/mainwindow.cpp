#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    matr_calc = new MatricesCalculator();
    tree_scene = new TreeScene();
    tree_scene->setParent(this);
    tree_view = new TreeView(this, tree_scene);
    ui->centralLayout->addWidget(tree_view);
    m_input_t = ui->addNewMatrixTable;
    set_up_matrix_input_table(2, 2);
    connect(this, SIGNAL(w_resized(int, int)), tree_scene, SLOT(view_scaled(int, int)));
    connect(tree_scene, SIGNAL(show_result(int)), this, SLOT(show_result(int)));
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

void MainWindow::show_result(int action_id)
{
    qDebug() << "From scene signal; received: " << action_id;
}

void MainWindow::set_up_matrix_input_table(int rows, int cols)
{
    m_input_t->setRowCount(rows);
    m_input_t->setColumnCount(cols);

    for (int i = 0; i < rows; ++i)
    {
        m_input_t->setRowHeight(i, int(190 / rows));

        for (int j = 0; j < cols; ++j)
        {
            m_input_t->setColumnWidth(j, int(240 / cols));
            set_table_cell_edit(i, j);
        }
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
    set_up_matrix_input_table(arg1, m_input_t->columnCount());
}

void MainWindow::on_addMatrixColSpinBox_valueChanged(int arg1)
{
    set_up_matrix_input_table(m_input_t->rowCount(), arg1);
}

void MainWindow::on_addMatrixPushButton_clicked()
{
    int rows = m_input_t->rowCount(),
        cols = m_input_t->columnCount();

    NumericMatrix *m = new NumericMatrix(rows, cols);
    QString m_name = matrix_name();
    QLineEdit *edit;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            edit = qobject_cast<QLineEdit*>(
                m_input_t->cellWidget(i, j)
            );
            m->set_v(i, j, edit->text().toDouble());
        }
    }

    this->init_new_matrix(*m, m_name);
    matr_calc->add_m(*m, m_name);
    render_result();
}

void MainWindow::on_saveMatrixButton_clicked()
{
    int rows = m_input_t->rowCount(),
        cols = m_input_t->columnCount();

    NumericMatrix *nm = new NumericMatrix(rows, cols);
    QLineEdit *edit;
    for(int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {

            edit = qobject_cast<QLineEdit*>(
                m_input_t->cellWidget(i, j)
            );
            nm->set_v(i, j, edit->text().toDouble());
        }
    }

    active_repr_p->set_m_name(ui->matrixNameLineEdit->text());
    active_repr_p->set_m_data(QString::fromUtf8(nm->get_str_data().c_str()));
    matr_calc->edit_m(*active_m_p, *nm);
    set_up_matrix_input_table(2, 2);
    ui->matrixNameLineEdit->setText("");
    ui->saveMatrixButton->setEnabled(false);
    ui->addMatrixPushButton->setEnabled(true);
}

QString MainWindow::matrix_name()
{
    int i = 0;
    bool found = false;
    QString name;

    while (!found)
    {
        if (i < 26)
            name = alphabet[i++];
        else
            name = alphabet[i] + QString::number(i);

        if (!taken_names->contains(name))
        {
            taken_names->push_back(name);
            found = true;
        }
    }
    return name;
}

void MainWindow::init_new_matrix(NumericMatrix& m, QString name)
{
    QListWidgetItem *matrix_item = new QListWidgetItem();
    matrix_item->setSizeHint(QSize(0, 60 + (30 * m.get_r())));
    ui->existingMatricesView->addItem(matrix_item);

    QString text = ui->matrixNameLineEdit->text();
    repr_counter++;
    MatrixRepr *m_repr = new MatrixRepr(name, m.get_str_data());

    ui->existingMatricesView->setItemWidget(matrix_item, m_repr);

    connect(m_repr->get_edit_b(), &QPushButton::clicked,
        [this, &m, m_repr, matrix_item] {
                // Prepare table
                int rows = m.get_r(),
                    cols = m.get_c();

                set_up_matrix_input_table(rows, cols);

                QLineEdit *edit;
                for (int i = 0; i < rows; ++i)
                {
                    for (int j = 0; j < cols; ++j)
                    {
                        edit = qobject_cast<QLineEdit*>(
                            m_input_t->cellWidget(i, j)
                        );
                        edit->setText(QString(QString::number(m.get_v(i, j))));
                    }
                }

                // Indicate matrix name
                ui->matrixNameLineEdit->setText(m_repr->get_m_name());

                // Set active objects to work with
                this->active_m_p = &m;
                this->active_item_p = matrix_item;
                this->active_repr_p = m_repr;

                // Switch buttons activity
                ui->addMatrixPushButton->setEnabled(false);
                ui->saveMatrixButton->setEnabled(true);
            }
    );
    connect(m_repr->get_remove_b(), &QPushButton::clicked,
        [this, &m, matrix_item] {
                ui->existingMatricesView->takeItem(
                    ui->existingMatricesView->row(matrix_item)
                );
                matr_calc->remove_m(m);
                repr_counter--;
                render_result();
                ui->saveMatrixButton->click();
            }
    );
}

void MainWindow::render_result()
{
    tree_scene->set_msinfo(matr_calc->get_msinfo());
    tree_scene->draw();
}
