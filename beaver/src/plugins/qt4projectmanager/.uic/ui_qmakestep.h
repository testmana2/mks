/********************************************************************************
** Form generated from reading ui file 'qmakestep.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QMAKESTEP_H
#define UI_QMAKESTEP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMakeStep
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QComboBox *buildConfigurationComboBox;
    QSpacerItem *horizontalSpacer;
    QLabel *qmakeArgsLabel;
    QLineEdit *qmakeAdditonalArgumentsLineEdit;
    QLabel *label;
    QPlainTextEdit *qmakeArgumentsEdit;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;

    void setupUi(QWidget *QMakeStep)
    {
        if (QMakeStep->objectName().isEmpty())
            QMakeStep->setObjectName(QString::fromUtf8("QMakeStep"));
        QMakeStep->resize(414, 442);
        verticalLayout = new QVBoxLayout(QMakeStep);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(QMakeStep);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page_1 = new QWidget();
        page_1->setObjectName(QString::fromUtf8("page_1"));
        verticalLayout_2 = new QVBoxLayout(page_1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(page_1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buildConfigurationComboBox = new QComboBox(page_1);
        buildConfigurationComboBox->setObjectName(QString::fromUtf8("buildConfigurationComboBox"));

        horizontalLayout->addWidget(buildConfigurationComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        qmakeArgsLabel = new QLabel(page_1);
        qmakeArgsLabel->setObjectName(QString::fromUtf8("qmakeArgsLabel"));

        verticalLayout_2->addWidget(qmakeArgsLabel);

        qmakeAdditonalArgumentsLineEdit = new QLineEdit(page_1);
        qmakeAdditonalArgumentsLineEdit->setObjectName(QString::fromUtf8("qmakeAdditonalArgumentsLineEdit"));

        verticalLayout_2->addWidget(qmakeAdditonalArgumentsLineEdit);

        label = new QLabel(page_1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(label);

        qmakeArgumentsEdit = new QPlainTextEdit(page_1);
        qmakeArgumentsEdit->setObjectName(QString::fromUtf8("qmakeArgumentsEdit"));
        qmakeArgumentsEdit->setMaximumSize(QSize(16777215, 120));
        qmakeArgumentsEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_2->addWidget(qmakeArgumentsEdit);

        verticalSpacer = new QSpacerItem(393, 179, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        stackedWidget->addWidget(page_1);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(QMakeStep);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QMakeStep);
    } // setupUi

    void retranslateUi(QWidget *QMakeStep)
    {
        QMakeStep->setWindowTitle(QApplication::translate("QMakeStep", "Form", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QMakeStep", "QMake Build Configuration:", 0, QApplication::UnicodeUTF8));
        buildConfigurationComboBox->clear();
        buildConfigurationComboBox->insertItems(0, QStringList()
         << QApplication::translate("QMakeStep", "debug", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QMakeStep", "release", 0, QApplication::UnicodeUTF8)
        );
        qmakeArgsLabel->setText(QApplication::translate("QMakeStep", "Additional arguments:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QMakeStep", "Arguments:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(QMakeStep);
    } // retranslateUi

};

namespace Ui {
    class QMakeStep: public Ui_QMakeStep {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMAKESTEP_H
