/********************************************************************************
** Form generated from reading UI file 'page4IrxmCk.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PAGE4IRXMCK_H
#define PAGE4IRXMCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page4
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *page4)
    {
        if (page4->objectName().isEmpty())
            page4->setObjectName("page4");
        page4->resize(474, 474);
        page4->setMinimumSize(QSize(474, 474));
        page4->setMaximumSize(QSize(474, 474));
        page4->setStyleSheet(QString::fromUtf8("background:url(:/OIP.png)"));
        centralwidget = new QWidget(page4);
        centralwidget->setObjectName("centralwidget");
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(130, 230, 75, 24));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(130, 320, 75, 24));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(130, 410, 75, 24));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(370, 230, 75, 24));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(370, 320, 75, 24));
        page4->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(page4);
        statusbar->setObjectName("statusbar");
        page4->setStatusBar(statusbar);

        retranslateUi(page4);

        QMetaObject::connectSlotsByName(page4);
    } // setupUi

    void retranslateUi(QMainWindow *page4)
    {
        page4->setWindowTitle(QCoreApplication::translate("page4", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("page4", "tiger", nullptr));
        pushButton_2->setText(QCoreApplication::translate("page4", "t34_85", nullptr));
        pushButton_3->setText(QCoreApplication::translate("page4", "sherman", nullptr));
        pushButton_4->setText(QCoreApplication::translate("page4", "is2", nullptr));
        pushButton_5->setText(QCoreApplication::translate("page4", "churchill", nullptr));
    } // retranslateUi

};

namespace Ui {
    class page4: public Ui_page4 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PAGE4IRXMCK_H
