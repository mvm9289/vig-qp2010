/********************************************************************************
** Form generated from reading ui file 'myForm.ui'
**
** Created: Sat Mar 27 02:48:08 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "myQLCDNumber.h"
#include "myQPushButton.h"

QT_BEGIN_NAMESPACE

class Ui_P0_Application
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    myQPushButton *myQPushButtonObject;
    myQPushButton *myQPushButtonObject_3;
    myQPushButton *myQPushButtonObject_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    myQLCDNumber *myQLCDNumberObject;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *QuitButton;

    void setupUi(QMainWindow *P0_Application)
    {
        if (P0_Application->objectName().isEmpty())
            P0_Application->setObjectName(QString::fromUtf8("P0_Application"));
        P0_Application->resize(332, 198);
        centralwidget = new QWidget(P0_Application);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        myQPushButtonObject = new myQPushButton(centralwidget);
        myQPushButtonObject->setObjectName(QString::fromUtf8("myQPushButtonObject"));
        myQPushButtonObject->setMinimumSize(QSize(100, 100));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../images/img1.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        myQPushButtonObject->setIcon(icon);

        horizontalLayout->addWidget(myQPushButtonObject);

        myQPushButtonObject_3 = new myQPushButton(centralwidget);
        myQPushButtonObject_3->setObjectName(QString::fromUtf8("myQPushButtonObject_3"));
        myQPushButtonObject_3->setMinimumSize(QSize(100, 100));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../images/img2.gif"), QSize(), QIcon::Normal, QIcon::Off);
        myQPushButtonObject_3->setIcon(icon1);

        horizontalLayout->addWidget(myQPushButtonObject_3);

        myQPushButtonObject_2 = new myQPushButton(centralwidget);
        myQPushButtonObject_2->setObjectName(QString::fromUtf8("myQPushButtonObject_2"));
        myQPushButtonObject_2->setMinimumSize(QSize(100, 100));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../images/img3.gif"), QSize(), QIcon::Normal, QIcon::Off);
        myQPushButtonObject_2->setIcon(icon2);

        horizontalLayout->addWidget(myQPushButtonObject_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        myQLCDNumberObject = new myQLCDNumber(centralwidget);
        myQLCDNumberObject->setObjectName(QString::fromUtf8("myQLCDNumberObject"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(myQLCDNumberObject->sizePolicy().hasHeightForWidth());
        myQLCDNumberObject->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(myQLCDNumberObject);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        QuitButton = new QPushButton(centralwidget);
        QuitButton->setObjectName(QString::fromUtf8("QuitButton"));
        sizePolicy.setHeightForWidth(QuitButton->sizePolicy().hasHeightForWidth());
        QuitButton->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(QuitButton);


        verticalLayout->addLayout(horizontalLayout_3);

        P0_Application->setCentralWidget(centralwidget);

        retranslateUi(P0_Application);
        QObject::connect(QuitButton, SIGNAL(clicked()), P0_Application, SLOT(close()));
        QObject::connect(myQPushButtonObject, SIGNAL(clicked()), myQPushButtonObject, SLOT(changeState()));
        QObject::connect(myQPushButtonObject_3, SIGNAL(clicked()), myQPushButtonObject_3, SLOT(changeState()));
        QObject::connect(myQPushButtonObject_2, SIGNAL(clicked()), myQPushButtonObject_2, SLOT(changeState()));
        QObject::connect(myQPushButtonObject, SIGNAL(stateChanged(int)), myQLCDNumberObject, SLOT(changeValueOn(int)));
        QObject::connect(myQPushButtonObject_3, SIGNAL(stateChanged(int)), myQLCDNumberObject, SLOT(changeValueOn(int)));
        QObject::connect(myQPushButtonObject_2, SIGNAL(stateChanged(int)), myQLCDNumberObject, SLOT(changeValueOn(int)));

        QMetaObject::connectSlotsByName(P0_Application);
    } // setupUi

    void retranslateUi(QMainWindow *P0_Application)
    {
        P0_Application->setWindowTitle(QApplication::translate("P0_Application", "P0_Application", 0, QApplication::UnicodeUTF8));
        QuitButton->setText(QApplication::translate("P0_Application", "&Quit", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(P0_Application);
    } // retranslateUi

};

namespace Ui {
    class P0_Application: public Ui_P0_Application {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
