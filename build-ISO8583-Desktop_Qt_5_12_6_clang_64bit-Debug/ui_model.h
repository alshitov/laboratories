/********************************************************************************
** Form generated from reading UI file 'model.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODEL_H
#define UI_MODEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Model
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Model)
    {
        if (Model->objectName().isEmpty())
            Model->setObjectName(QString::fromUtf8("Model"));
        Model->resize(800, 600);
        centralwidget = new QWidget(Model);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Model->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Model);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        Model->setMenuBar(menubar);
        statusbar = new QStatusBar(Model);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Model->setStatusBar(statusbar);

        retranslateUi(Model);

        QMetaObject::connectSlotsByName(Model);
    } // setupUi

    void retranslateUi(QMainWindow *Model)
    {
        Model->setWindowTitle(QApplication::translate("Model", "Model", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Model: public Ui_Model {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEL_H
