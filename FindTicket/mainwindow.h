#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#pragma once
#include <QtCore>
#include <QtWidgets>

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
    void on_aviaSearchPushButton_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    int *tabsFilled = new int[4] {0, 0, 0, 0};
    void fillAviaTabControls();
    void fillRailwayTabControls();
    void fillBusTabControls();
    void fillTrainTabControls();

    /* Tickets search pipeline */
    // Arrays of pointers to controls from which to get user settings
    QList<QObject*> aviaTabControls;
    QList<QObject*> railwayTabControls;
    QList<QObject*> busTabControls;
    QList<QObject*> trainTabControls;
    // Tab settings getter
    QMap<QString, QString>* getAviaTabSettings();
    QMap<QString, QString>* getRailwayTabSettings();
    QMap<QString, QString>* getBusTabSettings();
    QMap<QString, QString>* getTrainTabSettings();
    // Search methods
    void aviaSearch();
    void railwaySearch();
    void busSearch();
    void trainSearch();

    /* Tickets render pipeline */
    QStringList* aviaTicketsHeaders = new QStringList { "From", "To", "Date", "Company", "Price" };

    QTableWidget* createTicketsTable(int colCnt, QStringList& headers);
    void fillTable(QTableWidget& table, QJsonArray& tickets);
    void insertTable(QTableWidget& table, QVBoxLayout& layout);
    void showResult(QStringList& horHeaders, QVBoxLayout& targetLayout, QJsonArray& tickets);
};
#endif // MAINWINDOW_H
