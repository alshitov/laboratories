#include "matrixrepr.h"

MatrixRepr::MatrixRepr(QString& _m_name, std::string _m_data)
    : m_name(_m_name)
{
    make_elements(_m_name, _m_data);
    place_elements();
}

void MatrixRepr::make_elements(QString& _m_name, std::string _m_data)
{
    frame_l = new QVBoxLayout();
    name_label = new QLabel(_m_name);
    body_l = new QHBoxLayout();
    controls_l = new QVBoxLayout();
    matrix_repr = new QLabel(std_to_q(_m_data));
    edit_b = new QPushButton("|");
    remove_b = new QPushButton("X");

    edit_b->setToolTip("Edit");
    edit_b->setFixedSize(30, 30);
    edit_b->setCursor(Qt::PointingHandCursor);
    remove_b->setToolTip("Remove");
    remove_b->setFixedSize(30, 30);
    remove_b->setCursor(Qt::PointingHandCursor);
}

void MatrixRepr::place_elements()
{
    frame_l->addWidget(name_label);
    frame_l->addLayout(body_l);
    body_l->addWidget(matrix_repr);

    controls_l->setAlignment(Qt::AlignRight);
    controls_l->addWidget(edit_b);
    controls_l->addWidget(remove_b);
    body_l->addLayout(controls_l);

    setFrameShape(QFrame::Box);
    setLayout(frame_l);
}

void MatrixRepr::set_m_data(const QString &data)
{
    matrix_repr->setText(data);
}

QString MatrixRepr::std_to_q(std::string s)
{
    return QString::fromUtf8(s.c_str());
}

std::string MatrixRepr::q_to_std(QString& s)
{
    return s.toUtf8().constData();
}

QPushButton* MatrixRepr::get_edit_b()
{
    return edit_b;
}

QPushButton* MatrixRepr::get_remove_b()
{
    return remove_b;
}

void MatrixRepr::set_m_name(const QString &m)
{
    name_label->setText(m);
}

QString MatrixRepr::get_m_name()
{
    return m_name;
}
