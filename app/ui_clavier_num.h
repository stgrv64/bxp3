/********************************************************************************
** Form generated from reading UI file 'clavier_num.ui'
**
** Created: Tue Apr 16 09:32:41 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLAVIER_NUM_H
#define UI_CLAVIER_NUM_H

#include <Qt3Support/Q3ButtonGroup>
#include <Qt3Support/Q3GroupBox>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "Qt3Support/Q3ListViewItemIterator"
#include "Qt3Support/Q3Table"
#include "QtGui/QLineEdit"

QT_BEGIN_NAMESPACE

class Ui_clavier_num
{
public:
    Q3ButtonGroup *buttonGroup1;
    QPushButton *T7;
    QPushButton *T4;
    QPushButton *T5;
    QPushButton *T8;
    QPushButton *T9;
    QPushButton *T0;
    QPushButton *TVal;
    QPushButton *T6;
    QPushButton *T3;
    QPushButton *T2;
    QPushButton *T1;
    QPushButton *TCLR;
    QPushButton *TBS;
    QPushButton *TP;
    QPushButton *TM;
    QLineEdit *cl_numlineEdit;

    void setupUi(QDialog *clavier_num)
    {
        if (clavier_num->objectName().isEmpty())
            clavier_num->setObjectName(QString::fromUtf8("clavier_num"));
        clavier_num->setEnabled(true);
        clavier_num->resize(151, 274);
        QFont font;
        font.setPointSize(22);
        clavier_num->setFont(font);
        clavier_num->setCursor(QCursor(Qt::PointingHandCursor));
        clavier_num->setFocusPolicy(Qt::StrongFocus);
        clavier_num->setModal(false);
        buttonGroup1 = new Q3ButtonGroup(clavier_num);
        buttonGroup1->setObjectName(QString::fromUtf8("buttonGroup1"));
        buttonGroup1->setGeometry(QRect(1, 34, 149, 240));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonGroup1->sizePolicy().hasHeightForWidth());
        buttonGroup1->setSizePolicy(sizePolicy);
        buttonGroup1->setFont(font);
        buttonGroup1->setCursor(QCursor(Qt::PointingHandCursor));
        T7 = new QPushButton(buttonGroup1);
        QButtonGroup *buttonGroup = new QButtonGroup(clavier_num);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(T7);
        T7->setObjectName(QString::fromUtf8("T7"));
        T7->setGeometry(QRect(1, 2, 50, 40));
        sizePolicy.setHeightForWidth(T7->sizePolicy().hasHeightForWidth());
        T7->setSizePolicy(sizePolicy);
        T7->setFont(font);
        T7->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T7, 55);
        T4 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T4);
        T4->setObjectName(QString::fromUtf8("T4"));
        T4->setGeometry(QRect(1, 40, 50, 40));
        sizePolicy.setHeightForWidth(T4->sizePolicy().hasHeightForWidth());
        T4->setSizePolicy(sizePolicy);
        T4->setFont(font);
        T4->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T4, 52);
        T5 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T5);
        T5->setObjectName(QString::fromUtf8("T5"));
        T5->setGeometry(QRect(49, 40, 50, 40));
        sizePolicy.setHeightForWidth(T5->sizePolicy().hasHeightForWidth());
        T5->setSizePolicy(sizePolicy);
        T5->setFont(font);
        T5->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T5, 53);
        T8 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T8);
        T8->setObjectName(QString::fromUtf8("T8"));
        T8->setGeometry(QRect(49, 2, 50, 40));
        sizePolicy.setHeightForWidth(T8->sizePolicy().hasHeightForWidth());
        T8->setSizePolicy(sizePolicy);
        T8->setFont(font);
        T8->setFocusPolicy(Qt::NoFocus);
        T8->setFlat(false);
        buttonGroup1->insert(T8, 56);
        T9 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T9);
        T9->setObjectName(QString::fromUtf8("T9"));
        T9->setGeometry(QRect(97, 2, 50, 40));
        T9->setFont(font);
        T9->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T9, 57);
        T0 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T0);
        T0->setObjectName(QString::fromUtf8("T0"));
        T0->setGeometry(QRect(1, 116, 50, 40));
        sizePolicy.setHeightForWidth(T0->sizePolicy().hasHeightForWidth());
        T0->setSizePolicy(sizePolicy);
        buttonGroup1->insert(T0, 48);
        TVal = new QPushButton(buttonGroup1);
        buttonGroup->addButton(TVal);
        TVal->setObjectName(QString::fromUtf8("TVal"));
        TVal->setGeometry(QRect(9, 197, 130, 40));
        TVal->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(TVal, 100);
        T6 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T6);
        T6->setObjectName(QString::fromUtf8("T6"));
        T6->setGeometry(QRect(97, 40, 50, 40));
        sizePolicy.setHeightForWidth(T6->sizePolicy().hasHeightForWidth());
        T6->setSizePolicy(sizePolicy);
        T6->setFont(font);
        T6->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T6, 54);
        T3 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T3);
        T3->setObjectName(QString::fromUtf8("T3"));
        T3->setGeometry(QRect(97, 78, 50, 40));
        sizePolicy.setHeightForWidth(T3->sizePolicy().hasHeightForWidth());
        T3->setSizePolicy(sizePolicy);
        T3->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T3, 51);
        T2 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T2);
        T2->setObjectName(QString::fromUtf8("T2"));
        T2->setGeometry(QRect(49, 78, 50, 40));
        sizePolicy.setHeightForWidth(T2->sizePolicy().hasHeightForWidth());
        T2->setSizePolicy(sizePolicy);
        buttonGroup1->insert(T2, 50);
        T1 = new QPushButton(buttonGroup1);
        buttonGroup->addButton(T1);
        T1->setObjectName(QString::fromUtf8("T1"));
        T1->setGeometry(QRect(1, 78, 50, 40));
        sizePolicy.setHeightForWidth(T1->sizePolicy().hasHeightForWidth());
        T1->setSizePolicy(sizePolicy);
        T1->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(T1, 49);
        TCLR = new QPushButton(buttonGroup1);
        buttonGroup->addButton(TCLR);
        TCLR->setObjectName(QString::fromUtf8("TCLR"));
        TCLR->setGeometry(QRect(75, 156, 72, 40));
        sizePolicy.setHeightForWidth(TCLR->sizePolicy().hasHeightForWidth());
        TCLR->setSizePolicy(sizePolicy);
        TCLR->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(TCLR, 98);
        TBS = new QPushButton(buttonGroup1);
        buttonGroup->addButton(TBS);
        TBS->setObjectName(QString::fromUtf8("TBS"));
        TBS->setGeometry(QRect(1, 156, 72, 40));
        sizePolicy.setHeightForWidth(TBS->sizePolicy().hasHeightForWidth());
        TBS->setSizePolicy(sizePolicy);
        TBS->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(TBS, 99);
        TP = new QPushButton(buttonGroup1);
        buttonGroup->addButton(TP);
        TP->setObjectName(QString::fromUtf8("TP"));
        TP->setGeometry(QRect(49, 116, 50, 40));
        sizePolicy.setHeightForWidth(TP->sizePolicy().hasHeightForWidth());
        TP->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(22);
        font1.setBold(true);
        font1.setWeight(75);
        TP->setFont(font1);
        TP->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(TP, 46);
        TM = new QPushButton(buttonGroup1);
        buttonGroup->addButton(TM);
        TM->setObjectName(QString::fromUtf8("TM"));
        TM->setGeometry(QRect(97, 116, 50, 40));
        sizePolicy.setHeightForWidth(TM->sizePolicy().hasHeightForWidth());
        TM->setSizePolicy(sizePolicy);
        TM->setFont(font1);
        TM->setFocusPolicy(Qt::NoFocus);
        buttonGroup1->insert(TM, 45);
        cl_numlineEdit = new QLineEdit(clavier_num);
        cl_numlineEdit->setObjectName(QString::fromUtf8("cl_numlineEdit"));
        cl_numlineEdit->setGeometry(QRect(34, 1, 80, 31));
        cl_numlineEdit->setFont(font);
        cl_numlineEdit->setMaxLength(6);
        QWidget::setTabOrder(T0, cl_numlineEdit);
        QWidget::setTabOrder(cl_numlineEdit, T2);

        retranslateUi(clavier_num);
        QObject::connect(buttonGroup1, SIGNAL(clicked(int)), clavier_num, SLOT(buttonGroup1_clicked(int)));

        QMetaObject::connectSlotsByName(clavier_num);
    } // setupUi

    void retranslateUi(QDialog *clavier_num)
    {
        clavier_num->setWindowTitle(QApplication::translate("clavier_num", "-", 0, QApplication::UnicodeUTF8));
        buttonGroup1->setTitle(QString());
        T7->setText(QApplication::translate("clavier_num", "7", 0, QApplication::UnicodeUTF8));
        T7->setShortcut(QApplication::translate("clavier_num", "7", 0, QApplication::UnicodeUTF8));
        T4->setText(QApplication::translate("clavier_num", "4", 0, QApplication::UnicodeUTF8));
        T4->setShortcut(QApplication::translate("clavier_num", "4", 0, QApplication::UnicodeUTF8));
        T5->setText(QApplication::translate("clavier_num", "5", 0, QApplication::UnicodeUTF8));
        T5->setShortcut(QApplication::translate("clavier_num", "5", 0, QApplication::UnicodeUTF8));
        T8->setText(QApplication::translate("clavier_num", "8", 0, QApplication::UnicodeUTF8));
        T8->setShortcut(QApplication::translate("clavier_num", "8", 0, QApplication::UnicodeUTF8));
        T9->setText(QApplication::translate("clavier_num", "9", 0, QApplication::UnicodeUTF8));
        T9->setShortcut(QApplication::translate("clavier_num", "9", 0, QApplication::UnicodeUTF8));
        T0->setText(QApplication::translate("clavier_num", "0", 0, QApplication::UnicodeUTF8));
        T0->setShortcut(QApplication::translate("clavier_num", "0", 0, QApplication::UnicodeUTF8));
        TVal->setText(QString());
        TVal->setShortcut(QApplication::translate("clavier_num", "Return", 0, QApplication::UnicodeUTF8));
        T6->setText(QApplication::translate("clavier_num", "6", 0, QApplication::UnicodeUTF8));
        T6->setShortcut(QApplication::translate("clavier_num", "6", 0, QApplication::UnicodeUTF8));
        T3->setText(QApplication::translate("clavier_num", "3", 0, QApplication::UnicodeUTF8));
        T3->setShortcut(QApplication::translate("clavier_num", "3", 0, QApplication::UnicodeUTF8));
        T2->setText(QApplication::translate("clavier_num", "2", 0, QApplication::UnicodeUTF8));
        T2->setShortcut(QApplication::translate("clavier_num", "2", 0, QApplication::UnicodeUTF8));
        T1->setText(QApplication::translate("clavier_num", "1", 0, QApplication::UnicodeUTF8));
        T1->setShortcut(QApplication::translate("clavier_num", "1", 0, QApplication::UnicodeUTF8));
        TCLR->setText(QString());
        TCLR->setShortcut(QApplication::translate("clavier_num", "Del", 0, QApplication::UnicodeUTF8));
        TBS->setText(QString());
        TBS->setShortcut(QString());
        TP->setText(QApplication::translate("clavier_num", ".", 0, QApplication::UnicodeUTF8));
        TP->setShortcut(QApplication::translate("clavier_num", ".", 0, QApplication::UnicodeUTF8));
        TM->setText(QApplication::translate("clavier_num", "-", 0, QApplication::UnicodeUTF8));
        TM->setShortcut(QApplication::translate("clavier_num", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class clavier_num: public Ui_clavier_num {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLAVIER_NUM_H
