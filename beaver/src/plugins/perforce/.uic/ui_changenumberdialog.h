/********************************************************************************
** Form generated from reading ui file 'changenumberdialog.ui'
**
** Created: Fri Mar 6 15:15:17 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHANGENUMBERDIALOG_H
#define UI_CHANGENUMBERDIALOG_H

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

namespace Perforce {
namespace Internal {

class Ui_ChangeNumberDialog
{
public:
    QGridLayout *gridLayout;
    QLineEdit *numberLineEdit;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Perforce__Internal__ChangeNumberDialog)
    {
        if (Perforce__Internal__ChangeNumberDialog->objectName().isEmpty())
            Perforce__Internal__ChangeNumberDialog->setObjectName(QString::fromUtf8("Perforce__Internal__ChangeNumberDialog"));
        Perforce__Internal__ChangeNumberDialog->resize(319, 76);
        gridLayout = new QGridLayout(Perforce__Internal__ChangeNumberDialog);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        numberLineEdit = new QLineEdit(Perforce__Internal__ChangeNumberDialog);
        numberLineEdit->setObjectName(QString::fromUtf8("numberLineEdit"));

        gridLayout->addWidget(numberLineEdit, 0, 1, 1, 1);

        label = new QLabel(Perforce__Internal__ChangeNumberDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(Perforce__Internal__ChangeNumberDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 2);


        retranslateUi(Perforce__Internal__ChangeNumberDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Perforce__Internal__ChangeNumberDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Perforce__Internal__ChangeNumberDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Perforce__Internal__ChangeNumberDialog);
    } // setupUi

    void retranslateUi(QDialog *Perforce__Internal__ChangeNumberDialog)
    {
        Perforce__Internal__ChangeNumberDialog->setWindowTitle(QApplication::translate("Perforce::Internal::ChangeNumberDialog", "Change Number", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Perforce::Internal::ChangeNumberDialog", "Change Number:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Perforce__Internal__ChangeNumberDialog);
    } // retranslateUi

};

} // namespace Internal
} // namespace Perforce

namespace Perforce {
namespace Internal {
namespace Ui {
    class ChangeNumberDialog: public Ui_ChangeNumberDialog {};
} // namespace Ui
} // namespace Internal
} // namespace Perforce

#endif // UI_CHANGENUMBERDIALOG_H
