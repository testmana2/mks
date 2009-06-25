/********************************************************************************
** Form generated from reading ui file 'envvariablespage.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ENVVARIABLESPAGE_H
#define UI_ENVVARIABLESPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_EnvVariablesPage
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout;
    QListWidget *listView;
    QVBoxLayout *vboxLayout1;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *delButton;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *mkspecEdit;
    QLineEdit *defaultCmdEdit;

    void setupUi(QWidget *Qt4ProjectManager__Internal__EnvVariablesPage)
    {
        if (Qt4ProjectManager__Internal__EnvVariablesPage->objectName().isEmpty())
            Qt4ProjectManager__Internal__EnvVariablesPage->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__EnvVariablesPage"));
        Qt4ProjectManager__Internal__EnvVariablesPage->resize(398, 336);
        vboxLayout = new QVBoxLayout(Qt4ProjectManager__Internal__EnvVariablesPage);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        groupBox = new QGroupBox(Qt4ProjectManager__Internal__EnvVariablesPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setEnabled(true);
        hboxLayout = new QHBoxLayout(groupBox);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setMargin(9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        listView = new QListWidget(groupBox);
        listView->setObjectName(QString::fromUtf8("listView"));

        hboxLayout->addWidget(listView);

        vboxLayout1 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
        vboxLayout1->setMargin(0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        addButton = new QPushButton(groupBox);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        vboxLayout1->addWidget(addButton);

        editButton = new QPushButton(groupBox);
        editButton->setObjectName(QString::fromUtf8("editButton"));

        vboxLayout1->addWidget(editButton);

        delButton = new QPushButton(groupBox);
        delButton->setObjectName(QString::fromUtf8("delButton"));

        vboxLayout1->addWidget(delButton);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout1->addItem(spacerItem);


        hboxLayout->addLayout(vboxLayout1);


        vboxLayout->addWidget(groupBox);

        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(Qt4ProjectManager__Internal__EnvVariablesPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(Qt4ProjectManager__Internal__EnvVariablesPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        mkspecEdit = new QLineEdit(Qt4ProjectManager__Internal__EnvVariablesPage);
        mkspecEdit->setObjectName(QString::fromUtf8("mkspecEdit"));

        gridLayout->addWidget(mkspecEdit, 1, 1, 1, 1);

        defaultCmdEdit = new QLineEdit(Qt4ProjectManager__Internal__EnvVariablesPage);
        defaultCmdEdit->setObjectName(QString::fromUtf8("defaultCmdEdit"));

        gridLayout->addWidget(defaultCmdEdit, 0, 1, 1, 1);


        vboxLayout->addLayout(gridLayout);


        retranslateUi(Qt4ProjectManager__Internal__EnvVariablesPage);

        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__EnvVariablesPage);
    } // setupUi

    void retranslateUi(QWidget *Qt4ProjectManager__Internal__EnvVariablesPage)
    {
        Qt4ProjectManager__Internal__EnvVariablesPage->setWindowTitle(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Build Environments", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Add...", 0, QApplication::UnicodeUTF8));
        editButton->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Edit...", 0, QApplication::UnicodeUTF8));
        delButton->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Delete", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Default mkspec:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Qt4ProjectManager::Internal::EnvVariablesPage", "Default make command:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__EnvVariablesPage);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class EnvVariablesPage: public Ui_EnvVariablesPage {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_ENVVARIABLESPAGE_H
