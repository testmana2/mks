/********************************************************************************
** Form generated from reading ui file 'UIItemSettings.ui'
**
** Created: Sun Apr 1 20:48:55 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_UIITEMSETTINGS_H
#define UI_UIITEMSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>

class Ui_UIItemSettings
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *dbbButtons;
    QSpacerItem *spacerItem;
    QTreeWidget *twValueRoles;

    void setupUi(QDialog *UIItemSettings)
    {
    UIItemSettings->setObjectName(QString::fromUtf8("UIItemSettings"));
    gridLayout = new QGridLayout(UIItemSettings);
    gridLayout->setSpacing(3);
    gridLayout->setMargin(5);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    dbbButtons = new QDialogButtonBox(UIItemSettings);
    dbbButtons->setObjectName(QString::fromUtf8("dbbButtons"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(4), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(dbbButtons->sizePolicy().hasHeightForWidth());
    dbbButtons->setSizePolicy(sizePolicy);
    dbbButtons->setOrientation(Qt::Horizontal);
    dbbButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    gridLayout->addWidget(dbbButtons, 1, 1, 1, 1);

    spacerItem = new QSpacerItem(211, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem, 1, 0, 1, 1);

    twValueRoles = new QTreeWidget(UIItemSettings);
    twValueRoles->setObjectName(QString::fromUtf8("twValueRoles"));
    twValueRoles->setEditTriggers(QAbstractItemView::CurrentChanged);
    twValueRoles->setAlternatingRowColors(true);
    twValueRoles->setSelectionBehavior(QAbstractItemView::SelectRows);
    twValueRoles->setRootIsDecorated(false);
    twValueRoles->setItemsExpandable(false);
    twValueRoles->setAllColumnsShowFocus(true);

    gridLayout->addWidget(twValueRoles, 0, 0, 1, 2);


    retranslateUi(UIItemSettings);

    QSize size(533, 288);
    size = size.expandedTo(UIItemSettings->minimumSizeHint());
    UIItemSettings->resize(size);

    QObject::connect(dbbButtons, SIGNAL(accepted()), UIItemSettings, SLOT(accept()));
    QObject::connect(dbbButtons, SIGNAL(rejected()), UIItemSettings, SLOT(reject()));

    QMetaObject::connectSlotsByName(UIItemSettings);
    } // setupUi

    void retranslateUi(QDialog *UIItemSettings)
    {
    UIItemSettings->setWindowTitle(QApplication::translate("UIItemSettings", "Edit item value...", 0, QApplication::UnicodeUTF8));
    twValueRoles->headerItem()->setText(0, QApplication::translate("UIItemSettings", "Role", 0, QApplication::UnicodeUTF8));
    twValueRoles->headerItem()->setText(1, QApplication::translate("UIItemSettings", "Value", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(UIItemSettings);
    } // retranslateUi

};

namespace Ui {
    class UIItemSettings: public Ui_UIItemSettings {};
} // namespace Ui

#endif // UI_UIITEMSETTINGS_H
