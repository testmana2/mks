/********************************************************************************
** Form generated from reading ui file 'settingspage.ui'
**
** Created: Fri Mar 6 15:15:17 2009
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
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <utils/pathchooser.h>

namespace Perforce {
namespace Internal {

class Ui_SettingsPage
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_4;
    Core::Utils::PathChooser *pathChooser;
    QCheckBox *defaultCheckBox;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *clientLineEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QLineEdit *userLineEdit;
    QLineEdit *portLineEdit;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *Perforce__Internal__SettingsPage)
    {
        if (Perforce__Internal__SettingsPage->objectName().isEmpty())
            Perforce__Internal__SettingsPage->setObjectName(QString::fromUtf8("Perforce__Internal__SettingsPage"));
        Perforce__Internal__SettingsPage->resize(276, 198);
        vboxLayout = new QVBoxLayout(Perforce__Internal__SettingsPage);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setMargin(0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label_4 = new QLabel(Perforce__Internal__SettingsPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        hboxLayout->addWidget(label_4);

        pathChooser = new Core::Utils::PathChooser(Perforce__Internal__SettingsPage);
        pathChooser->setObjectName(QString::fromUtf8("pathChooser"));

        hboxLayout->addWidget(pathChooser);


        vboxLayout->addLayout(hboxLayout);

        defaultCheckBox = new QCheckBox(Perforce__Internal__SettingsPage);
        defaultCheckBox->setObjectName(QString::fromUtf8("defaultCheckBox"));

        vboxLayout->addWidget(defaultCheckBox);

        groupBox = new QGroupBox(Perforce__Internal__SettingsPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setCheckable(false);
        gridLayout = new QGridLayout(groupBox);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        clientLineEdit = new QLineEdit(groupBox);
        clientLineEdit->setObjectName(QString::fromUtf8("clientLineEdit"));

        gridLayout->addWidget(clientLineEdit, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        userLineEdit = new QLineEdit(groupBox);
        userLineEdit->setObjectName(QString::fromUtf8("userLineEdit"));

        gridLayout->addWidget(userLineEdit, 2, 1, 1, 1);

        portLineEdit = new QLineEdit(groupBox);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));

        gridLayout->addWidget(portLineEdit, 0, 1, 1, 1);


        vboxLayout->addWidget(groupBox);

        spacerItem = new QSpacerItem(141, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        QWidget::setTabOrder(portLineEdit, clientLineEdit);
        QWidget::setTabOrder(clientLineEdit, userLineEdit);

        retranslateUi(Perforce__Internal__SettingsPage);
        QObject::connect(defaultCheckBox, SIGNAL(toggled(bool)), groupBox, SLOT(setDisabled(bool)));

        QMetaObject::connectSlotsByName(Perforce__Internal__SettingsPage);
    } // setupUi

    void retranslateUi(QWidget *Perforce__Internal__SettingsPage)
    {
        Perforce__Internal__SettingsPage->setWindowTitle(QApplication::translate("Perforce::Internal::SettingsPage", "Form", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Perforce::Internal::SettingsPage", "P4 Command:", 0, QApplication::UnicodeUTF8));
        defaultCheckBox->setText(QApplication::translate("Perforce::Internal::SettingsPage", "Use default P4 environment variables", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Perforce::Internal::SettingsPage", "Environment variables", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Perforce::Internal::SettingsPage", "P4 Client:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Perforce::Internal::SettingsPage", "P4 User:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Perforce::Internal::SettingsPage", "P4 Port:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Perforce__Internal__SettingsPage);
    } // retranslateUi

};

} // namespace Internal
} // namespace Perforce

namespace Perforce {
namespace Internal {
namespace Ui {
    class SettingsPage: public Ui_SettingsPage {};
} // namespace Ui
} // namespace Internal
} // namespace Perforce

#endif // UI_SETTINGSPAGE_H
