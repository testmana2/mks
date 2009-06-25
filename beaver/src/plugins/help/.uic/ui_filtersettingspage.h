/********************************************************************************
** Form generated from reading ui file 'filtersettingspage.ui'
**
** Created: Fri Mar 6 15:16:21 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FILTERSETTINGSPAGE_H
#define UI_FILTERSETTINGSPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterSettingsPage
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QListWidget *filterWidget;
    QTreeWidget *attributeWidget;
    QPushButton *filterAddButton;
    QPushButton *filterRemoveButton;

    void setupUi(QWidget *FilterSettingsPage)
    {
        if (FilterSettingsPage->objectName().isEmpty())
            FilterSettingsPage->setObjectName(QString::fromUtf8("FilterSettingsPage"));
        FilterSettingsPage->resize(400, 300);
        gridLayout = new QGridLayout(FilterSettingsPage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FilterSettingsPage);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        label_2 = new QLabel(FilterSettingsPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        filterWidget = new QListWidget(FilterSettingsPage);
        filterWidget->setObjectName(QString::fromUtf8("filterWidget"));

        gridLayout->addWidget(filterWidget, 1, 0, 1, 2);

        attributeWidget = new QTreeWidget(FilterSettingsPage);
        attributeWidget->setObjectName(QString::fromUtf8("attributeWidget"));
        attributeWidget->setProperty("showDropIndicator", QVariant(false));
        attributeWidget->setRootIsDecorated(false);
        attributeWidget->setUniformRowHeights(true);

        gridLayout->addWidget(attributeWidget, 1, 2, 2, 1);

        filterAddButton = new QPushButton(FilterSettingsPage);
        filterAddButton->setObjectName(QString::fromUtf8("filterAddButton"));

        gridLayout->addWidget(filterAddButton, 2, 0, 1, 1);

        filterRemoveButton = new QPushButton(FilterSettingsPage);
        filterRemoveButton->setObjectName(QString::fromUtf8("filterRemoveButton"));

        gridLayout->addWidget(filterRemoveButton, 2, 1, 1, 1);


        retranslateUi(FilterSettingsPage);

        QMetaObject::connectSlotsByName(FilterSettingsPage);
    } // setupUi

    void retranslateUi(QWidget *FilterSettingsPage)
    {
        FilterSettingsPage->setWindowTitle(QApplication::translate("FilterSettingsPage", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FilterSettingsPage", "Filter:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FilterSettingsPage", "Attributes:", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = attributeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("FilterSettingsPage", "1", 0, QApplication::UnicodeUTF8));
        filterAddButton->setText(QApplication::translate("FilterSettingsPage", "Add", 0, QApplication::UnicodeUTF8));
        filterRemoveButton->setText(QApplication::translate("FilterSettingsPage", "Remove", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FilterSettingsPage);
    } // retranslateUi

};

namespace Ui {
    class FilterSettingsPage: public Ui_FilterSettingsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERSETTINGSPAGE_H
