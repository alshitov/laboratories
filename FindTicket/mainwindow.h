#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fillUIControls();

private slots:
    // Avia Tab slots
    // Params choice pane
    void on_aviaFromComboBox_activated(const QString &arg1);
    void on_aviaFromComboBox_editTextChanged(const QString &arg1);
    void on_aviaToComboBox_activated(const QString &arg1);
    void on_aviaToComboBox_editTextChanged(const QString &arg1);
    void on_aviaDepartComboBox_userDateChanged(const QDate &date);
    void on_aviaArrivalComboBox_userDateChanged(const QDate &date);
    void on_aviaAdultSpinBox_valueChanged(int arg1);
    void on_aviaChildrenSpinBox_valueChanged(int arg1);
    void on_aviaBabySpinBox_valueChanged(int arg1);
    void on_aviaTypeComboBox_activated(const QString &arg1);
    void on_aviaTypeComboBox_editTextChanged(const QString &arg1);
    // Find Button
    void on_aviaSearchPushButton_clicked();

    // Railway Tab slots
    // Params choice pane
    void on_railwayFromComboBox_activated(const QString &arg1);
    void on_railwayFromComboBox_editTextChanged(const QString &arg1);
    void on_railwayToComboBox_activated(const QString &arg1);
    void on_railwayToComboBox_editTextChanged(const QString &arg1);
    void on_railwayDepartComboBox_userDateChanged(const QDate &date);
    void on_railwayArrivalComboBox_userDateChanged(const QDate &date);
    // Find Button
    void on_railwaySearchPushButton_clicked();

    // Bus Tab clots
    // Params choice pane
    void on_busFromComboBox_activated(const QString &arg1);
    void on_busFromComboBox_editTextChanged(const QString &arg1);
    void on_busToComboBox_activated(const QString &arg1);
    void on_busToComboBox_editTextChanged(const QString &arg1);
    void on_busDepartComboBox_userDateChanged(const QDate &date);
    void on_busPassengersSpinBox_valueChanged(int arg1);
    // Find Button
    void on_busSearchPushButton_clicked();

    // Train Tab slots
    // Params choice pane
    void on_trainFromComboBox_activated(const QString &arg1);
    void on_trainFromComboBox_editTextChanged(const QString &arg1);
    void on_trainToComboBox_activated(const QString &arg1);
    void on_trainToComboBox_editTextChanged(const QString &arg1);
    void on_trainDepartComboBox_userDateChanged(const QDate &date);
    // Show Schedule Button
    void on_trainSearchPushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
