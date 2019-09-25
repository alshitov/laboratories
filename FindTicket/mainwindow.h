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
    // Getters for tabs data processing
    QMap<QString, QString>* getAviaTabSettings();
    QMap<QString, QString>* getRailwayTabSettings();
    QMap<QString, QString>* getBusTabSettings();
    QMap<QString, QString>* getTrainTabSettings();

    void aviaSearch();
    void railwaySearch();
    void busSearch();
    void trainSearch();

};
#endif // MAINWINDOW_H
