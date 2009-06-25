/********************************************************************************
** Form generated from reading ui file 'qtversionmanager.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QTVERSIONMANAGER_H
#define UI_QTVERSIONMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <utils/pathchooser.h>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_QtVersionManager
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QVBoxLayout *vboxLayout;
    QToolButton *addButton;
    QToolButton *delButton;
    QSpacerItem *spacerItem;
    QTreeWidget *qtdirList;
    QLabel *versionNameLabel;
    QLineEdit *nameEdit;
    QLabel *pathLabel;
    QLabel *mingwLabel;
    QLabel *msvcLabel;
    QLabel *errorLabel;
    QComboBox *msvcComboBox;
    Core::Utils::PathChooser *qtPath;
    Core::Utils::PathChooser *mingwPath;
    QHBoxLayout *horizontalLayout;
    QLabel *defaultLabel;
    QComboBox *defaultCombo;

    void setupUi(QWidget *Qt4ProjectManager__Internal__QtVersionManager)
    {
        if (Qt4ProjectManager__Internal__QtVersionManager->objectName().isEmpty())
            Qt4ProjectManager__Internal__QtVersionManager->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__QtVersionManager"));
        Qt4ProjectManager__Internal__QtVersionManager->resize(823, 929);
        verticalLayout = new QVBoxLayout(Qt4ProjectManager__Internal__QtVersionManager);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(Qt4ProjectManager__Internal__QtVersionManager);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(0);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        addButton = new QToolButton(groupBox);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setMinimumSize(QSize(21, 23));

        vboxLayout->addWidget(addButton);

        delButton = new QToolButton(groupBox);
        delButton->setObjectName(QString::fromUtf8("delButton"));
        delButton->setMinimumSize(QSize(21, 23));

        vboxLayout->addWidget(delButton);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);


        gridLayout_2->addLayout(vboxLayout, 0, 3, 1, 1);

        qtdirList = new QTreeWidget(groupBox);
        qtdirList->setObjectName(QString::fromUtf8("qtdirList"));
        qtdirList->setUniformRowHeights(true);
        qtdirList->setItemsExpandable(false);
        qtdirList->setColumnCount(2);

        gridLayout_2->addWidget(qtdirList, 0, 0, 1, 3);

        versionNameLabel = new QLabel(groupBox);
        versionNameLabel->setObjectName(QString::fromUtf8("versionNameLabel"));

        gridLayout_2->addWidget(versionNameLabel, 1, 0, 1, 1);

        nameEdit = new QLineEdit(groupBox);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        gridLayout_2->addWidget(nameEdit, 1, 1, 1, 2);

        pathLabel = new QLabel(groupBox);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));

        gridLayout_2->addWidget(pathLabel, 2, 0, 1, 1);

        mingwLabel = new QLabel(groupBox);
        mingwLabel->setObjectName(QString::fromUtf8("mingwLabel"));

        gridLayout_2->addWidget(mingwLabel, 3, 0, 1, 1);

        msvcLabel = new QLabel(groupBox);
        msvcLabel->setObjectName(QString::fromUtf8("msvcLabel"));

        gridLayout_2->addWidget(msvcLabel, 4, 0, 1, 3);

        errorLabel = new QLabel(groupBox);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));

        gridLayout_2->addWidget(errorLabel, 7, 0, 1, 4);

        msvcComboBox = new QComboBox(groupBox);
        msvcComboBox->setObjectName(QString::fromUtf8("msvcComboBox"));

        gridLayout_2->addWidget(msvcComboBox, 5, 1, 1, 2);

        qtPath = new Core::Utils::PathChooser(groupBox);
        qtPath->setObjectName(QString::fromUtf8("qtPath"));

        gridLayout_2->addWidget(qtPath, 2, 1, 1, 2);

        mingwPath = new Core::Utils::PathChooser(groupBox);
        mingwPath->setObjectName(QString::fromUtf8("mingwPath"));

        gridLayout_2->addWidget(mingwPath, 3, 1, 1, 2);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        defaultLabel = new QLabel(Qt4ProjectManager__Internal__QtVersionManager);
        defaultLabel->setObjectName(QString::fromUtf8("defaultLabel"));

        horizontalLayout->addWidget(defaultLabel);

        defaultCombo = new QComboBox(Qt4ProjectManager__Internal__QtVersionManager);
        defaultCombo->setObjectName(QString::fromUtf8("defaultCombo"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(defaultCombo->sizePolicy().hasHeightForWidth());
        defaultCombo->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(defaultCombo);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(qtdirList, delButton);
        QWidget::setTabOrder(delButton, nameEdit);
        QWidget::setTabOrder(nameEdit, defaultCombo);

        retranslateUi(Qt4ProjectManager__Internal__QtVersionManager);

        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__QtVersionManager);
    } // setupUi

    void retranslateUi(QWidget *Qt4ProjectManager__Internal__QtVersionManager)
    {
        Qt4ProjectManager__Internal__QtVersionManager->setWindowTitle(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Qt versions", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "+", 0, QApplication::UnicodeUTF8));
        delButton->setText(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "-", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = qtdirList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Path", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Name", 0, QApplication::UnicodeUTF8));
        versionNameLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Version Name:", 0, QApplication::UnicodeUTF8));
        pathLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Path:", 0, QApplication::UnicodeUTF8));
        mingwLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "MinGw Directory:", 0, QApplication::UnicodeUTF8));
        msvcLabel->setText(QString());
        errorLabel->setText(QString());
        defaultLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::QtVersionManager", "Default Qt Version:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__QtVersionManager);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class QtVersionManager: public Ui_QtVersionManager {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_QTVERSIONMANAGER_H
