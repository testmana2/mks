/********************************************************************************
** Form generated from reading ui file 'proeditorcontainer.ui'
**
** Created: Fri Mar 6 15:16:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROEDITORCONTAINER_H
#define UI_PROEDITORCONTAINER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "proparser/proeditor.h"
#include "valueeditor.h"

QT_BEGIN_NAMESPACE

class Ui_ProEditorContainer
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QTreeView *m_scopeList;
    Qt4ProjectManager::Internal::ProEditor *m_variableEdit;
    QCheckBox *m_advancedCheckBox;
    Qt4ProjectManager::Internal::ValueEditor *m_valueEdit;

    void setupUi(QWidget *ProEditorContainer)
    {
        if (ProEditorContainer->objectName().isEmpty())
            ProEditorContainer->setObjectName(QString::fromUtf8("ProEditorContainer"));
        ProEditorContainer->resize(503, 372);
        hboxLayout = new QHBoxLayout(ProEditorContainer);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        m_scopeList = new QTreeView(ProEditorContainer);
        m_scopeList->setObjectName(QString::fromUtf8("m_scopeList"));
        m_scopeList->setUniformRowHeights(true);

        vboxLayout->addWidget(m_scopeList);

        m_variableEdit = new Qt4ProjectManager::Internal::ProEditor(ProEditorContainer);
        m_variableEdit->setObjectName(QString::fromUtf8("m_variableEdit"));

        vboxLayout->addWidget(m_variableEdit);

        m_advancedCheckBox = new QCheckBox(ProEditorContainer);
        m_advancedCheckBox->setObjectName(QString::fromUtf8("m_advancedCheckBox"));

        vboxLayout->addWidget(m_advancedCheckBox);


        hboxLayout->addLayout(vboxLayout);

        m_valueEdit = new Qt4ProjectManager::Internal::ValueEditor(ProEditorContainer);
        m_valueEdit->setObjectName(QString::fromUtf8("m_valueEdit"));

        hboxLayout->addWidget(m_valueEdit);


        retranslateUi(ProEditorContainer);

        QMetaObject::connectSlotsByName(ProEditorContainer);
    } // setupUi

    void retranslateUi(QWidget *ProEditorContainer)
    {
        ProEditorContainer->setWindowTitle(QApplication::translate("ProEditorContainer", "Form", 0, QApplication::UnicodeUTF8));
        m_advancedCheckBox->setText(QApplication::translate("ProEditorContainer", "Advanced Mode", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ProEditorContainer);
    } // retranslateUi

};

namespace Ui {
    class ProEditorContainer: public Ui_ProEditorContainer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROEDITORCONTAINER_H
