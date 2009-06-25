/********************************************************************************
** Form generated from reading ui file 'cmakeconfigurewidget.ui'
**
** Created: Fri Mar 6 15:19:33 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CMAKECONFIGUREWIDGET_H
#define UI_CMAKECONFIGUREWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <utils/pathchooser.h>

namespace CMakeProjectManager {
namespace Internal {

class Ui_CMakeConfigureWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelCommandArguments;
    QLineEdit *cmakeArgumentsLineEdit;
    QLabel *label_2;
    Core::Utils::PathChooser *buildDirectoryLineEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *configureButton;
    QPlainTextEdit *cmakeOutput;

    void setupUi(QWidget *CMakeProjectManager__Internal__CMakeConfigureWidget)
    {
        if (CMakeProjectManager__Internal__CMakeConfigureWidget->objectName().isEmpty())
            CMakeProjectManager__Internal__CMakeConfigureWidget->setObjectName(QString::fromUtf8("CMakeProjectManager__Internal__CMakeConfigureWidget"));
        CMakeProjectManager__Internal__CMakeConfigureWidget->resize(521, 662);
        verticalLayout = new QVBoxLayout(CMakeProjectManager__Internal__CMakeConfigureWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelCommandArguments = new QLabel(CMakeProjectManager__Internal__CMakeConfigureWidget);
        labelCommandArguments->setObjectName(QString::fromUtf8("labelCommandArguments"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelCommandArguments);

        cmakeArgumentsLineEdit = new QLineEdit(CMakeProjectManager__Internal__CMakeConfigureWidget);
        cmakeArgumentsLineEdit->setObjectName(QString::fromUtf8("cmakeArgumentsLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cmakeArgumentsLineEdit);

        label_2 = new QLabel(CMakeProjectManager__Internal__CMakeConfigureWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        buildDirectoryLineEdit = new Core::Utils::PathChooser(CMakeProjectManager__Internal__CMakeConfigureWidget);
        buildDirectoryLineEdit->setObjectName(QString::fromUtf8("buildDirectoryLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, buildDirectoryLineEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        configureButton = new QPushButton(CMakeProjectManager__Internal__CMakeConfigureWidget);
        configureButton->setObjectName(QString::fromUtf8("configureButton"));

        horizontalLayout->addWidget(configureButton);


        verticalLayout->addLayout(horizontalLayout);

        cmakeOutput = new QPlainTextEdit(CMakeProjectManager__Internal__CMakeConfigureWidget);
        cmakeOutput->setObjectName(QString::fromUtf8("cmakeOutput"));

        verticalLayout->addWidget(cmakeOutput);


        retranslateUi(CMakeProjectManager__Internal__CMakeConfigureWidget);

        QMetaObject::connectSlotsByName(CMakeProjectManager__Internal__CMakeConfigureWidget);
    } // setupUi

    void retranslateUi(QWidget *CMakeProjectManager__Internal__CMakeConfigureWidget)
    {
        CMakeProjectManager__Internal__CMakeConfigureWidget->setWindowTitle(QApplication::translate("CMakeProjectManager::Internal::CMakeConfigureWidget", "Form", 0, QApplication::UnicodeUTF8));
        labelCommandArguments->setText(QApplication::translate("CMakeProjectManager::Internal::CMakeConfigureWidget", "CMake Arguments:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CMakeProjectManager::Internal::CMakeConfigureWidget", "Builddirectory:", 0, QApplication::UnicodeUTF8));
        configureButton->setText(QApplication::translate("CMakeProjectManager::Internal::CMakeConfigureWidget", "Run cmake", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CMakeProjectManager__Internal__CMakeConfigureWidget);
    } // retranslateUi

};

} // namespace Internal
} // namespace CMakeProjectManager

namespace CMakeProjectManager {
namespace Internal {
namespace Ui {
    class CMakeConfigureWidget: public Ui_CMakeConfigureWidget {};
} // namespace Ui
} // namespace Internal
} // namespace CMakeProjectManager

#endif // UI_CMAKECONFIGUREWIDGET_H
