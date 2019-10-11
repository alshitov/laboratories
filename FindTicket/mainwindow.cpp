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
    loader::dls.loadCitiesList();
    loader::dls.loadCitiesDistances();
    fillTabControls(ui->findTicketsTabWidget->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Clear table with CTRL+L;
    if (event->key() == Qt::Key_L && event->modifiers() == Qt::ControlModifier)
    {
        QVBoxLayout *layoutPtr;

        switch (ui->findTicketsTabWidget->currentIndex())
        {
        case 0:
            layoutPtr = ui->aviaResultsVertLayout;
            break;
        case 1:
            layoutPtr = ui->railwayResultsVertLayout;
            break;
        case 2:
            layoutPtr = ui->busResultsVertLayout;
            break;
        case 3:
            layoutPtr = ui->trainResultsVertLayout;
            break;
        default:
            layoutPtr = nullptr;
        }

        if (layoutPtr != nullptr)
        {
            QWidget *layoutItemWidget = layoutPtr->itemAt(0)->widget();
            qDebug() << layoutItemWidget;

            if (qobject_cast<QTableWidget*>(layoutItemWidget) != nullptr)
            {
                auto *tablePtr = layoutItemWidget;
                // not working!
//                if (tablePtr->rowCount() == 0)
//                {
//                    qDebug() << "here!";
//                    tablePtr->setRowCount(0);
//                }
            }
            else qDebug() << "Table does not exist yet!";
        }
    }
    // Show previous search results with CTRL+P;
    if (event->key() == Qt::Key_P && event->modifiers() == Qt::ControlModifier)
    {
        // Not implemented
    }
    // Save user's search preferences to config with CTRL+S;
    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier)
    {
        // Not implemented
    }
    // Load user's search preferences from config with CTRL+S;
    if (event->key() == Qt::Key_V && event->modifiers() == Qt::ControlModifier)
    {
        // Not implemented
    }
    QMainWindow::keyPressEvent(event);
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
            messageBox.setInformativeText("Очистить таблицу (Нет - результат поиска будет добавлен в начало таблицы)?");
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
    return ticketsTable;
}

void MainWindow::sortTicketsByRelevance(QJsonArray *tickets)
{
    qDebug() << "Sort here: " << *tickets;
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

    QString company = ticket.value("company").toString();
    QString from = ticket.value("departure_place").toString();
    QString to = ticket.value("arrival_place").toString();
    QString date =   ticket.value("departure_datetime").toString()
                   + " - "
                   + ticket.value("arrival_datetime").toString();
    QString price = QString("%1 %2").arg(
                QString::number(ticket.value("price").toDouble()),
                ticket.value("price_currency").toString()
    );

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

    Q_ASSERT(keys.contains("company"));
    Q_ASSERT(keys.contains("departure_place"));
    Q_ASSERT(keys.contains("arrival_place"));
    Q_ASSERT(keys.contains("departure_datetime"));
    Q_ASSERT(keys.contains("arrival_datetime"));
    Q_ASSERT(keys.contains("price"));
    Q_ASSERT(keys.contains("price_currency"));

    QString company = ticket.value("company").toString();
    QString from = ticket.value("departure_place").toString();
    QString to = ticket.value("arrival_place").toString();
    QString date =   ticket.value("departure_datetime").toString()
                   + " - "
                   + ticket.value("arrival_datetime").toString();
    QString price = QString("%1 %2").arg(
                QString::number(ticket.value("price").toDouble()),
                ticket.value("price_currency").toString()
    );

    values->append(from);
    values->append(to);
    values->append(date);
    values->append(company);
    values->append(price);

    return values;
}

QStringList* MainWindow::busTicketsMapper(const QJsonObject& ticket)
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

    QString company = ticket.value("company").toString();
    QString from = ticket.value("departure_place").toString();
    QString to = ticket.value("arrival_place").toString();
    QString date =   ticket.value("departure_datetime").toString()
                   + " - "
                   + ticket.value("arrival_datetime").toString();
    QString price = QString("%1 %2").arg(
                QString::number(ticket.value("price").toDouble()),
                ticket.value("price_currency").toString()
    );

    values->append(from);
    values->append(to);
    values->append(date);
    values->append(company);
    values->append(price);

    return values;
}

QStringList* MainWindow::trainTicketsMapper(const QJsonObject& ticket)
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

    QString company = ticket.value("company").toString();
    QString from = ticket.value("departure_place").toString();
    QString to = ticket.value("arrival_place").toString();
    QString date =   ticket.value("departure_datetime").toString()
                   + " - "
                   + ticket.value("arrival_datetime").toString();
    QString price = QString("%1 %2").arg(
                QString::number(ticket.value("price").toDouble()),
                ticket.value("price_currency").toString()
    );

    values->append(from);
    values->append(to);
    values->append(date);
    values->append(company);
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
