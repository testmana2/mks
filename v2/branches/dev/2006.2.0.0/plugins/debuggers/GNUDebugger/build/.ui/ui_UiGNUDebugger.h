/********************************************************************************
** Form generated from reading ui file 'UiGNUDebugger.ui'
**
** Created: Tue Feb 27 22:22:45 2007
**      by: Qt User Interface Compiler version 4.2.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_UIGNUDEBUGGER_H
#define UI_UIGNUDEBUGGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_UiGNUDebugger
{
public:
    QWidget *wCentral;
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QVBoxLayout *vboxLayout;
    QPushButton *butonNextStepOver;
    QPushButton *butonNextStepInto;
    QPushButton *butonContinue;
    QLabel *gdbWorking;
    QSpacerItem *spacerItem;
    QStackedWidget *stackedWidget;
    QWidget *page_2;

    void setupUi(QDockWidget *UiGNUDebugger)
    {
    UiGNUDebugger->setObjectName(QString::fromUtf8("UiGNUDebugger"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(UiGNUDebugger->sizePolicy().hasHeightForWidth());
    UiGNUDebugger->setSizePolicy(sizePolicy);
    UiGNUDebugger->setMinimumSize(QSize(0, 120));
    wCentral = new QWidget(UiGNUDebugger);
    wCentral->setObjectName(QString::fromUtf8("wCentral"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(13), static_cast<QSizePolicy::Policy>(13));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(wCentral->sizePolicy().hasHeightForWidth());
    wCentral->setSizePolicy(sizePolicy1);
    wCentral->setMinimumSize(QSize(16, 16));
    wCentral->setMouseTracking(true);
    wCentral->setWindowIcon(QIcon());
    gridLayout = new QGridLayout(wCentral);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    groupBox = new QGroupBox(wCentral);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(13), static_cast<QSizePolicy::Policy>(1));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy2);
    groupBox->setMinimumSize(QSize(150, 16));
    gridLayout1 = new QGridLayout(groupBox);
    gridLayout1->setSpacing(0);
    gridLayout1->setMargin(0);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    butonNextStepOver = new QPushButton(groupBox);
    butonNextStepOver->setObjectName(QString::fromUtf8("butonNextStepOver"));
    butonNextStepOver->setIcon(QIcon(QString::fromUtf8(":/icons/hi22-action-pgmnext.png")));

    vboxLayout->addWidget(butonNextStepOver);

    butonNextStepInto = new QPushButton(groupBox);
    butonNextStepInto->setObjectName(QString::fromUtf8("butonNextStepInto"));
    butonNextStepInto->setIcon(QIcon(QString::fromUtf8(":/icons/hi22-action-pgmstep.png")));

    vboxLayout->addWidget(butonNextStepInto);

    butonContinue = new QPushButton(groupBox);
    butonContinue->setObjectName(QString::fromUtf8("butonContinue"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(butonContinue->sizePolicy().hasHeightForWidth());
    butonContinue->setSizePolicy(sizePolicy3);
    butonContinue->setIcon(QIcon(QString::fromUtf8(":/icons/hi22-action-pgmrun.png")));

    vboxLayout->addWidget(butonContinue);

    gdbWorking = new QLabel(groupBox);
    gdbWorking->setObjectName(QString::fromUtf8("gdbWorking"));
    QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(gdbWorking->sizePolicy().hasHeightForWidth());
    gdbWorking->setSizePolicy(sizePolicy4);
    gdbWorking->setAutoFillBackground(false);
    gdbWorking->setFrameShape(QFrame::NoFrame);
    gdbWorking->setLineWidth(1);
    gdbWorking->setScaledContents(false);
    gdbWorking->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(gdbWorking);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

    vboxLayout->addItem(spacerItem);


    gridLayout1->addLayout(vboxLayout, 0, 0, 1, 1);


    hboxLayout->addWidget(groupBox);

    stackedWidget = new QStackedWidget(wCentral);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    QSizePolicy sizePolicy5(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(13));
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(page_2->sizePolicy().hasHeightForWidth());
    page_2->setSizePolicy(sizePolicy5);
    stackedWidget->addWidget(page_2);

    hboxLayout->addWidget(stackedWidget);


    gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

    UiGNUDebugger->setWidget(wCentral);

    retranslateUi(UiGNUDebugger);

    QSize size(205, 150);
    size = size.expandedTo(UiGNUDebugger->minimumSizeHint());
    UiGNUDebugger->resize(size);


    QMetaObject::connectSlotsByName(UiGNUDebugger);
    } // setupUi

    void retranslateUi(QDockWidget *UiGNUDebugger)
    {
    wCentral->setWindowTitle(QApplication::translate("UiGNUDebugger", "Debuger", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("UiGNUDebugger", "Remote debugger", 0, QApplication::UnicodeUTF8));
    butonNextStepOver->setText(QApplication::translate("UiGNUDebugger", "Next step over", 0, QApplication::UnicodeUTF8));
    butonNextStepInto->setText(QApplication::translate("UiGNUDebugger", "Next step into", 0, QApplication::UnicodeUTF8));
    butonContinue->setText(QApplication::translate("UiGNUDebugger", "Continue", 0, QApplication::UnicodeUTF8));
    gdbWorking->setText(QApplication::translate("UiGNUDebugger", "Gdb no started ", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(UiGNUDebugger);
    } // retranslateUi

};

namespace Ui {
    class UiGNUDebugger: public Ui_UiGNUDebugger {};
} // namespace Ui

#endif // UI_UIGNUDEBUGGER_H
