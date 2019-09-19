#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <dataloader.h>
#include <dataprocessor.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // MainWindow constructor.
    ui->setupUi(this);

    // Instantiate data processing classes
    dataLoader = DataLoader();
    dataProcessor = DataProcessor();
    fillAviaTabControls();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillAviaTabControls()
{
    // Fill aviaFrom and aviaTo ComboBoxes
    dataLoader.loadCitiesList();
    ui->aviaFromComboBox->addItems(dataLoader.cities);
    ui->aviaToComboBox->addItems(dataLoader.cities);
}

void MainWindow::on_aviaFromComboBox_currentIndexChanged(const QString &arg1)
{
    enableChoice(ui->aviaToComboBox, chosenCity);
    int chosenCityIndex = ui->aviaToComboBox->findText(arg1);
    QModelIndex index = ui->aviaToComboBox->model()->index(chosenCityIndex, 0);
    chosenCity = index;
    disableChoice(ui->aviaToComboBox, &index);
}

void MainWindow::on_aviaToComboBox_currentIndexChanged(const QString &arg1)
{
    enableChoice(ui->aviaFromComboBox, chosenCity);
    int chosenCityIndex = ui->aviaFromComboBox->findText(arg1);
    QModelIndex index = ui->aviaFromComboBox->model()->index(chosenCityIndex, 0);
    chosenCity = index;
    disableChoice(ui->aviaFromComboBox, &index);
}

void MainWindow::disableChoice(QComboBox *combo, QModelIndex *index)
{
    combo->model()->setData(*index, QVariant(0), Qt::UserRole - 1);
}

void MainWindow::enableChoice(QComboBox *combo, QModelIndex index)
{
    combo->model()->setData(index, QVariant(1 | 32), Qt::UserRole - 1);
}



