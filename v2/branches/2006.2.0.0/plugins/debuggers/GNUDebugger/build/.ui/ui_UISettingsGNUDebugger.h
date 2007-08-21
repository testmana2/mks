/********************************************************************************
** Form generated from reading ui file 'UISettingsGNUDebugger.ui'
**
** Created: Tue Feb 27 22:22:45 2007
**      by: Qt User Interface Compiler version 4.2.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_UISETTINGSGNUDEBUGGER_H
#define UI_UISETTINGSGNUDEBUGGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QWidget>

class Ui_UISettingsGNUDebugger
{
public:
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_4;

    void setupUi(QWidget *UISettingsGNUDebugger)
    {
    UISettingsGNUDebugger->setObjectName(QString::fromUtf8("UISettingsGNUDebugger"));
    checkBox = new QCheckBox(UISettingsGNUDebugger);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));
    checkBox->setGeometry(QRect(30, 30, 161, 22));
    checkBox->setChecked(true);
    checkBox_2 = new QCheckBox(UISettingsGNUDebugger);
    checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
    checkBox_2->setGeometry(QRect(30, 60, 83, 22));
    checkBox_2->setChecked(true);
    checkBox_3 = new QCheckBox(UISettingsGNUDebugger);
    checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
    checkBox_3->setGeometry(QRect(30, 90, 83, 22));
    checkBox_3->setChecked(true);
    checkBox_5 = new QCheckBox(UISettingsGNUDebugger);
    checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
    checkBox_5->setGeometry(QRect(210, 60, 83, 22));
    checkBox_4 = new QCheckBox(UISettingsGNUDebugger);
    checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
    checkBox_4->setGeometry(QRect(210, 30, 83, 22));

    retranslateUi(UISettingsGNUDebugger);

    QSize size(400, 300);
    size = size.expandedTo(UISettingsGNUDebugger->minimumSizeHint());
    UISettingsGNUDebugger->resize(size);


    QMetaObject::connectSlotsByName(UISettingsGNUDebugger);
    } // setupUi

    void retranslateUi(QWidget *UISettingsGNUDebugger)
    {
    UISettingsGNUDebugger->setWindowTitle(QApplication::translate("UISettingsGNUDebugger", "Form", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("UISettingsGNUDebugger", "Set confirm", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("UISettingsGNUDebugger", "Set width", 0, QApplication::UnicodeUTF8));
    checkBox_3->setText(QApplication::translate("UISettingsGNUDebugger", "Set height", 0, QApplication::UnicodeUTF8));
    checkBox_5->setText(QApplication::translate("UISettingsGNUDebugger", "-fullname", 0, QApplication::UnicodeUTF8));
    checkBox_4->setText(QApplication::translate("UISettingsGNUDebugger", "-quiet", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(UISettingsGNUDebugger);
    } // retranslateUi

};

namespace Ui {
    class UISettingsGNUDebugger: public Ui_UISettingsGNUDebugger {};
} // namespace Ui

#endif // UI_UISETTINGSGNUDEBUGGER_H
