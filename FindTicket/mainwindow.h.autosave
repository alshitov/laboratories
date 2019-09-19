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

    // Getters for tabs data processing


private slots:
    // Avia Tab slots
    void on_aviaFromComboBox_currentIndexChanged(const QString &arg1);
    void on_aviaToComboBox_currentIndexChanged(const QString &arg1);
    void on_aviaSearchPushButton_clicked();

private:
    Ui::MainWindow *ui;

    void fillAviaTabControls();
    QModelIndex chosenCity;
    void disableChoice(QComboBox *combo, QModelIndex * index);
    void enableChoice(QComboBox *combo, QModelIndex index);

    QMap<QString, QString>* getAviaTabSettings();
    void aviaSearch();

};
#endif // MAINWINDOW_H
