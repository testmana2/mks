/********************************************************************************
** Form generated from reading ui file 'proeditor.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROEDITOR_H
#define UI_PROEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Qt4ProjectManager {
namespace Internal {

class Ui_ProEditor
{
public:
    QHBoxLayout *hboxLayout;
    QListView *m_editListView;
    QVBoxLayout *vboxLayout;
    QToolButton *m_addToolButton;
    QToolButton *m_removeToolButton;
    QToolButton *m_moveUpToolButton;
    QToolButton *m_moveDownToolButton;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *Qt4ProjectManager__Internal__ProEditor)
    {
        if (Qt4ProjectManager__Internal__ProEditor->objectName().isEmpty())
            Qt4ProjectManager__Internal__ProEditor->setObjectName(QString::fromUtf8("Qt4ProjectManager__Internal__ProEditor"));
        Qt4ProjectManager__Internal__ProEditor->resize(621, 557);
        hboxLayout = new QHBoxLayout(Qt4ProjectManager__Internal__ProEditor);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_editListView = new QListView(Qt4ProjectManager__Internal__ProEditor);
        m_editListView->setObjectName(QString::fromUtf8("m_editListView"));
        m_editListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_editListView->setDragEnabled(true);
        m_editListView->setUniformItemSizes(true);

        hboxLayout->addWidget(m_editListView);

        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setMargin(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_addToolButton = new QToolButton(Qt4ProjectManager__Internal__ProEditor);
        m_addToolButton->setObjectName(QString::fromUtf8("m_addToolButton"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_addToolButton->sizePolicy().hasHeightForWidth());
        m_addToolButton->setSizePolicy(sizePolicy);
        m_addToolButton->setMinimumSize(QSize(0, 23));

        vboxLayout->addWidget(m_addToolButton);

        m_removeToolButton = new QToolButton(Qt4ProjectManager__Internal__ProEditor);
        m_removeToolButton->setObjectName(QString::fromUtf8("m_removeToolButton"));
        sizePolicy.setHeightForWidth(m_removeToolButton->sizePolicy().hasHeightForWidth());
        m_removeToolButton->setSizePolicy(sizePolicy);
        m_removeToolButton->setMinimumSize(QSize(0, 23));

        vboxLayout->addWidget(m_removeToolButton);

        m_moveUpToolButton = new QToolButton(Qt4ProjectManager__Internal__ProEditor);
        m_moveUpToolButton->setObjectName(QString::fromUtf8("m_moveUpToolButton"));
        sizePolicy.setHeightForWidth(m_moveUpToolButton->sizePolicy().hasHeightForWidth());
        m_moveUpToolButton->setSizePolicy(sizePolicy);
        m_moveUpToolButton->setMinimumSize(QSize(0, 23));

        vboxLayout->addWidget(m_moveUpToolButton);

        m_moveDownToolButton = new QToolButton(Qt4ProjectManager__Internal__ProEditor);
        m_moveDownToolButton->setObjectName(QString::fromUtf8("m_moveDownToolButton"));
        sizePolicy.setHeightForWidth(m_moveDownToolButton->sizePolicy().hasHeightForWidth());
        m_moveDownToolButton->setSizePolicy(sizePolicy);
        m_moveDownToolButton->setMinimumSize(QSize(0, 23));

        vboxLayout->addWidget(m_moveDownToolButton);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);


        hboxLayout->addLayout(vboxLayout);


        retranslateUi(Qt4ProjectManager__Internal__ProEditor);

        QMetaObject::connectSlotsByName(Qt4ProjectManager__Internal__ProEditor);
    } // setupUi

    void retranslateUi(QWidget *Qt4ProjectManager__Internal__ProEditor)
    {
        m_addToolButton->setText(QApplication::translate("Qt4ProjectManager::Internal::ProEditor", "New", 0, QApplication::UnicodeUTF8));
        m_removeToolButton->setText(QApplication::translate("Qt4ProjectManager::Internal::ProEditor", "Remove", 0, QApplication::UnicodeUTF8));
        m_moveUpToolButton->setText(QApplication::translate("Qt4ProjectManager::Internal::ProEditor", "Up", 0, QApplication::UnicodeUTF8));
        m_moveDownToolButton->setText(QApplication::translate("Qt4ProjectManager::Internal::ProEditor", "Down", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt4ProjectManager__Internal__ProEditor);
    } // retranslateUi

};

} // namespace Internal
} // namespace Qt4ProjectManager

namespace Qt4ProjectManager {
namespace Internal {
namespace Ui {
    class ProEditor: public Ui_ProEditor {};
} // namespace Ui
} // namespace Internal
} // namespace Qt4ProjectManager

#endif // UI_PROEDITOR_H
