/********************************************************************************
** Form generated from reading ui file 'embeddedpropertiespage.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EMBEDDEDPROPERTIESPAGE_H
#define UI_EMBEDDEDPROPERTIESPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EmbeddedPropertiesPage
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QCheckBox *virtualBoxCheckbox;
    QLabel *skinLabel;
    QComboBox *skinComboBox;

    void setupUi(QWidget *EmbeddedPropertiesPage)
    {
        if (EmbeddedPropertiesPage->objectName().isEmpty())
            EmbeddedPropertiesPage->setObjectName(QString::fromUtf8("EmbeddedPropertiesPage"));
        EmbeddedPropertiesPage->resize(649, 302);
        verticalLayout = new QVBoxLayout(EmbeddedPropertiesPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        virtualBoxCheckbox = new QCheckBox(EmbeddedPropertiesPage);
        virtualBoxCheckbox->setObjectName(QString::fromUtf8("virtualBoxCheckbox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, virtualBoxCheckbox);

        skinLabel = new QLabel(EmbeddedPropertiesPage);
        skinLabel->setObjectName(QString::fromUtf8("skinLabel"));
        skinLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, skinLabel);

        skinComboBox = new QComboBox(EmbeddedPropertiesPage);
        skinComboBox->setObjectName(QString::fromUtf8("skinComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, skinComboBox);


        verticalLayout->addLayout(formLayout);


        retranslateUi(EmbeddedPropertiesPage);

        QMetaObject::connectSlotsByName(EmbeddedPropertiesPage);
    } // setupUi

    void retranslateUi(QWidget *EmbeddedPropertiesPage)
    {
        EmbeddedPropertiesPage->setWindowTitle(QApplication::translate("EmbeddedPropertiesPage", "Form", 0, QApplication::UnicodeUTF8));
        virtualBoxCheckbox->setText(QApplication::translate("EmbeddedPropertiesPage", "Use Virtual Box\n"
"Note: This adds the toolchain to the build environment and runs the program inside a virtual machine.\n"
"It also automatically sets the correct qt version.", 0, QApplication::UnicodeUTF8));
        skinLabel->setText(QApplication::translate("EmbeddedPropertiesPage", "Skin:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EmbeddedPropertiesPage);
    } // retranslateUi

};

namespace Ui {
    class EmbeddedPropertiesPage: public Ui_EmbeddedPropertiesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMBEDDEDPROPERTIESPAGE_H
