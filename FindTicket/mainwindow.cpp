#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <dataloadersingleton.h>
#include <loader.h>
#include <dataprocessorsingleton.h>
#include <processor.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillAviaTabControls();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // Fill controls on tab switch
    switch (index)
    {
    case 1:     // Railway
        if (!*(tabsFilled + 1)) {
            fillRailwayTabControls();
            *(tabsFilled + 1) = 1;
        }
        break;
    case 2:     // Bus
        if (!*(tabsFilled + 2)) {
            fillBusTabControls();
            *(tabsFilled + 2) = 1;
        }
        break;
    case 3:     // Train
        if (!*(tabsFilled + 3)) {
            fillTrainTabControls();
            *(tabsFilled + 3) = 1;
        }
        break;
    }
}

void MainWindow::fillAviaTabControls()
{
    // Fill aviaFrom and aviaTo ComboBoxes
    loader::dls.loadCitiesList();   // once
    ui->aviaFromComboBox->addItems(*loader::dls.cities);
    ui->aviaToComboBox->addItems(*loader::dls.cities);
}

void MainWindow::fillRailwayTabControls()
{
    ui->railwayFromComboBox->addItems(*loader::dls.cities);
    ui->railwayToComboBox->addItems(*loader::dls.cities);
}

void MainWindow::fillBusTabControls()
{
    ui->busFromComboBox->addItems(*loader::dls.cities);
    ui->busToComboBox->addItems(*loader::dls.cities);
}

void MainWindow::fillTrainTabControls()
{
    ui->trainFromComboBox->addItems(*loader::dls.cities);
    ui->trainToComboBox->addItems(*loader::dls.cities);
}

void MainWindow::on_aviaSearchPushButton_clicked()
{
    // Collect data for DataProcessor
    getAviaTabSettings();
    loader::dls.loadCitiesDistances();  // once
    // Call findAviaTickets

    // Call findExtraAviaTickets

    // Return result to MainWindow

    // Render tickets

}

QMap<QString, QString>* MainWindow::getAviaTabSettings()
{
    /* Method collects user settings from aviaTab */
    QMap<QString, QString>* settings = new QMap<QString, QString>;
    settings->insert("pref_department_city", ui->aviaFromComboBox->currentText());
    settings->insert("pref_destination_city", ui->aviaToComboBox->currentText());
    settings->insert("depart_date", ui->aviaDepartDateEdit->text());
    settings->insert("return_date", ui->aviaReturnDateEdit->text());
    settings->insert("adults_cnt", QString::number(ui->aviaAdultSpinBox->value()));
    settings->insert("children_cnt", QString::number(ui->aviaChildrenSpinBox->value()));
    settings->insert("babies_cnt", QString::number(ui->aviaBabySpinBox->value()));
    settings->insert("pref_rate", ui->aviaRateComboBox->currentText());
    return settings;
}

void MainWindow::aviaSearch()
{
    // Animate aviaProgressBar while processor::dps.aviaSearch(...);
    // When found => return result to renderFound(...);
}
