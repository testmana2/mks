/********************************************************************************
** Form generated from reading ui file 'makestep.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAKESTEP_H
#define UI_MAKESTEP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MakeStep
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_1;
    QFormLayout *formLayout_2;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *makeLabel;
    QLineEdit *makeLineEdit;
    QLabel *makeArgumentsLabel;
    QLineEdit *makeArgumentsLineEdit;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *MakeStep)
    {
        if (MakeStep->objectName().isEmpty())
            MakeStep->setObjectName(QString::fromUtf8("MakeStep"));
        MakeStep->resize(428, 384);
        verticalLayout = new QVBoxLayout(MakeStep);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(MakeStep);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page_1 = new QWidget();
        page_1->setObjectName(QString::fromUtf8("page_1"));
        formLayout_2 = new QFormLayout(page_1);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        stackedWidget->addWidget(page_1);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_2 = new QVBoxLayout(page_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        makeLabel = new QLabel(page_2);
        makeLabel->setObjectName(QString::fromUtf8("makeLabel"));

        verticalLayout_2->addWidget(makeLabel);

        makeLineEdit = new QLineEdit(page_2);
        makeLineEdit->setObjectName(QString::fromUtf8("makeLineEdit"));

        verticalLayout_2->addWidget(makeLineEdit);

        makeArgumentsLabel = new QLabel(page_2);
        makeArgumentsLabel->setObjectName(QString::fromUtf8("makeArgumentsLabel"));

        verticalLayout_2->addWidget(makeArgumentsLabel);

        makeArgumentsLineEdit = new QLineEdit(page_2);
        makeArgumentsLineEdit->setObjectName(QString::fromUtf8("makeArgumentsLineEdit"));

        verticalLayout_2->addWidget(makeArgumentsLineEdit);

        verticalSpacer = new QSpacerItem(20, 255, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);

        QWidget::setTabOrder(makeLineEdit, makeArgumentsLineEdit);

        retranslateUi(MakeStep);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MakeStep);
    } // setupUi

    void retranslateUi(QWidget *MakeStep)
    {
        MakeStep->setWindowTitle(QApplication::translate("MakeStep", "Form", 0, QApplication::UnicodeUTF8));
        makeLabel->setText(QApplication::translate("MakeStep", "Override %1:", 0, QApplication::UnicodeUTF8));
        makeArgumentsLabel->setText(QApplication::translate("MakeStep", "Make arguments:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MakeStep);
    } // retranslateUi

};

namespace Ui {
    class MakeStep: public Ui_MakeStep {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAKESTEP_H
