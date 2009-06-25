/********************************************************************************
** Form generated from reading ui file 'qt4buildenvironmentwidget.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QT4BUILDENVIRONMENTWIDGET_H
#define UI_QT4BUILDENVIRONMENTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_Qt4BuildEnvironmentWidget
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *clearSystemEnvironmentCheckBox;
    QHBoxLayout *horizontalLayout;
    QTreeView *environmentTreeView;
    QVBoxLayout *verticalLayout_2;
    QPushButton *editButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *unsetButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget)
    {
        if (Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget->objectName().isEmpty())
            Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget"));
        Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        clearSystemEnvironmentCheckBox = new QCheckBox(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        clearSystemEnvironmentCheckBox->setObjectName(QString::fromUtf8("clearSystemEnvironmentCheckBox"));

        verticalLayout->addWidget(clearSystemEnvironmentCheckBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        environmentTreeView = new QTreeView(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        environmentTreeView->setObjectName(QString::fromUtf8("environmentTreeView"));
        environmentTreeView->setRootIsDecorated(false);
        environmentTreeView->setHeaderHidden(false);

        horizontalLayout->addWidget(environmentTreeView);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        editButton = new QPushButton(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        editButton->setObjectName(QString::fromUtf8("editButton"));

        verticalLayout_2->addWidget(editButton);

        addButton = new QPushButton(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        verticalLayout_2->addWidget(addButton);

        removeButton = new QPushButton(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setEnabled(false);

        verticalLayout_2->addWidget(removeButton);

        unsetButton = new QPushButton(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
        unsetButton->setObjectName(QString::fromUtf8("unsetButton"));
        unsetButton->setEnabled(false);

        verticalLayout_2->addWidget(unsetButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);

        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
    } // setupUi

    void retranslateUi(QWidget *Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget)
    {
        Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget->setWindowTitle(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildEnvironmentWidget", "Form", 0, QApplication::UnicodeUTF8));
        clearSystemEnvironmentCheckBox->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildEnvironmentWidget", "Clear system environment", 0, QApplication::UnicodeUTF8));
        editButton->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildEnvironmentWidget", "&Edit", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildEnvironmentWidget", "&Add", 0, QApplication::UnicodeUTF8));
        removeButton->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildEnvironmentWidget", "&Reset", 0, QApplication::UnicodeUTF8));
        unsetButton->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildEnvironmentWidget", "&Unset", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__Qt4BuildEnvironmentWidget);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class Qt4BuildEnvironmentWidget: public Ui_Qt4BuildEnvironmentWidget {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_QT4BUILDENVIRONMENTWIDGET_H
