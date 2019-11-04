#ifndef MATRIXREPR_H
#define MATRIXREPR_H
#include <QtCore>
#include <QtWidgets>

class MatrixRepr : public QFrame
{
    Q_OBJECT
private:
    QVBoxLayout *frame_l;
    QLabel      *name_label;
    QHBoxLayout *body_l;
    QVBoxLayout *controls_l;
    QLabel      *matrix_repr;
    QPushButton *edit_b;
    QPushButton *remove_b;
    QString m_name;

public:
    MatrixRepr(QString& _m_name, std::string _m_data);

    ~MatrixRepr();
    void make_elements(QString& _m_name, std::string _m_data);
    void place_elements();

    void set_m_name(const QString& m);
    void set_m_data(const QString& data);

    QString std_to_q(std::string s);
    std::string q_to_std(QString& s);

    QPushButton* get_edit_b();
    QPushButton* get_remove_b();

    QString get_m_name();
};

#endif // MATRIXREPR_H
