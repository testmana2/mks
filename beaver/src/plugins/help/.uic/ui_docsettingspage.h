/********************************************************************************
** Form generated from reading ui file 'docsettingspage.ui'
**
** Created: Fri Mar 6 15:16:21 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DOCSETTINGSPAGE_H
#define UI_DOCSETTINGSPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DocSettingsPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *_3;
    QListWidget *docsListWidget;
    QVBoxLayout *_4;
    QPushButton *addButton;
    QPushButton *removeButton;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *DocSettingsPage)
    {
        if (DocSettingsPage->objectName().isEmpty())
            DocSettingsPage->setObjectName(QString::fromUtf8("DocSettingsPage"));
        DocSettingsPage->resize(364, 240);
        verticalLayout = new QVBoxLayout(DocSettingsPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(DocSettingsPage);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        _3 = new QHBoxLayout();
#ifndef Q_OS_MAC
        _3->setSpacing(6);
#endif
        _3->setMargin(0);
        _3->setObjectName(QString::fromUtf8("_3"));
        docsListWidget = new QListWidget(DocSettingsPage);
        docsListWidget->setObjectName(QString::fromUtf8("docsListWidget"));

        _3->addWidget(docsListWidget);

        _4 = new QVBoxLayout();
#ifndef Q_OS_MAC
        _4->setSpacing(6);
#endif
        _4->setMargin(0);
        _4->setObjectName(QString::fromUtf8("_4"));
        addButton = new QPushButton(DocSettingsPage);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        _4->addWidget(addButton);

        removeButton = new QPushButton(DocSettingsPage);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));

        _4->addWidget(removeButton);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        _4->addItem(spacerItem);


        _3->addLayout(_4);


        verticalLayout->addLayout(_3);


        retranslateUi(DocSettingsPage);

        QMetaObject::connectSlotsByName(DocSettingsPage);
    } // setupUi

    void retranslateUi(QWidget *DocSettingsPage)
    {
        DocSettingsPage->setWindowTitle(QApplication::translate("DocSettingsPage", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DocSettingsPage", "Registered Documentation:", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("DocSettingsPage", "Add...", 0, QApplication::UnicodeUTF8));
        removeButton->setText(QApplication::translate("DocSettingsPage", "Remove", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DocSettingsPage);
    } // retranslateUi

};

namespace Ui {
    class DocSettingsPage: public Ui_DocSettingsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCSETTINGSPAGE_H
