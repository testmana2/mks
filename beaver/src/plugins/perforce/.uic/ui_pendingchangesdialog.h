/********************************************************************************
** Form generated from reading ui file 'pendingchangesdialog.ui'
**
** Created: Fri Mar 6 15:15:17 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PENDINGCHANGESDIALOG_H
#define UI_PENDINGCHANGESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

namespace Perforce {
namespace Internal {

class Ui_PendingChangesDialog
{
public:
    QVBoxLayout *vboxLayout;
    QListWidget *listWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *submitButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Perforce__Internal__PendingChangesDialog)
    {
        if (Perforce__Internal__PendingChangesDialog->objectName().isEmpty())
            Perforce__Internal__PendingChangesDialog->setObjectName(QString::fromUtf8("Perforce__Internal__PendingChangesDialog"));
        Perforce__Internal__PendingChangesDialog->resize(333, 126);
        vboxLayout = new QVBoxLayout(Perforce__Internal__PendingChangesDialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        listWidget = new QListWidget(Perforce__Internal__PendingChangesDialog);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        vboxLayout->addWidget(listWidget);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        submitButton = new QPushButton(Perforce__Internal__PendingChangesDialog);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));

        hboxLayout->addWidget(submitButton);

        cancelButton = new QPushButton(Perforce__Internal__PendingChangesDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(Perforce__Internal__PendingChangesDialog);
        QObject::connect(submitButton, SIGNAL(clicked()), Perforce__Internal__PendingChangesDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Perforce__Internal__PendingChangesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Perforce__Internal__PendingChangesDialog);
    } // setupUi

    void retranslateUi(QDialog *Perforce__Internal__PendingChangesDialog)
    {
        Perforce__Internal__PendingChangesDialog->setWindowTitle(QApplication::translate("Perforce::Internal::PendingChangesDialog", "P4 Pending Changes", 0, QApplication::UnicodeUTF8));
        submitButton->setText(QApplication::translate("Perforce::Internal::PendingChangesDialog", "Submit", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Perforce::Internal::PendingChangesDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Perforce__Internal__PendingChangesDialog);
    } // retranslateUi

};

} // namespace Internal
} // namespace Perforce

namespace Perforce {
namespace Internal {
namespace Ui {
    class PendingChangesDialog: public Ui_PendingChangesDialog {};
} // namespace Ui
} // namespace Internal
} // namespace Perforce

#endif // UI_PENDINGCHANGESDIALOG_H
