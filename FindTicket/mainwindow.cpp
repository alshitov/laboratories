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

void MainWindow::fillAviaTabControls()
{
    // Fill aviaFrom and aviaTo ComboBoxes
    loader::dls.loadCitiesList();
    ui->aviaFromComboBox->addItems(loader::dls.cities);
    ui->aviaToComboBox->addItems(loader::dls.cities);
}

void MainWindow::on_aviaFromComboBox_currentIndexChanged(const QString &arg1)
{
    /* User made choice in aviaFromComboBox =>
     * enable previously disabled choice in aviaToComboBox
     * and disable newly chosen city.
     */
    enableChoice(ui->aviaToComboBox, chosenCity);
    int chosenCityIndex = ui->aviaToComboBox->findText(arg1);
    QModelIndex index = ui->aviaToComboBox->model()->index(chosenCityIndex, 0);
    chosenCity = index;
    disableChoice(ui->aviaToComboBox, &index);
}

void MainWindow::on_aviaToComboBox_currentIndexChanged(const QString &arg1)
{
    /* User made choice in aviaToComboBox =>
     * enable previously disabled choice in aviaFromComboBox
     * and disable newly chosen city.
     */
    enableChoice(ui->aviaFromComboBox, chosenCity);
    int chosenCityIndex = ui->aviaFromComboBox->findText(arg1);
    QModelIndex index = ui->aviaFromComboBox->model()->index(chosenCityIndex, 0);
    chosenCity = index;
    disableChoice(ui->aviaFromComboBox, &index);
}

void MainWindow::disableChoice(QComboBox *combo, QModelIndex * index)
{
    combo->model()->setData(*index, QVariant(0), Qt::UserRole - 1);
}

void MainWindow::enableChoice(QComboBox *combo, QModelIndex index)
{
    combo->model()->setData(index, QVariant(1 | 32), Qt::UserRole - 1);
}

void MainWindow::on_aviaSearchPushButton_clicked()
{
    // Collect data for DataProcessor
    // Call findAviaTickets
    // Call findExtraAviaTickets
    // Return result to MainWindow
    // Render tickets
    getAviaTabSettings();
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
