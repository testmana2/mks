/********************************************************************************
** Form generated from reading ui file 'settingspage.ui'
**
** Created: Fri Mar 6 15:15:52 2009
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
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Git {
namespace Internal {

class Ui_SettingsPage
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *environmentGroupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *pathlabel;
    QLineEdit *pathLineEdit;
    QPushButton *adoptButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *noteLabel;
    QLabel *noteFieldlabel;
    QFormLayout *formLayout;
    QLabel *logCountLabel;
    QSpinBox *logCountSpinBox;
    QLabel *timeoutLabel;
    QSpinBox *timeoutSpinBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Git__Internal__SettingsPage)
    {
        if (Git__Internal__SettingsPage->objectName().isEmpty())
            Git__Internal__SettingsPage->setObjectName(QString::fromUtf8("Git__Internal__SettingsPage"));
        Git__Internal__SettingsPage->resize(403, 251);
        verticalLayout = new QVBoxLayout(Git__Internal__SettingsPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        environmentGroupBox = new QGroupBox(Git__Internal__SettingsPage);
        environmentGroupBox->setObjectName(QString::fromUtf8("environmentGroupBox"));
        environmentGroupBox->setEnabled(true);
        environmentGroupBox->setCheckable(true);
        verticalLayout_2 = new QVBoxLayout(environmentGroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pathlabel = new QLabel(environmentGroupBox);
        pathlabel->setObjectName(QString::fromUtf8("pathlabel"));

        horizontalLayout->addWidget(pathlabel);

        pathLineEdit = new QLineEdit(environmentGroupBox);
        pathLineEdit->setObjectName(QString::fromUtf8("pathLineEdit"));

        horizontalLayout->addWidget(pathLineEdit);

        adoptButton = new QPushButton(environmentGroupBox);
        adoptButton->setObjectName(QString::fromUtf8("adoptButton"));

        horizontalLayout->addWidget(adoptButton);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        noteLabel = new QLabel(environmentGroupBox);
        noteLabel->setObjectName(QString::fromUtf8("noteLabel"));

        horizontalLayout_2->addWidget(noteLabel);

        noteFieldlabel = new QLabel(environmentGroupBox);
        noteFieldlabel->setObjectName(QString::fromUtf8("noteFieldlabel"));

        horizontalLayout_2->addWidget(noteFieldlabel);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(environmentGroupBox);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        logCountLabel = new QLabel(Git__Internal__SettingsPage);
        logCountLabel->setObjectName(QString::fromUtf8("logCountLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, logCountLabel);

        logCountSpinBox = new QSpinBox(Git__Internal__SettingsPage);
        logCountSpinBox->setObjectName(QString::fromUtf8("logCountSpinBox"));
        logCountSpinBox->setMaximum(1000);

        formLayout->setWidget(0, QFormLayout::FieldRole, logCountSpinBox);

        timeoutLabel = new QLabel(Git__Internal__SettingsPage);
        timeoutLabel->setObjectName(QString::fromUtf8("timeoutLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, timeoutLabel);

        timeoutSpinBox = new QSpinBox(Git__Internal__SettingsPage);
        timeoutSpinBox->setObjectName(QString::fromUtf8("timeoutSpinBox"));
        timeoutSpinBox->setMinimum(10);
        timeoutSpinBox->setMaximum(300);
        timeoutSpinBox->setValue(30);

        formLayout->setWidget(1, QFormLayout::FieldRole, timeoutSpinBox);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(Git__Internal__SettingsPage);

        QMetaObject::connectSlotsByName(Git__Internal__SettingsPage);
    } // setupUi

    void retranslateUi(QWidget *Git__Internal__SettingsPage)
    {
        Git__Internal__SettingsPage->setWindowTitle(QApplication::translate("Git::Internal::SettingsPage", "Form", 0, QApplication::UnicodeUTF8));
        environmentGroupBox->setTitle(QApplication::translate("Git::Internal::SettingsPage", "Environment variables", 0, QApplication::UnicodeUTF8));
        pathlabel->setText(QApplication::translate("Git::Internal::SettingsPage", "PATH:", 0, QApplication::UnicodeUTF8));
        adoptButton->setText(QApplication::translate("Git::Internal::SettingsPage", "From system", 0, QApplication::UnicodeUTF8));
        noteLabel->setText(QApplication::translate("Git::Internal::SettingsPage", "<b>Note:</b>", 0, QApplication::UnicodeUTF8));
        noteFieldlabel->setText(QApplication::translate("Git::Internal::SettingsPage", "Git needs to find Perl in the environment as well.", 0, QApplication::UnicodeUTF8));
        logCountLabel->setText(QApplication::translate("Git::Internal::SettingsPage", "Log commit display count:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        logCountSpinBox->setToolTip(QApplication::translate("Git::Internal::SettingsPage", "Note that huge amount of commits might take some time.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeoutLabel->setText(QApplication::translate("Git::Internal::SettingsPage", "Timeout (seconds):", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Git__Internal__SettingsPage);
    } // retranslateUi

};

} // namespace Internal
} // namespace Git

namespace Git {
namespace Internal {
namespace Ui {
    class SettingsPage: public Ui_SettingsPage {};
} // namespace Ui
} // namespace Internal
} // namespace Git

#endif // UI_SETTINGSPAGE_H
