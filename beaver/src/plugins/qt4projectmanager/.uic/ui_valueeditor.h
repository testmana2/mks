/********************************************************************************
** Form generated from reading ui file 'valueeditor.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VALUEEDITOR_H
#define UI_VALUEEDITOR_H

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
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_ValueEditor
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *m_varGroupBox;
    QGridLayout *gridLayout;
    QComboBox *m_varComboBox;
    QLabel *m_varLineEditLabel;
    QLabel *m_assignComboBoxLabel;
    QLineEdit *m_varLineEdit;
    QLabel *m_varComboBoxLabel;
    QComboBox *m_assignComboBox;
    QStackedWidget *m_editStackWidget;
    QWidget *m_singleDefinedPage;
    QVBoxLayout *vboxLayout1;
    QGroupBox *m_singleDefinedGroupBox;
    QVBoxLayout *vboxLayout2;
    QLabel *m_singleDefinedDescriptionLabel;
    QComboBox *m_itemComboBox;
    QSpacerItem *spacerItem;
    QWidget *m_singleUndefinedPage;
    QVBoxLayout *vboxLayout3;
    QGroupBox *m_singleUndefinedGroupBox;
    QVBoxLayout *vboxLayout4;
    QLabel *m_singleUndefinedDescriptionLabel;
    QLineEdit *m_itemLineEdit;
    QSpacerItem *spacerItem1;
    QWidget *m_multiDefinedPage;
    QVBoxLayout *vboxLayout5;
    QGroupBox *m_multiDefinedGroupBox;
    QVBoxLayout *vboxLayout6;
    QLabel *m_multiDefinedDescriptionLabel;
    QListWidget *m_itemListWidget;
    QWidget *m_multiUndefinedPage;
    QVBoxLayout *vboxLayout7;
    QGroupBox *m_multiUndefinedGroupBox;
    QVBoxLayout *vboxLayout8;
    QLabel *m_multiUndefinedDescriptionLabel;
    QHBoxLayout *hboxLayout;
    QListView *m_itemListView;
    QVBoxLayout *vboxLayout9;
    QToolButton *m_itemAddButton;
    QToolButton *m_itemRemoveButton;
    QSpacerItem *spacerItem2;

    void setupUi(QWidget *Qt4ProjectManager__Internal__ValueEditor)
    {
        if (Qt4ProjectManager__Internal__ValueEditor->objectName().isEmpty())
            Qt4ProjectManager__Internal__ValueEditor->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__ValueEditor"));
        Qt4ProjectManager__Internal__ValueEditor->resize(621, 557);
        vboxLayout = new QVBoxLayout(Qt4ProjectManager__Internal__ValueEditor);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setMargin(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_varGroupBox = new QGroupBox(Qt4ProjectManager__Internal__ValueEditor);
        m_varGroupBox->setObjectName(QString::fromUtf8("m_varGroupBox"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(4));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_varGroupBox->sizePolicy().hasHeightForWidth());
        m_varGroupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(m_varGroupBox);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_varComboBox = new QComboBox(m_varGroupBox);
        m_varComboBox->setObjectName(QString::fromUtf8("m_varComboBox"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_varComboBox->sizePolicy().hasHeightForWidth());
        m_varComboBox->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(m_varComboBox, 0, 1, 1, 1);

        m_varLineEditLabel = new QLabel(m_varGroupBox);
        m_varLineEditLabel->setObjectName(QString::fromUtf8("m_varLineEditLabel"));

        gridLayout->addWidget(m_varLineEditLabel, 1, 0, 1, 1);

        m_assignComboBoxLabel = new QLabel(m_varGroupBox);
        m_assignComboBoxLabel->setObjectName(QString::fromUtf8("m_assignComboBoxLabel"));

        gridLayout->addWidget(m_assignComboBoxLabel, 2, 0, 1, 1);

        m_varLineEdit = new QLineEdit(m_varGroupBox);
        m_varLineEdit->setObjectName(QString::fromUtf8("m_varLineEdit"));

        gridLayout->addWidget(m_varLineEdit, 1, 1, 1, 1);

        m_varComboBoxLabel = new QLabel(m_varGroupBox);
        m_varComboBoxLabel->setObjectName(QString::fromUtf8("m_varComboBoxLabel"));

        gridLayout->addWidget(m_varComboBoxLabel, 0, 0, 1, 1);

        m_assignComboBox = new QComboBox(m_varGroupBox);
        m_assignComboBox->setObjectName(QString::fromUtf8("m_assignComboBox"));

        gridLayout->addWidget(m_assignComboBox, 2, 1, 1, 1);


        vboxLayout->addWidget(m_varGroupBox);

        m_editStackWidget = new QStackedWidget(Qt4ProjectManager__Internal__ValueEditor);
        m_editStackWidget->setObjectName(QString::fromUtf8("m_editStackWidget"));
        m_singleDefinedPage = new QWidget();
        m_singleDefinedPage->setObjectName(QString::fromUtf8("m_singleDefinedPage"));
        vboxLayout1 = new QVBoxLayout(m_singleDefinedPage);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
        vboxLayout1->setMargin(0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        m_singleDefinedGroupBox = new QGroupBox(m_singleDefinedPage);
        m_singleDefinedGroupBox->setObjectName(QString::fromUtf8("m_singleDefinedGroupBox"));
        vboxLayout2 = new QVBoxLayout(m_singleDefinedGroupBox);
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout2->setMargin(9);
#endif
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        m_singleDefinedDescriptionLabel = new QLabel(m_singleDefinedGroupBox);
        m_singleDefinedDescriptionLabel->setObjectName(QString::fromUtf8("m_singleDefinedDescriptionLabel"));
        m_singleDefinedDescriptionLabel->setWordWrap(true);

        vboxLayout2->addWidget(m_singleDefinedDescriptionLabel);

        m_itemComboBox = new QComboBox(m_singleDefinedGroupBox);
        m_itemComboBox->setObjectName(QString::fromUtf8("m_itemComboBox"));

        vboxLayout2->addWidget(m_itemComboBox);


        vboxLayout1->addWidget(m_singleDefinedGroupBox);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout1->addItem(spacerItem);

        m_editStackWidget->addWidget(m_singleDefinedPage);
        m_singleUndefinedPage = new QWidget();
        m_singleUndefinedPage->setObjectName(QString::fromUtf8("m_singleUndefinedPage"));
        vboxLayout3 = new QVBoxLayout(m_singleUndefinedPage);
#ifndef Q_OS_MAC
        vboxLayout3->setSpacing(6);
#endif
        vboxLayout3->setMargin(0);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        m_singleUndefinedGroupBox = new QGroupBox(m_singleUndefinedPage);
        m_singleUndefinedGroupBox->setObjectName(QString::fromUtf8("m_singleUndefinedGroupBox"));
        vboxLayout4 = new QVBoxLayout(m_singleUndefinedGroupBox);
#ifndef Q_OS_MAC
        vboxLayout4->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout4->setMargin(9);
#endif
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        m_singleUndefinedDescriptionLabel = new QLabel(m_singleUndefinedGroupBox);
        m_singleUndefinedDescriptionLabel->setObjectName(QString::fromUtf8("m_singleUndefinedDescriptionLabel"));
        m_singleUndefinedDescriptionLabel->setWordWrap(true);

        vboxLayout4->addWidget(m_singleUndefinedDescriptionLabel);

        m_itemLineEdit = new QLineEdit(m_singleUndefinedGroupBox);
        m_itemLineEdit->setObjectName(QString::fromUtf8("m_itemLineEdit"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_itemLineEdit->sizePolicy().hasHeightForWidth());
        m_itemLineEdit->setSizePolicy(sizePolicy2);

        vboxLayout4->addWidget(m_itemLineEdit);


        vboxLayout3->addWidget(m_singleUndefinedGroupBox);

        spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout3->addItem(spacerItem1);

        m_editStackWidget->addWidget(m_singleUndefinedPage);
        m_multiDefinedPage = new QWidget();
        m_multiDefinedPage->setObjectName(QString::fromUtf8("m_multiDefinedPage"));
        vboxLayout5 = new QVBoxLayout(m_multiDefinedPage);
#ifndef Q_OS_MAC
        vboxLayout5->setSpacing(6);
#endif
        vboxLayout5->setMargin(0);
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        m_multiDefinedGroupBox = new QGroupBox(m_multiDefinedPage);
        m_multiDefinedGroupBox->setObjectName(QString::fromUtf8("m_multiDefinedGroupBox"));
        vboxLayout6 = new QVBoxLayout(m_multiDefinedGroupBox);
#ifndef Q_OS_MAC
        vboxLayout6->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout6->setMargin(9);
#endif
        vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
        m_multiDefinedDescriptionLabel = new QLabel(m_multiDefinedGroupBox);
        m_multiDefinedDescriptionLabel->setObjectName(QString::fromUtf8("m_multiDefinedDescriptionLabel"));
        m_multiDefinedDescriptionLabel->setWordWrap(true);

        vboxLayout6->addWidget(m_multiDefinedDescriptionLabel);

        m_itemListWidget = new QListWidget(m_multiDefinedGroupBox);
        m_itemListWidget->setObjectName(QString::fromUtf8("m_itemListWidget"));

        vboxLayout6->addWidget(m_itemListWidget);


        vboxLayout5->addWidget(m_multiDefinedGroupBox);

        m_editStackWidget->addWidget(m_multiDefinedPage);
        m_multiUndefinedPage = new QWidget();
        m_multiUndefinedPage->setObjectName(QString::fromUtf8("m_multiUndefinedPage"));
        vboxLayout7 = new QVBoxLayout(m_multiUndefinedPage);
#ifndef Q_OS_MAC
        vboxLayout7->setSpacing(6);
#endif
        vboxLayout7->setMargin(0);
        vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
        m_multiUndefinedGroupBox = new QGroupBox(m_multiUndefinedPage);
        m_multiUndefinedGroupBox->setObjectName(QString::fromUtf8("m_multiUndefinedGroupBox"));
        vboxLayout8 = new QVBoxLayout(m_multiUndefinedGroupBox);
#ifndef Q_OS_MAC
        vboxLayout8->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout8->setMargin(9);
#endif
        vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
        m_multiUndefinedDescriptionLabel = new QLabel(m_multiUndefinedGroupBox);
        m_multiUndefinedDescriptionLabel->setObjectName(QString::fromUtf8("m_multiUndefinedDescriptionLabel"));
        m_multiUndefinedDescriptionLabel->setWordWrap(true);

        vboxLayout8->addWidget(m_multiUndefinedDescriptionLabel);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_itemListView = new QListView(m_multiUndefinedGroupBox);
        m_itemListView->setObjectName(QString::fromUtf8("m_itemListView"));
        m_itemListView->setUniformItemSizes(true);

        hboxLayout->addWidget(m_itemListView);

        vboxLayout9 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout9->setSpacing(6);
#endif
        vboxLayout9->setMargin(0);
        vboxLayout9->setObjectName(QString::fromUtf8("vboxLayout9"));
        m_itemAddButton = new QToolButton(m_multiUndefinedGroupBox);
        m_itemAddButton->setObjectName(QString::fromUtf8("m_itemAddButton"));
        QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(m_itemAddButton->sizePolicy().hasHeightForWidth());
        m_itemAddButton->setSizePolicy(sizePolicy3);
        m_itemAddButton->setMinimumSize(QSize(0, 23));

        vboxLayout9->addWidget(m_itemAddButton);

        m_itemRemoveButton = new QToolButton(m_multiUndefinedGroupBox);
        m_itemRemoveButton->setObjectName(QString::fromUtf8("m_itemRemoveButton"));
        sizePolicy3.setHeightForWidth(m_itemRemoveButton->sizePolicy().hasHeightForWidth());
        m_itemRemoveButton->setSizePolicy(sizePolicy3);
        m_itemRemoveButton->setMinimumSize(QSize(0, 23));

        vboxLayout9->addWidget(m_itemRemoveButton);

        spacerItem2 = new QSpacerItem(44, 128, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout9->addItem(spacerItem2);


        hboxLayout->addLayout(vboxLayout9);


        vboxLayout8->addLayout(hboxLayout);


        vboxLayout7->addWidget(m_multiUndefinedGroupBox);

        m_editStackWidget->addWidget(m_multiUndefinedPage);

        vboxLayout->addWidget(m_editStackWidget);


        retranslateUi(Qt4ProjectManager__Internal__ValueEditor);

        m_editStackWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__ValueEditor);
    } // setupUi

    void retranslateUi(QWidget *Qt4ProjectManager__Internal__ValueEditor)
    {
        m_varGroupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Edit Variable", 0, QApplication::UnicodeUTF8));
        m_varLineEditLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Variable Name:", 0, QApplication::UnicodeUTF8));
        m_assignComboBoxLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Assignment Operator:", 0, QApplication::UnicodeUTF8));
        m_varComboBoxLabel->setText(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Variable:", 0, QApplication::UnicodeUTF8));
        m_assignComboBox->clear();
        m_assignComboBox->insertItems(0, QStringList()
         << QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Append (+=)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Remove (-=)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Replace (~=)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Set (=)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Unique (*=)", 0, QApplication::UnicodeUTF8)
        );
        m_singleDefinedGroupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Select Item", 0, QApplication::UnicodeUTF8));
        m_singleDefinedDescriptionLabel->setText(QString());
        m_singleUndefinedGroupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Edit Item", 0, QApplication::UnicodeUTF8));
        m_singleUndefinedDescriptionLabel->setText(QString());
        m_multiDefinedGroupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Select Items", 0, QApplication::UnicodeUTF8));
        m_multiDefinedDescriptionLabel->setText(QString());
        m_multiUndefinedGroupBox->setTitle(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Edit Items", 0, QApplication::UnicodeUTF8));
        m_multiUndefinedDescriptionLabel->setText(QString());
        m_itemAddButton->setText(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "New", 0, QApplication::UnicodeUTF8));
        m_itemRemoveButton->setText(QApplication::translate("Qt4ProjectManager::Internal::ValueEditor", "Remove", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__ValueEditor);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class ValueEditor: public Ui_ValueEditor {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_VALUEEDITOR_H
