/********************************************************************************
** Form generated from reading ui file 'settingspage.ui'
**
** Created: Fri Mar 6 15:15:38 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETTINGSPAGE_H
#define UI_SETTINGSPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <utils/pathchooser.h>

namespace Subversion {
namespace Internal {

class Ui_SettingsPage
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *commandHBox;
    QLabel *commandLabel;
    Core::Utils::PathChooser *pathChooser;
    QGroupBox *userGroupBox;
    QFormLayout *formLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Subversion__Internal__SettingsPage)
    {
        if (Subversion__Internal__SettingsPage->objectName().isEmpty())
            Subversion__Internal__SettingsPage->setObjectName(QString::fromUtf8("Subversion__Internal__SettingsPage"));
        Subversion__Internal__SettingsPage->resize(427, 280);
        horizontalLayout = new QHBoxLayout(Subversion__Internal__SettingsPage);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        commandHBox = new QHBoxLayout();
#ifndef Q_OS_MAC
        commandHBox->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        commandHBox->setMargin(0);
#endif
        commandHBox->setObjectName(QString::fromUtf8("commandHBox"));
        commandLabel = new QLabel(Subversion__Internal__SettingsPage);
        commandLabel->setObjectName(QString::fromUtf8("commandLabel"));

        commandHBox->addWidget(commandLabel);

        pathChooser = new Core::Utils::PathChooser(Subversion__Internal__SettingsPage);
        pathChooser->setObjectName(QString::fromUtf8("pathChooser"));

        commandHBox->addWidget(pathChooser);


        verticalLayout->addLayout(commandHBox);

        userGroupBox = new QGroupBox(Subversion__Internal__SettingsPage);
        userGroupBox->setObjectName(QString::fromUtf8("userGroupBox"));
        userGroupBox->setCheckable(true);
        formLayout = new QFormLayout(userGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        usernameLabel = new QLabel(userGroupBox);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, usernameLabel);

        usernameLineEdit = new QLineEdit(userGroupBox);
        usernameLineEdit->setObjectName(QString::fromUtf8("usernameLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, usernameLineEdit);

        passwordLabel = new QLabel(userGroupBox);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, passwordLabel);

        passwordLineEdit = new QLineEdit(userGroupBox);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, passwordLineEdit);


        verticalLayout->addWidget(userGroupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(105, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(Subversion__Internal__SettingsPage);

        QMetaObject::connectSlotsByName(Subversion__Internal__SettingsPage);
    } // setupUi

    void retranslateUi(QWidget *Subversion__Internal__SettingsPage)
    {
        Subversion__Internal__SettingsPage->setWindowTitle(QApplication::translate("Subversion::Internal::SettingsPage", "Form", 0, QApplication::UnicodeUTF8));
        commandLabel->setText(QApplication::translate("Subversion::Internal::SettingsPage", "Subversion Command:", 0, QApplication::UnicodeUTF8));
        userGroupBox->setTitle(QApplication::translate("Subversion::Internal::SettingsPage", "Authentication", 0, QApplication::UnicodeUTF8));
        usernameLabel->setText(QApplication::translate("Subversion::Internal::SettingsPage", "User name:", 0, QApplication::UnicodeUTF8));
        passwordLabel->setText(QApplication::translate("Subversion::Internal::SettingsPage", "Password:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Subversion__Internal__SettingsPage);
    } // retranslateUi

};

} // namespace Internal
} // namespace Subversion

namespace Subversion {
namespace Internal {
namespace Ui {
    class SettingsPage: public Ui_SettingsPage {};
} // namespace Ui
} // namespace Internal
} // namespace Subversion

#endif // UI_SETTINGSPAGE_H
