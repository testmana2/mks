/********************************************************************************
** Form generated from reading ui file 'branchdialog.ui'
**
** Created: Fri Mar 6 15:15:52 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BRANCHDIALOG_H
#define UI_BRANCHDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QVBoxLayout>

namespace Git {
namespace Internal {

class Ui_BranchDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *infoGroupBox;
    QFormLayout *formLayout;
    QLabel *repositoryLabel;
    QLabel *repositoryFieldLabel;
    QGroupBox *localBranchGroupBox;
    QVBoxLayout *verticalLayout;
    QListView *localBranchListView;
    QGroupBox *remoteBranchGroupBox;
    QVBoxLayout *verticalLayout_2;
    QListView *remoteBranchListView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Git__Internal__BranchDialog)
    {
        if (Git__Internal__BranchDialog->objectName().isEmpty())
            Git__Internal__BranchDialog->setObjectName(QString::fromUtf8("Git__Internal__BranchDialog"));
        Git__Internal__BranchDialog->resize(544, 631);
        verticalLayout_3 = new QVBoxLayout(Git__Internal__BranchDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        infoGroupBox = new QGroupBox(Git__Internal__BranchDialog);
        infoGroupBox->setObjectName(QString::fromUtf8("infoGroupBox"));
        formLayout = new QFormLayout(infoGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        repositoryLabel = new QLabel(infoGroupBox);
        repositoryLabel->setObjectName(QString::fromUtf8("repositoryLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, repositoryLabel);

        repositoryFieldLabel = new QLabel(infoGroupBox);
        repositoryFieldLabel->setObjectName(QString::fromUtf8("repositoryFieldLabel"));

        formLayout->setWidget(0, QFormLayout::FieldRole, repositoryFieldLabel);


        verticalLayout_3->addWidget(infoGroupBox);

        localBranchGroupBox = new QGroupBox(Git__Internal__BranchDialog);
        localBranchGroupBox->setObjectName(QString::fromUtf8("localBranchGroupBox"));
        verticalLayout = new QVBoxLayout(localBranchGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        localBranchListView = new QListView(localBranchGroupBox);
        localBranchListView->setObjectName(QString::fromUtf8("localBranchListView"));

        verticalLayout->addWidget(localBranchListView);


        verticalLayout_3->addWidget(localBranchGroupBox);

        remoteBranchGroupBox = new QGroupBox(Git__Internal__BranchDialog);
        remoteBranchGroupBox->setObjectName(QString::fromUtf8("remoteBranchGroupBox"));
        verticalLayout_2 = new QVBoxLayout(remoteBranchGroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        remoteBranchListView = new QListView(remoteBranchGroupBox);
        remoteBranchListView->setObjectName(QString::fromUtf8("remoteBranchListView"));

        verticalLayout_2->addWidget(remoteBranchListView);


        verticalLayout_3->addWidget(remoteBranchGroupBox);

        buttonBox = new QDialogButtonBox(Git__Internal__BranchDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(Git__Internal__BranchDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), Git__Internal__BranchDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Git__Internal__BranchDialog);
    } // setupUi

    void retranslateUi(QDialog *Git__Internal__BranchDialog)
    {
        Git__Internal__BranchDialog->setWindowTitle(QApplication::translate("Git::Internal::BranchDialog", "Branches", 0, QApplication::UnicodeUTF8));
        infoGroupBox->setTitle(QApplication::translate("Git::Internal::BranchDialog", "General information", 0, QApplication::UnicodeUTF8));
        repositoryLabel->setText(QApplication::translate("Git::Internal::BranchDialog", "Repository:", 0, QApplication::UnicodeUTF8));
        repositoryFieldLabel->setText(QApplication::translate("Git::Internal::BranchDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        localBranchGroupBox->setTitle(QApplication::translate("Git::Internal::BranchDialog", "Branches", 0, QApplication::UnicodeUTF8));
        remoteBranchGroupBox->setTitle(QApplication::translate("Git::Internal::BranchDialog", "Remote branches", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Git__Internal__BranchDialog);
    } // retranslateUi

};

} // namespace Internal
} // namespace Git

namespace Git {
namespace Internal {
namespace Ui {
    class BranchDialog: public Ui_BranchDialog {};
} // namespace Ui
} // namespace Internal
} // namespace Git

#endif // UI_BRANCHDIALOG_H
