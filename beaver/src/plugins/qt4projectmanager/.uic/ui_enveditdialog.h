/********************************************************************************
** Form generated from reading ui file 'enveditdialog.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ENVEDITDIALOG_H
#define UI_ENVEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_EnvEditDialog
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLineEdit *envNameEdit;
    QLabel *label_4;
    QLineEdit *makeEdit;
    QLabel *label;
    QLabel *label_5;
    QComboBox *mkspecCombo;
    QGridLayout *gridLayout1;
    QLineEdit *varEdit;
    QSpacerItem *spacerItem;
    QTreeWidget *varList;
    QLineEdit *valEdit;
    QLabel *label_3;
    QLabel *label_2;
    QToolButton *addButton;
    QToolButton *delButton;
    QHBoxLayout *hboxLayout;
    QPushButton *importButton;
    QSpacerItem *spacerItem1;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Qt4ProjectManager__Internal__EnvEditDialog)
    {
        if (Qt4ProjectManager__Internal__EnvEditDialog->objectName().isEmpty())
            Qt4ProjectManager__Internal__EnvEditDialog->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__EnvEditDialog"));
        Qt4ProjectManager__Internal__EnvEditDialog->resize(398, 298);
        vboxLayout = new QVBoxLayout(Qt4ProjectManager__Internal__EnvEditDialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(0);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        envNameEdit = new QLineEdit(Qt4ProjectManager__Internal__EnvEditDialog);
        envNameEdit->setObjectName(QString::fromUtf8("envNameEdit"));

        gridLayout->addWidget(envNameEdit, 0, 1, 1, 1);

        label_4 = new QLabel(Qt4ProjectManager__Internal__EnvEditDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        makeEdit = new QLineEdit(Qt4ProjectManager__Internal__EnvEditDialog);
        makeEdit->setObjectName(QString::fromUtf8("makeEdit"));

        gridLayout->addWidget(makeEdit, 1, 1, 1, 1);

        label = new QLabel(Qt4ProjectManager__Internal__EnvEditDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(Qt4ProjectManager__Internal__EnvEditDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        mkspecCombo = new QComboBox(Qt4ProjectManager__Internal__EnvEditDialog);
        mkspecCombo->setObjectName(QString::fromUtf8("mkspecCombo"));

        gridLayout->addWidget(mkspecCombo, 2, 1, 1, 1);


        vboxLayout->addLayout(gridLayout);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setMargin(0);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        varEdit = new QLineEdit(Qt4ProjectManager__Internal__EnvEditDialog);
        varEdit->setObjectName(QString::fromUtf8("varEdit"));

        gridLayout1->addWidget(varEdit, 2, 1, 1, 1);

        spacerItem = new QSpacerItem(20, 91, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacerItem, 1, 2, 1, 1);

        varList = new QTreeWidget(Qt4ProjectManager__Internal__EnvEditDialog);
        varList->setObjectName(QString::fromUtf8("varList"));
        varList->setUniformRowHeights(true);
        varList->setItemsExpandable(false);
        varList->setColumnCount(2);

        gridLayout1->addWidget(varList, 0, 0, 2, 2);

        valEdit = new QLineEdit(Qt4ProjectManager__Internal__EnvEditDialog);
        valEdit->setObjectName(QString::fromUtf8("valEdit"));

        gridLayout1->addWidget(valEdit, 3, 1, 1, 1);

        label_3 = new QLabel(Qt4ProjectManager__Internal__EnvEditDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout1->addWidget(label_3, 3, 0, 1, 1);

        label_2 = new QLabel(Qt4ProjectManager__Internal__EnvEditDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout1->addWidget(label_2, 2, 0, 1, 1);

        addButton = new QToolButton(Qt4ProjectManager__Internal__EnvEditDialog);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        gridLayout1->addWidget(addButton, 2, 2, 1, 1);

        delButton = new QToolButton(Qt4ProjectManager__Internal__EnvEditDialog);
        delButton->setObjectName(QString::fromUtf8("delButton"));

        gridLayout1->addWidget(delButton, 0, 2, 1, 1);


        vboxLayout->addLayout(gridLayout1);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        importButton = new QPushButton(Qt4ProjectManager__Internal__EnvEditDialog);
        importButton->setObjectName(QString::fromUtf8("importButton"));

        hboxLayout->addWidget(importButton);

        spacerItem1 = new QSpacerItem(91, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);

        okButton = new QPushButton(Qt4ProjectManager__Internal__EnvEditDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(Qt4ProjectManager__Internal__EnvEditDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout);

        QWidget::setTabOrder(envNameEdit, makeEdit);
        QWidget::setTabOrder(makeEdit, mkspecCombo);
        QWidget::setTabOrder(mkspecCombo, varEdit);
        QWidget::setTabOrder(varEdit, valEdit);
        QWidget::setTabOrder(valEdit, delButton);
        QWidget::setTabOrder(delButton, addButton);
        QWidget::setTabOrder(addButton, varList);
        QWidget::setTabOrder(varList, importButton);
        QWidget::setTabOrder(importButton, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(Qt4ProjectManager__Internal__EnvEditDialog);
        QObject::connect(okButton, SIGNAL(clicked()), Qt4ProjectManager__Internal__EnvEditDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Qt4ProjectManager__Internal__EnvEditDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__EnvEditDialog);
    } // setupUi

    void retranslateUi(QDialog *Qt4ProjectManager__Internal__EnvEditDialog)
    {
        Qt4ProjectManager__Internal__EnvEditDialog->setWindowTitle(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Build Environment", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Make Command:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Build Environment:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "mkspec:", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = varList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "1", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "0", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Values:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Variable:", 0, QApplication::UnicodeUTF8));
        addButton->setText(QString());
        delButton->setText(QString());
        importButton->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Import", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvEditDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__EnvEditDialog);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class EnvEditDialog: public Ui_EnvEditDialog {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_ENVEDITDIALOG_H
