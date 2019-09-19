#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dataloader.h>
#include <dataprocessor.h>
#include <QComboBox>

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
    // Avia Tab slots
    void on_aviaFromComboBox_currentIndexChanged(const QString &arg1);

    void on_aviaToComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    DataLoader dataLoader;
    DataProcessor dataProcessor;

    void fillAviaTabControls();

    QModelIndex chosenCity;
    void disableChoice(QComboBox *combo, QModelIndex *index);
    void enableChoice(QComboBox *combo, QModelIndex index);
};
#endif // MAINWINDOW_H
