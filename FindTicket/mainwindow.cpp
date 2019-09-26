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
        if (!*(tabsFilled + 1))
        {
            fillRailwayTabControls();
            *(tabsFilled + 1) = 1;
        }
        break;
    case 2:     // Bus
        if (!*(tabsFilled + 2))
        {
            fillBusTabControls();
            *(tabsFilled + 2) = 1;
        }
        break;
    case 3:     // Train
        if (!*(tabsFilled + 3))
        {
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
    qDebug() << loader::dls.cities->length();
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
    QMap<QString, QString>* settings = getAviaTabSettings();
    loader::dls.loadCitiesDistances();  // once

    // Find Avia Tickets
    QJsonObject* aviaTickets = processor::dps.aviaSearch(*settings);
//    qDebug() << *aviaTickets;

    // Render
}

QMap<QString, QString>* MainWindow::getAviaTabSettings()
{
    /* Method collects user settings from aviaTab */
    QMap<QString, QString>* aviaSettings = new QMap<QString, QString>;
    aviaSettings->insert("depCity", ui->aviaFromComboBox->currentText());
    aviaSettings->insert("destCity", ui->aviaToComboBox->currentText());
    aviaSettings->insert("depDate", ui->aviaDepartDateEdit->text());
    aviaSettings->insert("retDate", ui->aviaReturnDateEdit->text());
    aviaSettings->insert("adultsCnt", QString::number(ui->aviaAdultSpinBox->value()));
    aviaSettings->insert("childCnt", QString::number(ui->aviaChildrenSpinBox->value()));
    aviaSettings->insert("babiesCnt", QString::number(ui->aviaBabySpinBox->value()));
    aviaSettings->insert("rate", ui->aviaRateComboBox->currentText());
    return aviaSettings;
}

QMap<QString, QString>* MainWindow::getRailwayTabSettings()
{
    /* Method collects user settings from aviaTab */
    QMap<QString, QString>* railwaySettings = new QMap<QString, QString>;
    return railwaySettings;
}

QMap<QString, QString>* MainWindow::getBusTabSettings()
{
    /* Method collects user settings from aviaTab */
    QMap<QString, QString>* busSettings = new QMap<QString, QString>;
    return busSettings;
}

QMap<QString, QString>* MainWindow::getTrainTabSettings()
{
    /* Method collects user settings from aviaTab */
    QMap<QString, QString>* trainSettings = new QMap<QString, QString>;
    return trainSettings;
}
