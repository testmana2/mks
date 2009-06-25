/********************************************************************************
** Form generated from reading ui file 'qt4buildconfigwidget.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QT4BUILDCONFIGWIDGET_H
#define UI_QT4BUILDCONFIGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include <utils/pathchooser.h>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_Qt4BuildConfigWidget
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *qtVersionLabel;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *qtVersionComboBox;
    QPushButton *manageQtVersionPushButtons;
    QLabel *invalidQtWarningLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QCheckBox *shadowBuildCheckBox;
    QLabel *buildDirLabel;
    QLabel *importLabel;
    Core::Utils::PathChooser *shadowBuildDirEdit;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Qt4ProjectManager__Internal__Qt4BuildConfigWidget)
    {
        if (Qt4ProjectManager__Internal__Qt4BuildConfigWidget->objectName().isEmpty())
            Qt4ProjectManager__Internal__Qt4BuildConfigWidget->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__Qt4BuildConfigWidget"));
        Qt4ProjectManager__Internal__Qt4BuildConfigWidget->resize(652, 247);
        gridLayout = new QGridLayout(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        nameLabel = new QLabel(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, nameLabel);

        nameLineEdit = new QLineEdit(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(nameLineEdit->sizePolicy().hasHeightForWidth());
        nameLineEdit->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        qtVersionLabel = new QLabel(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        qtVersionLabel->setObjectName(QString::fromUtf8("qtVersionLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qtVersionLabel->sizePolicy().hasHeightForWidth());
        qtVersionLabel->setSizePolicy(sizePolicy1);

        formLayout->setWidget(1, QFormLayout::LabelRole, qtVersionLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        qtVersionComboBox = new QComboBox(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        qtVersionComboBox->setObjectName(QString::fromUtf8("qtVersionComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(qtVersionComboBox->sizePolicy().hasHeightForWidth());
        qtVersionComboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(qtVersionComboBox);

        manageQtVersionPushButtons = new QPushButton(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        manageQtVersionPushButtons->setObjectName(QString::fromUtf8("manageQtVersionPushButtons"));

        horizontalLayout_2->addWidget(manageQtVersionPushButtons);

        invalidQtWarningLabel = new QLabel(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        invalidQtWarningLabel->setObjectName(QString::fromUtf8("invalidQtWarningLabel"));

        horizontalLayout_2->addWidget(invalidQtWarningLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_2);

        label = new QLabel(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label);

        shadowBuildCheckBox = new QCheckBox(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        shadowBuildCheckBox->setObjectName(QString::fromUtf8("shadowBuildCheckBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, shadowBuildCheckBox);

        buildDirLabel = new QLabel(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        buildDirLabel->setObjectName(QString::fromUtf8("buildDirLabel"));
        sizePolicy2.setHeightForWidth(buildDirLabel->sizePolicy().hasHeightForWidth());
        buildDirLabel->setSizePolicy(sizePolicy2);

        formLayout->setWidget(3, QFormLayout::LabelRole, buildDirLabel);

        importLabel = new QLabel(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        importLabel->setObjectName(QString::fromUtf8("importLabel"));
        importLabel->setTextFormat(Qt::RichText);

        formLayout->setWidget(4, QFormLayout::FieldRole, importLabel);

        shadowBuildDirEdit = new Core::Utils::PathChooser(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
        shadowBuildDirEdit->setObjectName(QString::fromUtf8("shadowBuildDirEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(shadowBuildDirEdit->sizePolicy().hasHeightForWidth());
        shadowBuildDirEdit->setSizePolicy(sizePolicy3);

        formLayout->setWidget(3, QFormLayout::FieldRole, shadowBuildDirEdit);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);

        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
    } // setupUi

    void retranslateUi(QWidget *Qt4ProjectManager__Internal__Qt4BuildConfigWidget)
    {
        Qt4ProjectManager__Internal__Qt4BuildConfigWidget->setWindowTitle(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "Form", 0, QApplication::UnicodeUTF8));
        nameLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "Configuration Name:", 0, QApplication::UnicodeUTF8));
        qtVersionLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "Qt Version:", 0, QApplication::UnicodeUTF8));
        manageQtVersionPushButtons->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "Manage Qt Versions", 0, QApplication::UnicodeUTF8));
        invalidQtWarningLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "This Qt-Version is invalid.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "Shadow Build:", 0, QApplication::UnicodeUTF8));
        shadowBuildCheckBox->setText(QString());
        buildDirLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "Build Directory:", 0, QApplication::UnicodeUTF8));
        importLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::Qt4BuildConfigWidget", "<a href=\"import\">Import existing build</a>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__Qt4BuildConfigWidget);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class Qt4BuildConfigWidget: public Ui_Qt4BuildConfigWidget {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_QT4BUILDCONFIGWIDGET_H
