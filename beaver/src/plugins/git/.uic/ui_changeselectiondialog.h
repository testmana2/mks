/********************************************************************************
** Form generated from reading ui file 'changeselectiondialog.ui'
**
** Created: Fri Mar 6 15:15:52 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHANGESELECTIONDIALOG_H
#define UI_CHANGESELECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ChangeSelectionDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *repositoryEdit;
    QPushButton *repositoryButton;
    QLabel *label_2;
    QLineEdit *changeNumberEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ChangeSelectionDialog)
    {
        if (ChangeSelectionDialog->objectName().isEmpty())
            ChangeSelectionDialog->setObjectName(QString::fromUtf8("ChangeSelectionDialog"));
        ChangeSelectionDialog->resize(401, 142);
        gridLayout = new QGridLayout(ChangeSelectionDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ChangeSelectionDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        repositoryEdit = new QLineEdit(ChangeSelectionDialog);
        repositoryEdit->setObjectName(QString::fromUtf8("repositoryEdit"));

        gridLayout->addWidget(repositoryEdit, 0, 1, 1, 1);

        repositoryButton = new QPushButton(ChangeSelectionDialog);
        repositoryButton->setObjectName(QString::fromUtf8("repositoryButton"));

        gridLayout->addWidget(repositoryButton, 0, 2, 1, 1);

        label_2 = new QLabel(ChangeSelectionDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        changeNumberEdit = new QLineEdit(ChangeSelectionDialog);
        changeNumberEdit->setObjectName(QString::fromUtf8("changeNumberEdit"));

        gridLayout->addWidget(changeNumberEdit, 1, 1, 1, 1);

        buttonBox = new QDialogButtonBox(ChangeSelectionDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 3);


        retranslateUi(ChangeSelectionDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ChangeSelectionDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ChangeSelectionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ChangeSelectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangeSelectionDialog)
    {
        ChangeSelectionDialog->setWindowTitle(QApplication::translate("ChangeSelectionDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ChangeSelectionDialog", "Repository Location:", 0, QApplication::UnicodeUTF8));
        repositoryButton->setText(QApplication::translate("ChangeSelectionDialog", "Select", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ChangeSelectionDialog", "Change:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ChangeSelectionDialog);
    } // retranslateUi

};

namespace Ui {
    class ChangeSelectionDialog: public Ui_ChangeSelectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGESELECTIONDIALOG_H
