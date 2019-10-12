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
    void clearTicketsTable();
    void showPreviousResults();
    void saveUserPreferences();
    void loadUserPreferences();

    void on_aviaSearchPushButton_clicked();
    void on_railwaySearchPushButton_clicked();
    void on_busSearchPushButton_clicked();
    void on_trainSearchPushButton_clicked();

    void on_findTicketsTabWidget_currentChanged(int index);
    void progressBarIncrement(int percents);

private:
    Ui::MainWindow *ui;

    QShortcut *key_Ctrl_L;
    QShortcut *key_Ctrl_P;
    QShortcut *key_Ctrl_S;
    QShortcut *key_Ctrl_V;
    void initializeShortcuts();

    int *tabsFilled = new int[4] {0, 0, 0, 0};
    void fillTabControls(int index);

    QMap<QString, QString>* getAviaTabSettings();
    QMap<QString, QString>* getRailwayTabSettings();
    QMap<QString, QString>* getBusTabSettings();
    QMap<QString, QString>* getTrainTabSettings();

    void aviaSearch();
    void railwaySearch();
    void busSearch();
    void trainSearch();
    QStringList* aviaTicketsHeaders = new QStringList {
        "Откуда", "Куда", "Даты", "Компания", "Цена"
    };
    QStringList* railwayTicketsHeaders = new QStringList {
        "Маршрут", "Отправление", "Прибытие", "Перевозчик", "Цена"
    };
    QStringList* busTicketsHeaders = new QStringList {
        "Маршрут", "Отправление", "Прибытие", "В пути (км)", "Цена"
    };
    QStringList* trainTicketsHeaders = new QStringList {
        "Маршрут", "Отправление", "Прибытие", "Цена"
    };

    QTableWidget *aviaTicketsTable = new QTableWidget();
    QTableWidget *railwayTicketsTable = new QTableWidget();
    QTableWidget *busTicketsTable = new QTableWidget();
    QTableWidget *trainTicketsTable = new QTableWidget();

    void showResult(QStringList* horHeaders, QVBoxLayout& targetLayout, QJsonArray* tickets);
    QStringList* aviaTicketsMapper(const QJsonObject& ticket);
    QStringList* railwayTicketsMapper(const QJsonObject& ticket);
    QStringList* busTicketsMapper(const QJsonObject& ticket);
    QStringList* trainTicketsMapper(const QJsonObject& ticket);

    void initializeTicketsTables();
    QTableWidget* getTicketsTable(QStringList* headers);
    void sortTicketsByRelevance(QJsonArray *tickets);
    void fillTable(QTableWidget* table, QJsonArray* tickets);
};
#endif // MAINWINDOW_H
