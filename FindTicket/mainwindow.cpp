#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <dataloader.h>
#include <dataprocessor.h>
#include <aviatabdelegate.h>
#include <railwaytabdelegate.h>
#include <bustabdelegate.h>
#include <traintabdelegate.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // MainWindow constructor.
    ui->setupUi(this);

    // Instantiate data processing classes
    DataLoader dataLoader = DataLoader();
    dataLoader.loadCitiesList();
    // Fill only aviaTab controls because it loads first,
    // other tabs controls are filled when tabSwitch signal triggered
    MainWindow::fillAviaTabControls(dataLoader.cities);

    DataProcessor dataProcessor = DataProcessor();

    // Instantiate delegates
    AviaTabDelegate aviaTabDelegate = AviaTabDelegate();
    RailwayTabDelegate railwayTabDelegate = RailwayTabDelegate();
    BusTabDelegate busTabDelegate = BusTabDelegate();
    TrainTabDelegate trainTabDelegate = TrainTabDelegate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillAviaTabControls(QStringList & citiesList)
{
    /* Method fills 'from' and 'to' comboboxes of aviaTab with data loaded by DataLoader */
    ui->aviaFromComboBox->addItems(citiesList);
    ui->aviaToComboBox->addItems(citiesList);
}

/* The next methods pass incoming events processing to corresponging delegates */

void MainWindow::on_aviaFromComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_aviaFromComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_aviaToComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_aviaToComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_aviaDepartComboBox_userDateChanged(const QDate &date)
{

}

void MainWindow::on_aviaArrivalComboBox_userDateChanged(const QDate &date)
{

}

void MainWindow::on_aviaAdultSpinBox_valueChanged(int arg1)
{

}

void MainWindow::on_aviaChildrenSpinBox_valueChanged(int arg1)
{

}

void MainWindow::on_aviaBabySpinBox_valueChanged(int arg1)
{

}

void MainWindow::on_aviaTypeComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_aviaTypeComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_aviaSearchPushButton_clicked()
{

}

void MainWindow::on_railwayFromComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_railwayFromComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_railwayToComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_railwayToComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_railwayDepartComboBox_userDateChanged(const QDate &date)
{

}

void MainWindow::on_railwayArrivalComboBox_userDateChanged(const QDate &date)
{

}

void MainWindow::on_railwaySearchPushButton_clicked()
{

}

void MainWindow::on_busFromComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_busFromComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_busToComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_busToComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_busDepartComboBox_userDateChanged(const QDate &date)
{

}

void MainWindow::on_busPassengersSpinBox_valueChanged(int arg1)
{

}

void MainWindow::on_trainFromComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_trainFromComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_trainToComboBox_activated(const QString &arg1)
{

}

void MainWindow::on_trainToComboBox_editTextChanged(const QString &arg1)
{

}

void MainWindow::on_trainDepartComboBox_userDateChanged(const QDate &date)
{

}

void MainWindow::on_busSearchPushButton_clicked()
{

}

void MainWindow::on_trainSearchPushButton_clicked()
{

}
