/********************************************************************************
** Form generated from reading ui file 'UIProjectsViewer.ui'
**
** Created: Tue Apr 10 19:58:47 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_UIPROJECTSVIEWER_H
#define UI_UIPROJECTSVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_UIProjectsViewer
{
public:
    QVBoxLayout *vboxLayout;
    QTreeView *tvProject;
    QLabel *lProject;
    QCheckBox *cbFilter;
    QTextEdit *teOutput;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *pbWrite;
    QPushButton *pbSettings;

    void setupUi(QWidget *UIProjectsViewer)
    {
    UIProjectsViewer->setObjectName(QString::fromUtf8("UIProjectsViewer"));
    vboxLayout = new QVBoxLayout(UIProjectsViewer);
    vboxLayout->setSpacing(3);
    vboxLayout->setMargin(5);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tvProject = new QTreeView(UIProjectsViewer);
    tvProject->setObjectName(QString::fromUtf8("tvProject"));

    vboxLayout->addWidget(tvProject);

    lProject = new QLabel(UIProjectsViewer);
    lProject->setObjectName(QString::fromUtf8("lProject"));

    vboxLayout->addWidget(lProject);

    cbFilter = new QCheckBox(UIProjectsViewer);
    cbFilter->setObjectName(QString::fromUtf8("cbFilter"));

    vboxLayout->addWidget(cbFilter);

    teOutput = new QTextEdit(UIProjectsViewer);
    teOutput->setObjectName(QString::fromUtf8("teOutput"));
    teOutput->setReadOnly(true);

    vboxLayout->addWidget(teOutput);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(3);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    pbWrite = new QPushButton(UIProjectsViewer);
    pbWrite->setObjectName(QString::fromUtf8("pbWrite"));

    hboxLayout->addWidget(pbWrite);

    pbSettings = new QPushButton(UIProjectsViewer);
    pbSettings->setObjectName(QString::fromUtf8("pbSettings"));

    hboxLayout->addWidget(pbSettings);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(UIProjectsViewer);

    QSize size(267, 442);
    size = size.expandedTo(UIProjectsViewer->minimumSizeHint());
    UIProjectsViewer->resize(size);


    QMetaObject::connectSlotsByName(UIProjectsViewer);
    } // setupUi

    void retranslateUi(QWidget *UIProjectsViewer)
    {
    UIProjectsViewer->setWindowTitle(QApplication::translate("UIProjectsViewer", "Form", 0, QApplication::UnicodeUTF8));
    lProject->setText(QApplication::translate("UIProjectsViewer", "TextLabel", 0, QApplication::UnicodeUTF8));
    cbFilter->setText(QApplication::translate("UIProjectsViewer", "Filter On/Off", 0, QApplication::UnicodeUTF8));
    pbWrite->setText(QApplication::translate("UIProjectsViewer", "Write Project", 0, QApplication::UnicodeUTF8));
    pbSettings->setText(QApplication::translate("UIProjectsViewer", "Project Settings", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(UIProjectsViewer);
    } // retranslateUi

};

namespace Ui {
    class UIProjectsViewer: public Ui_UIProjectsViewer {};
} // namespace Ui

#endif // UI_UIPROJECTSVIEWER_H
