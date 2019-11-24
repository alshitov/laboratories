#ifndef MODEL_H
#define MODEL_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Model; }
QT_END_NAMESPACE

class Model : public QMainWindow
{
    Q_OBJECT

public:
    Model(QWidget *parent = nullptr);
    ~Model();

private:
    Ui::Model *ui;
};
#endif // MODEL_H
