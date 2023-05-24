/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtDeclarative/QDeclarativeView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include "QtGui/QLineEdit"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QDeclarativeView *mainView;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::WindowModal);
        MainWindow->setEnabled(true);
        MainWindow->resize(800, 800);
        MainWindow->setCursor(QCursor(Qt::PointingHandCursor));
        MainWindow->setMouseTracking(false);
#ifndef QT_NO_STATUSTIP
        MainWindow->setStatusTip(QString::fromUtf8(""));
#endif // QT_NO_STATUSTIP
        MainWindow->setAutoFillBackground(false);
        MainWindow->setInputMethodHints(Qt::ImhDigitsOnly|Qt::ImhUppercaseOnly);
        horizontalLayout = new QHBoxLayout(MainWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        mainView = new QDeclarativeView(MainWindow);
        mainView->setObjectName(QString::fromUtf8("mainView"));
        mainView->setEnabled(true);
        mainView->setMaximumSize(QSize(800, 800));
        mainView->setLayoutDirection(Qt::LeftToRight);
        mainView->setSceneRect(QRectF(0, 0, 800, 800));
        mainView->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        mainView->setResizeMode(QDeclarativeView::SizeViewToRootObject);
        mainView->setSource(QUrl(QString::fromUtf8("qrc:/qml/mainView.qml")));

        horizontalLayout->addWidget(mainView);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
