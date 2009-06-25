/********************************************************************************
** Form generated from reading ui file 'gitsubmitpanel.ui'
**
** Created: Fri Mar 6 15:15:52 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GITSUBMITPANEL_H
#define UI_GITSUBMITPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Git {
namespace Internal {

class Ui_GitSubmitPanel
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *infoGroup;
    QFormLayout *infoFormLayout;
    QLabel *repositoryLabelLabel;
    QLabel *repositoryLabel;
    QLabel *branchLabelLabel;
    QLabel *branchLabel;
    QGroupBox *editGroup;
    QFormLayout *editFormLayout;
    QLabel *authorLabel;
    QLineEdit *authorLineEdit;
    QLabel *emailLabel;
    QLineEdit *emailLineEdit;

    void setupUi(QWidget *Git__Internal__GitSubmitPanel)
    {
        if (Git__Internal__GitSubmitPanel->objectName().isEmpty())
            Git__Internal__GitSubmitPanel->setObjectName(QString::fromUtf8("Git__Internal__GitSubmitPanel"));
        Git__Internal__GitSubmitPanel->resize(201, 189);
        verticalLayout = new QVBoxLayout(Git__Internal__GitSubmitPanel);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        infoGroup = new QGroupBox(Git__Internal__GitSubmitPanel);
        infoGroup->setObjectName(QString::fromUtf8("infoGroup"));
        infoFormLayout = new QFormLayout(infoGroup);
        infoFormLayout->setObjectName(QString::fromUtf8("infoFormLayout"));
        infoFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        repositoryLabelLabel = new QLabel(infoGroup);
        repositoryLabelLabel->setObjectName(QString::fromUtf8("repositoryLabelLabel"));

        infoFormLayout->setWidget(0, QFormLayout::LabelRole, repositoryLabelLabel);

        repositoryLabel = new QLabel(infoGroup);
        repositoryLabel->setObjectName(QString::fromUtf8("repositoryLabel"));

        infoFormLayout->setWidget(0, QFormLayout::FieldRole, repositoryLabel);

        branchLabelLabel = new QLabel(infoGroup);
        branchLabelLabel->setObjectName(QString::fromUtf8("branchLabelLabel"));

        infoFormLayout->setWidget(1, QFormLayout::LabelRole, branchLabelLabel);

        branchLabel = new QLabel(infoGroup);
        branchLabel->setObjectName(QString::fromUtf8("branchLabel"));

        infoFormLayout->setWidget(1, QFormLayout::FieldRole, branchLabel);


        verticalLayout->addWidget(infoGroup);

        editGroup = new QGroupBox(Git__Internal__GitSubmitPanel);
        editGroup->setObjectName(QString::fromUtf8("editGroup"));
        editFormLayout = new QFormLayout(editGroup);
        editFormLayout->setObjectName(QString::fromUtf8("editFormLayout"));
        authorLabel = new QLabel(editGroup);
        authorLabel->setObjectName(QString::fromUtf8("authorLabel"));

        editFormLayout->setWidget(0, QFormLayout::LabelRole, authorLabel);

        authorLineEdit = new QLineEdit(editGroup);
        authorLineEdit->setObjectName(QString::fromUtf8("authorLineEdit"));

        editFormLayout->setWidget(0, QFormLayout::FieldRole, authorLineEdit);

        emailLabel = new QLabel(editGroup);
        emailLabel->setObjectName(QString::fromUtf8("emailLabel"));

        editFormLayout->setWidget(1, QFormLayout::LabelRole, emailLabel);

        emailLineEdit = new QLineEdit(editGroup);
        emailLineEdit->setObjectName(QString::fromUtf8("emailLineEdit"));

        editFormLayout->setWidget(1, QFormLayout::FieldRole, emailLineEdit);


        verticalLayout->addWidget(editGroup);


        retranslateUi(Git__Internal__GitSubmitPanel);

        QMetaObject::connectSlotsByName(Git__Internal__GitSubmitPanel);
    } // setupUi

    void retranslateUi(QWidget *Git__Internal__GitSubmitPanel)
    {
        infoGroup->setTitle(QApplication::translate("Git::Internal::GitSubmitPanel", "General Information", 0, QApplication::UnicodeUTF8));
        repositoryLabelLabel->setText(QApplication::translate("Git::Internal::GitSubmitPanel", "Repository:", 0, QApplication::UnicodeUTF8));
        repositoryLabel->setText(QApplication::translate("Git::Internal::GitSubmitPanel", "repository", 0, QApplication::UnicodeUTF8));
        branchLabelLabel->setText(QApplication::translate("Git::Internal::GitSubmitPanel", "Branch:", 0, QApplication::UnicodeUTF8));
        branchLabel->setText(QApplication::translate("Git::Internal::GitSubmitPanel", "branch", 0, QApplication::UnicodeUTF8));
        editGroup->setTitle(QApplication::translate("Git::Internal::GitSubmitPanel", "Commit Information", 0, QApplication::UnicodeUTF8));
        authorLabel->setText(QApplication::translate("Git::Internal::GitSubmitPanel", "Author:", 0, QApplication::UnicodeUTF8));
        emailLabel->setText(QApplication::translate("Git::Internal::GitSubmitPanel", "Email:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Git__Internal__GitSubmitPanel);
    } // retranslateUi

};

} // namespace Internal
} // namespace Git

namespace Git {
namespace Internal {
namespace Ui {
    class GitSubmitPanel: public Ui_GitSubmitPanel {};
} // namespace Ui
} // namespace Internal
} // namespace Git

#endif // UI_GITSUBMITPANEL_H
