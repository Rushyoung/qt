/********************************************************************************
** Form generated from reading UI file 'page1FqtacP.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PAGE1FQTACP_H
#define PAGE1FQTACP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "ui_page-join.h"
#include "ui_page-creat.h"

QT_BEGIN_NAMESPACE

class Ui_mainpage
{
public:
    QWidget *centralwidget;
    QPushButton *signal;
    QPushButton *multiple;
    QPushButton *exit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainpage)
    {
        if (mainpage->objectName().isEmpty())
            mainpage->setObjectName("mainpage");
        mainpage->resize(474, 474);
        mainpage->setMinimumSize(QSize(474, 474));
        mainpage->setMaximumSize(QSize(474, 474));
        mainpage->setStyleSheet(QString::fromUtf8("background:url(:/OIP-C.png)"));
        centralwidget = new QWidget(mainpage);
        centralwidget->setObjectName("centralwidget");
        signal = new QPushButton(centralwidget);
        signal->setObjectName("signal");
        signal->setGeometry(QRect(170, 200, 131, 61));
        multiple = new QPushButton(centralwidget);
        multiple->setObjectName("multiple");
        multiple->setGeometry(QRect(170, 310, 131, 61));
        exit = new QPushButton(centralwidget);
        exit->setObjectName("exit");
        exit->setGeometry(QRect(380, 410, 75, 24));
        mainpage->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(mainpage);
        statusbar->setObjectName("statusbar");
        mainpage->setStatusBar(statusbar);

        retranslateUi(mainpage);
        QObject::connect(exit, &QPushButton::clicked, mainpage, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(mainpage);
    } // setupUi

    void retranslateUi(QMainWindow *mainpage)
    {
        mainpage->setWindowTitle(QCoreApplication::translate("mainpage", "MainWindow", nullptr));
        signal->setText(QCoreApplication::translate("mainpage", "\345\215\225\344\272\272\346\250\241\345\274\217", nullptr));
        multiple->setText(QCoreApplication::translate("mainpage", "\345\244\232\344\272\272\346\250\241\345\274\217", nullptr));
        exit->setText(QCoreApplication::translate("mainpage", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainpage: public Ui_mainpage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PAGE1FQTACP_H
