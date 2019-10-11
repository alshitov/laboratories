#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <dataloadersingleton.h>
#include <loader.h>
#include <dataprocessorsingleton.h>
#include <processor.h>
#include <searchthread.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initializeShortcuts();
    loader::dls.loadCitiesList();
    loader::dls.loadCitiesDistances();
    fillTabControls(ui->findTicketsTabWidget->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeShortcuts()
{
    key_Ctrl_L = new QShortcut(this);
    key_Ctrl_L->setKey(Qt::CTRL + Qt::Key_L);
    connect(key_Ctrl_L, SIGNAL(activated()), this, SLOT(clearTicketsTable()));

    this->key_Ctrl_P = new QShortcut(this);
    key_Ctrl_P->setKey(Qt::CTRL + Qt::Key_P);
    connect(key_Ctrl_P, SIGNAL(activated()), this, SLOT(showPreviousResults()));

    this->key_Ctrl_S = new QShortcut(this);
    key_Ctrl_S->setKey(Qt::CTRL + Qt::Key_S);
    connect(key_Ctrl_S, SIGNAL(activated()), this, SLOT(saveUserPreferences()));

    this->key_Ctrl_V = new QShortcut(this);
    key_Ctrl_V->setKey(Qt::CTRL + Qt::Key_V);
    connect(key_Ctrl_V, SIGNAL(activated()), this, SLOT(loadUserPreferences()));
}

void MainWindow::clearTicketsTable()
{
    int currentTab = ui->findTicketsTabWidget->currentIndex();
    QTableWidget *targetTable;

    switch (currentTab)
    {
    case 0:
        targetTable = this->aviaTicketsTable;
        break;
    case 1:
        targetTable = this->railwayTicketsTable;
        break;
    case 2:
        targetTable = this->busTicketsTable;
        break;
    case 3:
        targetTable = this->trainTicketsTable;
        break;
    default:
        targetTable = nullptr;
        break;
    }

    if (targetTable != nullptr)
    {
        targetTable->setRowCount(0);
    }
}

void MainWindow::showPreviousResults()
{
    // Show previous search results with CTRL+P;
    // Not implemented
}

void MainWindow::saveUserPreferences()
{
    // Save user's search preferences to config with CTRL+S;
    // Not implemented
}

void MainWindow::loadUserPreferences()
{
    // Load user's search preferences from config with CTRL+V;
    // Not implemented
}

void MainWindow::fillTabControls(int index)
{
    bool empty = *(tabsFilled + index) == 0;

    switch (index)
    {
    case 0:     // Avia
        if (empty)
        {
            ui->aviaFromComboBox->addItems(*loader::dls.cities);
            ui->aviaToComboBox->addItems(*loader::dls.cities);
        }
        break;
    case 1:     // Railway
        if (empty)
        {
            ui->railwayFromComboBox->addItems(*loader::dls.cities);
            ui->railwayToComboBox->addItems(*loader::dls.cities);
        }
        break;
    case 2:     // Bus
        if (empty)
        {
            ui->busFromComboBox->addItems(*loader::dls.cities);
            ui->busToComboBox->addItems(*loader::dls.cities);
        }
        break;
    case 3:     // Train
        if (empty)
        {
            ui->trainFromComboBox->addItems(*loader::dls.cities);
            ui->trainToComboBox->addItems(*loader::dls.cities);
        }
        break;
    }
    if (empty) *(tabsFilled + index) = 1;
}

void MainWindow::on_findTicketsTabWidget_currentChanged(int index)
{
    fillTabControls(index);
}

void MainWindow::on_aviaSearchPushButton_clicked()
{
    SearchThread *searchThread = new SearchThread();
    QMap<QString, QString> *settings = getAviaTabSettings();
    QJsonArray *aviaTickets = processor::dps.aviaSearch(*settings);
    connect(searchThread, SIGNAL(valueChanged(int)), this, SLOT(progressBarIncrement(int)));
    searchThread->start();
    showResult(this->aviaTicketsHeaders, *MainWindow::ui->aviaResultsVertLayout, aviaTickets);
}

void MainWindow::on_railwaySearchPushButton_clicked()
{

}

void MainWindow::on_busSearchPushButton_clicked()
{

}

void MainWindow::on_trainSearchPushButton_clicked()
{

}

void MainWindow::showResult(QStringList* horHeaders, QVBoxLayout& targetLayout, QJsonArray* tickets)
{
    QLayoutItem *layoutItem = targetLayout.itemAt(0);
    QTableWidget *tableWidget;

    if (qobject_cast<QLabel*>(layoutItem->widget()) == nullptr)
    {
        tableWidget = new QTableWidget(layoutItem->widget());
        if (tableWidget->rowCount() != 0)
        {
            QMessageBox messageBox;
            messageBox.setText("Выберите действие");
            messageBox.setInformativeText(
                        "Очистить таблицу (Нет - результат поиска будет добавлен в начало таблицы)?"
            );
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            messageBox.setIcon(QMessageBox::Question);
            messageBox.setDefaultButton(QMessageBox::Yes);
            int result = messageBox.exec();
            if (result == QMessageBox::Yes)
            {
                tableWidget->setRowCount(0);
            }
        }
    }
    else
    {
        targetLayout.layout()->removeItem(layoutItem);
        tableWidget = createTicketsTable(horHeaders->count(), horHeaders);
        targetLayout.addWidget(tableWidget);
    }
    sortTicketsByRelevance(tickets);
    fillTable(tableWidget, tickets);
}

QTableWidget* MainWindow::createTicketsTable(int colCnt, QStringList* headers)
{
    QTableWidget *ticketsTable = new QTableWidget();

    int maxWidth = 610;
    int normalSectionW = int((maxWidth - 40) / colCnt);
    ticketsTable->setColumnCount(colCnt + 1);
    ticketsTable->setMaximumWidth(maxWidth);
    headers->push_back("Ещё");
    ticketsTable->setHorizontalHeaderLabels(*headers);
    ticketsTable->horizontalHeader()->setDefaultSectionSize(normalSectionW);
    ticketsTable->horizontalHeader()->setStretchLastSection(true);
    ticketsTable->verticalHeader()->setVisible(false);

    if (headers == this->aviaTicketsHeaders)
    {
        qDebug() << "set avia";
        this->aviaTicketsTable = ticketsTable;
    }
    else if (headers == this->railwayTicketsHeaders)
    {
        this->railwayTicketsTable = ticketsTable;
    }
    else if (headers == this->busTicketsHeaders)
    {
        this->busTicketsTable = ticketsTable;
    }
    else if (headers == this->trainTicketsHeaders)
    {
        this->trainTicketsTable = ticketsTable;
    }
    return ticketsTable;
}

void MainWindow::sortTicketsByRelevance(QJsonArray *tickets)
{
    qDebug() << "Sort here";
}

QStringList* MainWindow::aviaTicketsMapper(const QJsonObject& ticket)
{
    QStringList *values = new QStringList();
    QList<QString> keys = ticket.keys();

    Q_ASSERT(keys.contains("company"));
    Q_ASSERT(keys.contains("departure_place"));
    Q_ASSERT(keys.contains("arrival_place"));
    Q_ASSERT(keys.contains("departure_datetime"));
    Q_ASSERT(keys.contains("arrival_datetime"));
    Q_ASSERT(keys.contains("price"));
    Q_ASSERT(keys.contains("price_currency"));

    QString company = ticket.value("company").toString(),
            from    = ticket.value("departure_place").toString(),
            to      = ticket.value("arrival_place").toString(),
            date    = ticket.value("departure_datetime").toString()
                            + " - "
                            + ticket.value("arrival_datetime").toString(),
            price   = QString("%1 %2").arg(
                              QString::number(ticket.value("price").toDouble()),
                              ticket.value("price_currency").toString());

    values->append(from);
    values->append(to);
    values->append(date);
    values->append(company);
    values->append(price);

    return values;
}

QStringList* MainWindow::railwayTicketsMapper(const QJsonObject& ticket)
{
    QStringList *values = new QStringList();
    QList<QString> keys = ticket.keys();

    Q_ASSERT(keys.contains("departure_place"));
    Q_ASSERT(keys.contains("arrival_place"));
    Q_ASSERT(keys.contains("departure_datetime"));
    Q_ASSERT(keys.contains("arrival_datetime"));
    Q_ASSERT(keys.contains("company"));
    Q_ASSERT(keys.contains("price"));
    Q_ASSERT(keys.contains("price_currency"));

    QString route   = ticket.value("departure_place").toString()
                    + " - "
                    + ticket.value("arrival_place").toString(),
            depDate = ticket.value("departure_datetime").toString(),
            arrDate = ticket.value("arrival_datetime").toString(),
            company = ticket.value("company").toString(),
            price   = QString("%1 %2").arg(
                              QString::number(ticket.value("price").toDouble()),
                              ticket.value("price_currency").toString());

    values->append(route);
    values->append(depDate);
    values->append(arrDate);
    values->append(company);
    values->append(price);

    return values;
}

QStringList* MainWindow::busTicketsMapper(const QJsonObject& ticket)
{
    QStringList *values = new QStringList();
    QList<QString> keys = ticket.keys();

    Q_ASSERT(keys.contains("departure_place"));
    Q_ASSERT(keys.contains("arrival_place"));
    Q_ASSERT(keys.contains("departure_datetime"));
    Q_ASSERT(keys.contains("arrival_datetime"));
    Q_ASSERT(keys.contains("distance"));
    Q_ASSERT(keys.contains("price"));
    Q_ASSERT(keys.contains("price_currency"));

    QString route   = ticket.value("departure_place").toString()
                    + " - "
                    + ticket.value("arrival_place").toString(),
            depDate = ticket.value("departure_datetime").toString(),
            arrDate = ticket.value("arrival_datetime").toString(),
            dist    = ticket.value("distance").toString(),
            price   = QString("%1 %2").arg(
                              QString::number(ticket.value("price").toDouble()),
                              ticket.value("price_currency").toString());

    values->append(route);
    values->append(depDate);
    values->append(arrDate);
    values->append(dist);
    values->append(price);

    return values;
}

QStringList* MainWindow::trainTicketsMapper(const QJsonObject& ticket)
{
    QStringList *values = new QStringList();
    QList<QString> keys = ticket.keys();

    Q_ASSERT(keys.contains("departure_place"));
    Q_ASSERT(keys.contains("arrival_place"));
    Q_ASSERT(keys.contains("departure_datetime"));
    Q_ASSERT(keys.contains("arrival_datetime"));
    Q_ASSERT(keys.contains("price"));
    Q_ASSERT(keys.contains("price_currency"));

    QString route   = ticket.value("departure_place").toString()
                    + " - "
                    + ticket.value("arrival_place").toString(),
            depDate = ticket.value("departure_datetime").toString(),
            arrDate = ticket.value("arrival_datetime").toString(),
            price   = QString("%1 %2").arg(
                              QString::number(ticket.value("price").toDouble()),
                              ticket.value("price_currency").toString());

    values->append(route);
    values->append(depDate);
    values->append(arrDate);
    values->append(price);

    return values;
}

void MainWindow::fillTable(QTableWidget* table, QJsonArray* tickets)
{
    int ticketsCount = tickets->count();
    table->setRowCount(ticketsCount / 2);
    if (ticketsCount == 0) return;

    for (int i = 0; i <= tickets->count(); ++i)
    {
        QJsonObject ticket = tickets->takeAt(i).toObject();
        QStringList* ticketMapped = aviaTicketsMapper(ticket);
        int count = ticketMapped->count();

        for (int j = 0; j < count; ++j)
        {
            QString value = ticketMapped->value(j);
            QTableWidgetItem *item = new QTableWidgetItem(value);
            table->setItem(i, j, item);
        }
        table->setItem(i, count, new QTableWidgetItem("▼"));
    }
}

void MainWindow::progressBarIncrement(int value)
{
    ui->aviaSearchProgressBar->setValue(value);
}

QMap<QString, QString>* MainWindow::getAviaTabSettings()
{
    QMap<QString, QString>* aviaSettings = new QMap<QString, QString>;
    aviaSettings->insert("depCity",   ui->aviaFromComboBox->currentText());
    aviaSettings->insert("destCity",  ui->aviaToComboBox->currentText());
    aviaSettings->insert("depDate",   ui->aviaDepartDateEdit->text());
    aviaSettings->insert("retDate",   ui->aviaReturnDateEdit->text());
    aviaSettings->insert("adultCnt",  QString::number(ui->aviaAdultSpinBox->value()));
    aviaSettings->insert("childCnt",  QString::number(ui->aviaChildrenSpinBox->value()));
    aviaSettings->insert("babiesCnt", QString::number(ui->aviaBabySpinBox->value()));
    aviaSettings->insert("rate",      ui->aviaRateComboBox->currentText());
    return aviaSettings;
}

QMap<QString, QString>* MainWindow::getRailwayTabSettings()
{
    QMap<QString, QString>* railwaySettings = new QMap<QString, QString>;
    railwaySettings->insert("depCity",  ui->railwayFromComboBox->currentText());
    railwaySettings->insert("destCity", ui->railwayToComboBox->currentText());
    railwaySettings->insert("depDate",  ui->railwayDepartDateEdit->text());
    railwaySettings->insert("retDate",  ui->railwayArrivalDateEdit->text());
    return railwaySettings;
}

QMap<QString, QString>* MainWindow::getBusTabSettings()
{
    QMap<QString, QString>* busSettings = new QMap<QString, QString>;
    busSettings->insert("depCity",  ui->busFromComboBox->currentText());
    busSettings->insert("destCity", ui->busToComboBox->currentText());
    busSettings->insert("depDate",  ui->busDepartDateEdit->text());
    busSettings->insert("passCnt",  QString::number(ui->busPassengersSpinBox->value()));
    return busSettings;
}

QMap<QString, QString>* MainWindow::getTrainTabSettings()
{
    QMap<QString, QString>* trainSettings = new QMap<QString, QString>;
    trainSettings->insert("depCity",  ui->trainFromComboBox->currentText());
    trainSettings->insert("destCity", ui->trainToComboBox->currentText());
    trainSettings->insert("depDate",  ui->trainDepartDateEdit->text());
    return trainSettings;
}
