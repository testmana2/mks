/********************************************************************************
** Form generated from reading ui file 'submitpanel.ui'
**
** Created: Fri Mar 6 15:15:17 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUBMITPANEL_H
#define UI_SUBMITPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

namespace Perforce {
namespace Internal {

class Ui_SubmitPanel
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *changeNumber;
    QLabel *label_2;
    QLabel *clientName;
    QLabel *label_3;
    QLabel *userName;

    void setupUi(QGroupBox *Perforce__Internal__SubmitPanel)
    {
        if (Perforce__Internal__SubmitPanel->objectName().isEmpty())
            Perforce__Internal__SubmitPanel->setObjectName(QString::fromUtf8("Perforce__Internal__SubmitPanel"));
        Perforce__Internal__SubmitPanel->resize(402, 134);
        Perforce__Internal__SubmitPanel->setFlat(true);
        verticalLayout = new QVBoxLayout(Perforce__Internal__SubmitPanel);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(5);
        formLayout->setContentsMargins(0, -1, -1, -1);
        label = new QLabel(Perforce__Internal__SubmitPanel);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        changeNumber = new QLabel(Perforce__Internal__SubmitPanel);
        changeNumber->setObjectName(QString::fromUtf8("changeNumber"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(changeNumber->sizePolicy().hasHeightForWidth());
        changeNumber->setSizePolicy(sizePolicy1);
        changeNumber->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        formLayout->setWidget(0, QFormLayout::FieldRole, changeNumber);

        label_2 = new QLabel(Perforce__Internal__SubmitPanel);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        clientName = new QLabel(Perforce__Internal__SubmitPanel);
        clientName->setObjectName(QString::fromUtf8("clientName"));
        sizePolicy1.setHeightForWidth(clientName->sizePolicy().hasHeightForWidth());
        clientName->setSizePolicy(sizePolicy1);
        clientName->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        formLayout->setWidget(1, QFormLayout::FieldRole, clientName);

        label_3 = new QLabel(Perforce__Internal__SubmitPanel);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        userName = new QLabel(Perforce__Internal__SubmitPanel);
        userName->setObjectName(QString::fromUtf8("userName"));
        sizePolicy1.setHeightForWidth(userName->sizePolicy().hasHeightForWidth());
        userName->setSizePolicy(sizePolicy1);
        userName->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        formLayout->setWidget(2, QFormLayout::FieldRole, userName);


        verticalLayout->addLayout(formLayout);


        retranslateUi(Perforce__Internal__SubmitPanel);

        QMetaObject::connectSlotsByName(Perforce__Internal__SubmitPanel);
    } // setupUi

    void retranslateUi(QGroupBox *Perforce__Internal__SubmitPanel)
    {
        Perforce__Internal__SubmitPanel->setTitle(QApplication::translate("Perforce::Internal::SubmitPanel", "Submit", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Perforce::Internal::SubmitPanel", "Change:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Perforce::Internal::SubmitPanel", "Client:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Perforce::Internal::SubmitPanel", "User:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Perforce__Internal__SubmitPanel);
    } // retranslateUi

};

} // namespace Internal
} // namespace Perforce

namespace Perforce {
namespace Internal {
namespace Ui {
    class SubmitPanel: public Ui_SubmitPanel {};
} // namespace Ui
} // namespace Internal
} // namespace Perforce

#endif // UI_SUBMITPANEL_H
