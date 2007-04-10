/********************************************************************************
** Form generated from reading ui file 'UIQMakeProjectSettings.ui'
**
** Created: Tue Apr 10 19:58:47 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_UIQMAKEPROJECTSETTINGS_H
#define UI_UIQMAKEPROJECTSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_UIQMakeProjectSettings
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *wMain;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QGroupBox *gb_Qt;
    QVBoxLayout *vboxLayout1;
    QCheckBox *ch_qt_core;
    QCheckBox *ch_qt_gui;
    QCheckBox *ch_qt_network;
    QCheckBox *ch_qt_opengl;
    QCheckBox *ch_qt_sql;
    QCheckBox *ch_qt_svg;
    QCheckBox *ch_qt_xml;
    QCheckBox *ch_qt_qt3support;
    QVBoxLayout *vboxLayout2;
    QGroupBox *gb_Target;
    QHBoxLayout *hboxLayout1;
    QVBoxLayout *vboxLayout3;
    QLabel *lTemplate;
    QLabel *lTarget;
    QLabel *lLanguage;
    QLabel *lOptimozation;
    QLabel *lAuthor;
    QLabel *lVersion;
    QVBoxLayout *vboxLayout4;
    QComboBox *cbTemplate;
    QLineEdit *leTarget;
    QComboBox *cbLanguage;
    QComboBox *cbOptimization;
    QLineEdit *leAuthor;
    QLineEdit *leVersion;
    QLabel *lDestdir;
    QHBoxLayout *hboxLayout2;
    QLineEdit *leDestdir;
    QToolButton *tbDestdir;
    QGroupBox *gbFlags;
    QGridLayout *gridLayout1;
    QCheckBox *cb_ordered;
    QRadioButton *rb_debug_and_release;
    QRadioButton *rb_debug;
    QRadioButton *rb_release;
    QCheckBox *cb_build_all;
    QRadioButton *rb_warn_on;
    QRadioButton *rb_warn_off;
    QWidget *wConfig;
    QVBoxLayout *vboxLayout5;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout2;
    QCheckBox *checkBox_30;
    QCheckBox *checkBox_29;
    QCheckBox *checkBox_32;
    QCheckBox *checkBox_31;
    QCheckBox *checkBox_28;
    QCheckBox *checkBox_27;
    QGroupBox *groupBox_3;
    QHBoxLayout *hboxLayout3;
    QCheckBox *checkBox_17;
    QCheckBox *checkBox_18;
    QCheckBox *checkBox_16;
    QSpacerItem *spacerItem;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout3;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_12;
    QCheckBox *checkBox_14;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_8;
    QCheckBox *checkBox_15;
    QLabel *label;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_13;
    QCheckBox *checkBox_9;
    QCheckBox *checkBox_11;
    QCheckBox *checkBox_10;
    QLabel *label_2;
    QHBoxLayout *hboxLayout4;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacerItem2;
    QPushButton *pushButton_2;
    QWidget *wIncludeFile;
    QVBoxLayout *vboxLayout6;
    QLabel *label_11;
    QListView *listView;
    QHBoxLayout *hboxLayout6;
    QListWidget *listWidget_2;
    QGroupBox *groupBox_7;
    QVBoxLayout *vboxLayout7;
    QComboBox *comboBox_4;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QWidget *wAdvanced;
    QGridLayout *gridLayout4;
    QLabel *lContents;
    QLabel *lScopes;
    QVBoxLayout *vboxLayout8;
    QToolButton *tbAdd;
    QToolButton *tbEdit;
    QToolButton *tbRemove;
    QToolButton *tbClear;
    QFrame *lSeparator;
    QToolButton *tbUp;
    QToolButton *tbDown;
    QSpacerItem *spacerItem3;
    QListView *lvContents;
    QTreeView *tvScopes;
    QWidget *wTranslations;
    QDialogButtonBox *dbbButtons;
    QLabel *lInformations;
    QListWidget *listWidget;

    void setupUi(QDialog *UIQMakeProjectSettings)
    {
    UIQMakeProjectSettings->setObjectName(QString::fromUtf8("UIQMakeProjectSettings"));
    gridLayout = new QGridLayout(UIQMakeProjectSettings);
    gridLayout->setSpacing(3);
    gridLayout->setMargin(5);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    stackedWidget = new QStackedWidget(UIQMakeProjectSettings);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
    stackedWidget->setSizePolicy(sizePolicy);
    wMain = new QWidget();
    wMain->setObjectName(QString::fromUtf8("wMain"));
    vboxLayout = new QVBoxLayout(wMain);
    vboxLayout->setSpacing(3);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(3);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    gb_Qt = new QGroupBox(wMain);
    gb_Qt->setObjectName(QString::fromUtf8("gb_Qt"));
    gb_Qt->setCheckable(true);
    vboxLayout1 = new QVBoxLayout(gb_Qt);
    vboxLayout1->setSpacing(3);
    vboxLayout1->setMargin(5);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    ch_qt_core = new QCheckBox(gb_Qt);
    ch_qt_core->setObjectName(QString::fromUtf8("ch_qt_core"));

    vboxLayout1->addWidget(ch_qt_core);

    ch_qt_gui = new QCheckBox(gb_Qt);
    ch_qt_gui->setObjectName(QString::fromUtf8("ch_qt_gui"));

    vboxLayout1->addWidget(ch_qt_gui);

    ch_qt_network = new QCheckBox(gb_Qt);
    ch_qt_network->setObjectName(QString::fromUtf8("ch_qt_network"));

    vboxLayout1->addWidget(ch_qt_network);

    ch_qt_opengl = new QCheckBox(gb_Qt);
    ch_qt_opengl->setObjectName(QString::fromUtf8("ch_qt_opengl"));

    vboxLayout1->addWidget(ch_qt_opengl);

    ch_qt_sql = new QCheckBox(gb_Qt);
    ch_qt_sql->setObjectName(QString::fromUtf8("ch_qt_sql"));

    vboxLayout1->addWidget(ch_qt_sql);

    ch_qt_svg = new QCheckBox(gb_Qt);
    ch_qt_svg->setObjectName(QString::fromUtf8("ch_qt_svg"));

    vboxLayout1->addWidget(ch_qt_svg);

    ch_qt_xml = new QCheckBox(gb_Qt);
    ch_qt_xml->setObjectName(QString::fromUtf8("ch_qt_xml"));

    vboxLayout1->addWidget(ch_qt_xml);

    ch_qt_qt3support = new QCheckBox(gb_Qt);
    ch_qt_qt3support->setObjectName(QString::fromUtf8("ch_qt_qt3support"));

    vboxLayout1->addWidget(ch_qt_qt3support);


    hboxLayout->addWidget(gb_Qt);

    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(3);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    gb_Target = new QGroupBox(wMain);
    gb_Target->setObjectName(QString::fromUtf8("gb_Target"));
    hboxLayout1 = new QHBoxLayout(gb_Target);
    hboxLayout1->setSpacing(3);
    hboxLayout1->setMargin(5);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setSpacing(3);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    lTemplate = new QLabel(gb_Target);
    lTemplate->setObjectName(QString::fromUtf8("lTemplate"));

    vboxLayout3->addWidget(lTemplate);

    lTarget = new QLabel(gb_Target);
    lTarget->setObjectName(QString::fromUtf8("lTarget"));

    vboxLayout3->addWidget(lTarget);

    lLanguage = new QLabel(gb_Target);
    lLanguage->setObjectName(QString::fromUtf8("lLanguage"));

    vboxLayout3->addWidget(lLanguage);

    lOptimozation = new QLabel(gb_Target);
    lOptimozation->setObjectName(QString::fromUtf8("lOptimozation"));

    vboxLayout3->addWidget(lOptimozation);

    lAuthor = new QLabel(gb_Target);
    lAuthor->setObjectName(QString::fromUtf8("lAuthor"));

    vboxLayout3->addWidget(lAuthor);

    lVersion = new QLabel(gb_Target);
    lVersion->setObjectName(QString::fromUtf8("lVersion"));

    vboxLayout3->addWidget(lVersion);


    hboxLayout1->addLayout(vboxLayout3);

    vboxLayout4 = new QVBoxLayout();
    vboxLayout4->setSpacing(3);
    vboxLayout4->setMargin(0);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    cbTemplate = new QComboBox(gb_Target);
    cbTemplate->setObjectName(QString::fromUtf8("cbTemplate"));

    vboxLayout4->addWidget(cbTemplate);

    leTarget = new QLineEdit(gb_Target);
    leTarget->setObjectName(QString::fromUtf8("leTarget"));

    vboxLayout4->addWidget(leTarget);

    cbLanguage = new QComboBox(gb_Target);
    cbLanguage->setObjectName(QString::fromUtf8("cbLanguage"));

    vboxLayout4->addWidget(cbLanguage);

    cbOptimization = new QComboBox(gb_Target);
    cbOptimization->setObjectName(QString::fromUtf8("cbOptimization"));

    vboxLayout4->addWidget(cbOptimization);

    leAuthor = new QLineEdit(gb_Target);
    leAuthor->setObjectName(QString::fromUtf8("leAuthor"));

    vboxLayout4->addWidget(leAuthor);

    leVersion = new QLineEdit(gb_Target);
    leVersion->setObjectName(QString::fromUtf8("leVersion"));

    vboxLayout4->addWidget(leVersion);


    hboxLayout1->addLayout(vboxLayout4);


    vboxLayout2->addWidget(gb_Target);

    lDestdir = new QLabel(wMain);
    lDestdir->setObjectName(QString::fromUtf8("lDestdir"));

    vboxLayout2->addWidget(lDestdir);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(3);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    leDestdir = new QLineEdit(wMain);
    leDestdir->setObjectName(QString::fromUtf8("leDestdir"));

    hboxLayout2->addWidget(leDestdir);

    tbDestdir = new QToolButton(wMain);
    tbDestdir->setObjectName(QString::fromUtf8("tbDestdir"));

    hboxLayout2->addWidget(tbDestdir);


    vboxLayout2->addLayout(hboxLayout2);


    hboxLayout->addLayout(vboxLayout2);


    vboxLayout->addLayout(hboxLayout);

    gbFlags = new QGroupBox(wMain);
    gbFlags->setObjectName(QString::fromUtf8("gbFlags"));
    gridLayout1 = new QGridLayout(gbFlags);
    gridLayout1->setSpacing(3);
    gridLayout1->setMargin(5);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    cb_ordered = new QCheckBox(gbFlags);
    cb_ordered->setObjectName(QString::fromUtf8("cb_ordered"));
    cb_ordered->setEnabled(false);

    gridLayout1->addWidget(cb_ordered, 1, 1, 1, 1);

    rb_debug_and_release = new QRadioButton(gbFlags);
    rb_debug_and_release->setObjectName(QString::fromUtf8("rb_debug_and_release"));

    gridLayout1->addWidget(rb_debug_and_release, 2, 0, 1, 1);

    rb_debug = new QRadioButton(gbFlags);
    rb_debug->setObjectName(QString::fromUtf8("rb_debug"));

    gridLayout1->addWidget(rb_debug, 1, 0, 1, 1);

    rb_release = new QRadioButton(gbFlags);
    rb_release->setObjectName(QString::fromUtf8("rb_release"));

    gridLayout1->addWidget(rb_release, 0, 0, 1, 1);

    cb_build_all = new QCheckBox(gbFlags);
    cb_build_all->setObjectName(QString::fromUtf8("cb_build_all"));
    cb_build_all->setEnabled(false);

    gridLayout1->addWidget(cb_build_all, 0, 1, 1, 1);

    rb_warn_on = new QRadioButton(gbFlags);
    rb_warn_on->setObjectName(QString::fromUtf8("rb_warn_on"));

    gridLayout1->addWidget(rb_warn_on, 0, 2, 1, 1);

    rb_warn_off = new QRadioButton(gbFlags);
    rb_warn_off->setObjectName(QString::fromUtf8("rb_warn_off"));

    gridLayout1->addWidget(rb_warn_off, 1, 2, 1, 1);


    vboxLayout->addWidget(gbFlags);

    stackedWidget->addWidget(wMain);
    wConfig = new QWidget();
    wConfig->setObjectName(QString::fromUtf8("wConfig"));
    vboxLayout5 = new QVBoxLayout(wConfig);
    vboxLayout5->setSpacing(3);
    vboxLayout5->setMargin(0);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    groupBox_5 = new QGroupBox(wConfig);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    gridLayout2 = new QGridLayout(groupBox_5);
    gridLayout2->setSpacing(3);
    gridLayout2->setMargin(5);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    checkBox_30 = new QCheckBox(groupBox_5);
    checkBox_30->setObjectName(QString::fromUtf8("checkBox_30"));

    gridLayout2->addWidget(checkBox_30, 0, 1, 1, 1);

    checkBox_29 = new QCheckBox(groupBox_5);
    checkBox_29->setObjectName(QString::fromUtf8("checkBox_29"));

    gridLayout2->addWidget(checkBox_29, 1, 1, 1, 1);

    checkBox_32 = new QCheckBox(groupBox_5);
    checkBox_32->setObjectName(QString::fromUtf8("checkBox_32"));

    gridLayout2->addWidget(checkBox_32, 1, 2, 1, 1);

    checkBox_31 = new QCheckBox(groupBox_5);
    checkBox_31->setObjectName(QString::fromUtf8("checkBox_31"));

    gridLayout2->addWidget(checkBox_31, 0, 2, 1, 1);

    checkBox_28 = new QCheckBox(groupBox_5);
    checkBox_28->setObjectName(QString::fromUtf8("checkBox_28"));

    gridLayout2->addWidget(checkBox_28, 1, 0, 1, 1);

    checkBox_27 = new QCheckBox(groupBox_5);
    checkBox_27->setObjectName(QString::fromUtf8("checkBox_27"));

    gridLayout2->addWidget(checkBox_27, 0, 0, 1, 1);


    vboxLayout5->addWidget(groupBox_5);

    groupBox_3 = new QGroupBox(wConfig);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    hboxLayout3 = new QHBoxLayout(groupBox_3);
    hboxLayout3->setSpacing(3);
    hboxLayout3->setMargin(5);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    checkBox_17 = new QCheckBox(groupBox_3);
    checkBox_17->setObjectName(QString::fromUtf8("checkBox_17"));

    hboxLayout3->addWidget(checkBox_17);

    checkBox_18 = new QCheckBox(groupBox_3);
    checkBox_18->setObjectName(QString::fromUtf8("checkBox_18"));

    hboxLayout3->addWidget(checkBox_18);

    checkBox_16 = new QCheckBox(groupBox_3);
    checkBox_16->setObjectName(QString::fromUtf8("checkBox_16"));

    hboxLayout3->addWidget(checkBox_16);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem);


    vboxLayout5->addWidget(groupBox_3);

    groupBox_2 = new QGroupBox(wConfig);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    gridLayout3 = new QGridLayout(groupBox_2);
    gridLayout3->setSpacing(3);
    gridLayout3->setMargin(5);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    checkBox_6 = new QCheckBox(groupBox_2);
    checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

    gridLayout3->addWidget(checkBox_6, 4, 1, 1, 1);

    checkBox_7 = new QCheckBox(groupBox_2);
    checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));

    gridLayout3->addWidget(checkBox_7, 4, 2, 1, 1);

    checkBox_12 = new QCheckBox(groupBox_2);
    checkBox_12->setObjectName(QString::fromUtf8("checkBox_12"));

    gridLayout3->addWidget(checkBox_12, 2, 0, 1, 1);

    checkBox_14 = new QCheckBox(groupBox_2);
    checkBox_14->setObjectName(QString::fromUtf8("checkBox_14"));

    gridLayout3->addWidget(checkBox_14, 2, 2, 1, 1);

    checkBox_3 = new QCheckBox(groupBox_2);
    checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
    checkBox_3->setChecked(true);

    gridLayout3->addWidget(checkBox_3, 0, 0, 1, 1);

    checkBox_8 = new QCheckBox(groupBox_2);
    checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));

    gridLayout3->addWidget(checkBox_8, 1, 0, 1, 1);

    checkBox_15 = new QCheckBox(groupBox_2);
    checkBox_15->setObjectName(QString::fromUtf8("checkBox_15"));

    gridLayout3->addWidget(checkBox_15, 1, 2, 1, 1);

    label = new QLabel(groupBox_2);
    label->setObjectName(QString::fromUtf8("label"));
    QPalette palette;
    QBrush brush(QColor(0, 0, 127, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(0, 0, 127, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
    QBrush brush2(QColor(118, 116, 108, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
    label->setPalette(palette);

    gridLayout3->addWidget(label, 4, 0, 1, 1);

    checkBox_4 = new QCheckBox(groupBox_2);
    checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

    gridLayout3->addWidget(checkBox_4, 0, 1, 1, 1);

    checkBox_5 = new QCheckBox(groupBox_2);
    checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));

    gridLayout3->addWidget(checkBox_5, 0, 2, 1, 1);

    checkBox_13 = new QCheckBox(groupBox_2);
    checkBox_13->setObjectName(QString::fromUtf8("checkBox_13"));

    gridLayout3->addWidget(checkBox_13, 2, 1, 1, 1);

    checkBox_9 = new QCheckBox(groupBox_2);
    checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));

    gridLayout3->addWidget(checkBox_9, 1, 1, 1, 1);

    checkBox_11 = new QCheckBox(groupBox_2);
    checkBox_11->setObjectName(QString::fromUtf8("checkBox_11"));

    gridLayout3->addWidget(checkBox_11, 3, 2, 1, 1);

    checkBox_10 = new QCheckBox(groupBox_2);
    checkBox_10->setObjectName(QString::fromUtf8("checkBox_10"));

    gridLayout3->addWidget(checkBox_10, 3, 1, 1, 1);

    label_2 = new QLabel(groupBox_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    QPalette palette1;
    QBrush brush3(QColor(0, 0, 127, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Active, QPalette::WindowText, brush3);
    QBrush brush4(QColor(0, 0, 127, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
    QBrush brush5(QColor(118, 116, 108, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
    label_2->setPalette(palette1);

    gridLayout3->addWidget(label_2, 3, 0, 1, 1);


    vboxLayout5->addWidget(groupBox_2);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setSpacing(3);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    label_4 = new QLabel(wConfig);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    hboxLayout4->addWidget(label_4);

    lineEdit = new QLineEdit(wConfig);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    hboxLayout4->addWidget(lineEdit);


    vboxLayout5->addLayout(hboxLayout4);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout5->addItem(spacerItem1);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setSpacing(3);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem2);

    pushButton_2 = new QPushButton(wConfig);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    hboxLayout5->addWidget(pushButton_2);


    vboxLayout5->addLayout(hboxLayout5);

    stackedWidget->addWidget(wConfig);
    wIncludeFile = new QWidget();
    wIncludeFile->setObjectName(QString::fromUtf8("wIncludeFile"));
    vboxLayout6 = new QVBoxLayout(wIncludeFile);
    vboxLayout6->setSpacing(3);
    vboxLayout6->setMargin(0);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    label_11 = new QLabel(wIncludeFile);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    vboxLayout6->addWidget(label_11);

    listView = new QListView(wIncludeFile);
    listView->setObjectName(QString::fromUtf8("listView"));

    vboxLayout6->addWidget(listView);

    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setSpacing(6);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    listWidget_2 = new QListWidget(wIncludeFile);
    listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));

    hboxLayout6->addWidget(listWidget_2);

    groupBox_7 = new QGroupBox(wIncludeFile);
    groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
    vboxLayout7 = new QVBoxLayout(groupBox_7);
    vboxLayout7->setSpacing(3);
    vboxLayout7->setMargin(5);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    comboBox_4 = new QComboBox(groupBox_7);
    comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));

    vboxLayout7->addWidget(comboBox_4);

    pushButton = new QPushButton(groupBox_7);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    vboxLayout7->addWidget(pushButton);

    pushButton_3 = new QPushButton(groupBox_7);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

    vboxLayout7->addWidget(pushButton_3);

    pushButton_4 = new QPushButton(groupBox_7);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

    vboxLayout7->addWidget(pushButton_4);

    pushButton_5 = new QPushButton(groupBox_7);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

    vboxLayout7->addWidget(pushButton_5);


    hboxLayout6->addWidget(groupBox_7);


    vboxLayout6->addLayout(hboxLayout6);

    stackedWidget->addWidget(wIncludeFile);
    wAdvanced = new QWidget();
    wAdvanced->setObjectName(QString::fromUtf8("wAdvanced"));
    gridLayout4 = new QGridLayout(wAdvanced);
    gridLayout4->setSpacing(3);
    gridLayout4->setMargin(0);
    gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
    lContents = new QLabel(wAdvanced);
    lContents->setObjectName(QString::fromUtf8("lContents"));

    gridLayout4->addWidget(lContents, 2, 0, 1, 1);

    lScopes = new QLabel(wAdvanced);
    lScopes->setObjectName(QString::fromUtf8("lScopes"));

    gridLayout4->addWidget(lScopes, 0, 0, 1, 2);

    vboxLayout8 = new QVBoxLayout();
    vboxLayout8->setSpacing(3);
    vboxLayout8->setMargin(0);
    vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
    tbAdd = new QToolButton(wAdvanced);
    tbAdd->setObjectName(QString::fromUtf8("tbAdd"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(tbAdd->sizePolicy().hasHeightForWidth());
    tbAdd->setSizePolicy(sizePolicy1);
    tbAdd->setPopupMode(QToolButton::InstantPopup);

    vboxLayout8->addWidget(tbAdd);

    tbEdit = new QToolButton(wAdvanced);
    tbEdit->setObjectName(QString::fromUtf8("tbEdit"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(tbEdit->sizePolicy().hasHeightForWidth());
    tbEdit->setSizePolicy(sizePolicy2);
    tbEdit->setPopupMode(QToolButton::InstantPopup);

    vboxLayout8->addWidget(tbEdit);

    tbRemove = new QToolButton(wAdvanced);
    tbRemove->setObjectName(QString::fromUtf8("tbRemove"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(tbRemove->sizePolicy().hasHeightForWidth());
    tbRemove->setSizePolicy(sizePolicy3);

    vboxLayout8->addWidget(tbRemove);

    tbClear = new QToolButton(wAdvanced);
    tbClear->setObjectName(QString::fromUtf8("tbClear"));
    QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(tbClear->sizePolicy().hasHeightForWidth());
    tbClear->setSizePolicy(sizePolicy4);

    vboxLayout8->addWidget(tbClear);

    lSeparator = new QFrame(wAdvanced);
    lSeparator->setObjectName(QString::fromUtf8("lSeparator"));
    lSeparator->setFrameShape(QFrame::HLine);
    lSeparator->setFrameShadow(QFrame::Sunken);

    vboxLayout8->addWidget(lSeparator);

    tbUp = new QToolButton(wAdvanced);
    tbUp->setObjectName(QString::fromUtf8("tbUp"));
    QSizePolicy sizePolicy5(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(tbUp->sizePolicy().hasHeightForWidth());
    tbUp->setSizePolicy(sizePolicy5);
    tbUp->setArrowType(Qt::UpArrow);

    vboxLayout8->addWidget(tbUp);

    tbDown = new QToolButton(wAdvanced);
    tbDown->setObjectName(QString::fromUtf8("tbDown"));
    QSizePolicy sizePolicy6(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy6.setHorizontalStretch(0);
    sizePolicy6.setVerticalStretch(0);
    sizePolicy6.setHeightForWidth(tbDown->sizePolicy().hasHeightForWidth());
    tbDown->setSizePolicy(sizePolicy6);
    tbDown->setArrowType(Qt::DownArrow);

    vboxLayout8->addWidget(tbDown);

    spacerItem3 = new QSpacerItem(94, 25, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout8->addItem(spacerItem3);


    gridLayout4->addLayout(vboxLayout8, 1, 1, 3, 1);

    lvContents = new QListView(wAdvanced);
    lvContents->setObjectName(QString::fromUtf8("lvContents"));
    lvContents->setEditTriggers(QAbstractItemView::NoEditTriggers);

    gridLayout4->addWidget(lvContents, 3, 0, 1, 1);

    tvScopes = new QTreeView(wAdvanced);
    tvScopes->setObjectName(QString::fromUtf8("tvScopes"));
    tvScopes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    gridLayout4->addWidget(tvScopes, 1, 0, 1, 1);

    stackedWidget->addWidget(wAdvanced);
    wTranslations = new QWidget();
    wTranslations->setObjectName(QString::fromUtf8("wTranslations"));
    stackedWidget->addWidget(wTranslations);

    gridLayout->addWidget(stackedWidget, 1, 1, 1, 1);

    dbbButtons = new QDialogButtonBox(UIQMakeProjectSettings);
    dbbButtons->setObjectName(QString::fromUtf8("dbbButtons"));
    dbbButtons->setOrientation(Qt::Horizontal);
    dbbButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Help|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    gridLayout->addWidget(dbbButtons, 2, 0, 1, 2);

    lInformations = new QLabel(UIQMakeProjectSettings);
    lInformations->setObjectName(QString::fromUtf8("lInformations"));
    QPalette palette2;
    QBrush brush6(QColor(255, 255, 255, 255));
    brush6.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Active, QPalette::Base, brush6);
    QBrush brush7(QColor(255, 255, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Active, QPalette::Window, brush7);
    QBrush brush8(QColor(255, 255, 255, 255));
    brush8.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Inactive, QPalette::Base, brush8);
    QBrush brush9(QColor(255, 255, 255, 255));
    brush9.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Inactive, QPalette::Window, brush9);
    QBrush brush10(QColor(255, 255, 255, 255));
    brush10.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Disabled, QPalette::Base, brush10);
    QBrush brush11(QColor(255, 255, 255, 255));
    brush11.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Disabled, QPalette::Window, brush11);
    lInformations->setPalette(palette2);
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    lInformations->setFont(font);
    lInformations->setAutoFillBackground(true);
    lInformations->setFrameShape(QFrame::StyledPanel);
    lInformations->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(lInformations, 0, 0, 1, 2);

    listWidget = new QListWidget(UIQMakeProjectSettings);
    listWidget->setObjectName(QString::fromUtf8("listWidget"));
    listWidget->setMinimumSize(QSize(140, 0));
    listWidget->setMaximumSize(QSize(140, 16777215));

    gridLayout->addWidget(listWidget, 1, 0, 1, 1);

    lTemplate->setBuddy(cbTemplate);

    retranslateUi(UIQMakeProjectSettings);

    QSize size(629, 424);
    size = size.expandedTo(UIQMakeProjectSettings->minimumSizeHint());
    UIQMakeProjectSettings->resize(size);

    QObject::connect(dbbButtons, SIGNAL(accepted()), UIQMakeProjectSettings, SLOT(accept()));
    QObject::connect(dbbButtons, SIGNAL(rejected()), UIQMakeProjectSettings, SLOT(reject()));
    QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
    QObject::connect(listWidget, SIGNAL(currentTextChanged(QString)), lInformations, SLOT(setText(QString)));

    listWidget->setCurrentRow(0);


    QMetaObject::connectSlotsByName(UIQMakeProjectSettings);
    } // setupUi

    void retranslateUi(QDialog *UIQMakeProjectSettings)
    {
    UIQMakeProjectSettings->setWindowTitle(QApplication::translate("UIQMakeProjectSettings", "Settings", 0, QApplication::UnicodeUTF8));
    gb_Qt->setTitle(QApplication::translate("UIQMakeProjectSettings", "QT", 0, QApplication::UnicodeUTF8));
    ch_qt_core->setText(QApplication::translate("UIQMakeProjectSettings", "QtCore", 0, QApplication::UnicodeUTF8));
    ch_qt_gui->setText(QApplication::translate("UIQMakeProjectSettings", "QtGui", 0, QApplication::UnicodeUTF8));
    ch_qt_network->setText(QApplication::translate("UIQMakeProjectSettings", "QtNetwork", 0, QApplication::UnicodeUTF8));
    ch_qt_opengl->setText(QApplication::translate("UIQMakeProjectSettings", "QtOpenGL", 0, QApplication::UnicodeUTF8));
    ch_qt_sql->setText(QApplication::translate("UIQMakeProjectSettings", "QtSql", 0, QApplication::UnicodeUTF8));
    ch_qt_svg->setText(QApplication::translate("UIQMakeProjectSettings", "QtSvg", 0, QApplication::UnicodeUTF8));
    ch_qt_xml->setText(QApplication::translate("UIQMakeProjectSettings", "QtXml", 0, QApplication::UnicodeUTF8));
    ch_qt_qt3support->setText(QApplication::translate("UIQMakeProjectSettings", "Qt3Support", 0, QApplication::UnicodeUTF8));
    gb_Target->setTitle(QApplication::translate("UIQMakeProjectSettings", "Target options :", 0, QApplication::UnicodeUTF8));
    lTemplate->setText(QApplication::translate("UIQMakeProjectSettings", "TEMPLATE :", 0, QApplication::UnicodeUTF8));
    lTarget->setText(QApplication::translate("UIQMakeProjectSettings", "TARGET :", 0, QApplication::UnicodeUTF8));
    lLanguage->setText(QApplication::translate("UIQMakeProjectSettings", "LANGUAGE :", 0, QApplication::UnicodeUTF8));
    lOptimozation->setText(QApplication::translate("UIQMakeProjectSettings", "OPTIMIZATION :", 0, QApplication::UnicodeUTF8));
    lAuthor->setText(QApplication::translate("UIQMakeProjectSettings", "AUTHOR :", 0, QApplication::UnicodeUTF8));
    lVersion->setText(QApplication::translate("UIQMakeProjectSettings", "VERSION :", 0, QApplication::UnicodeUTF8));
    cbTemplate->clear();
    cbTemplate->addItem(QApplication::translate("UIQMakeProjectSettings", "app", 0, QApplication::UnicodeUTF8));
    cbTemplate->addItem(QApplication::translate("UIQMakeProjectSettings", "lib", 0, QApplication::UnicodeUTF8));
    cbTemplate->addItem(QApplication::translate("UIQMakeProjectSettings", "subdirs", 0, QApplication::UnicodeUTF8));
    cbLanguage->clear();
    cbLanguage->addItem(QApplication::translate("UIQMakeProjectSettings", "Qt4/C++", 0, QApplication::UnicodeUTF8));
    cbLanguage->addItem(QApplication::translate("UIQMakeProjectSettings", "Qt3/C++", 0, QApplication::UnicodeUTF8));
    cbLanguage->addItem(QApplication::translate("UIQMakeProjectSettings", "C++", 0, QApplication::UnicodeUTF8));
    cbOptimization->clear();
    cbOptimization->addItem(QApplication::translate("UIQMakeProjectSettings", "No", 0, QApplication::UnicodeUTF8));
    cbOptimization->addItem(QApplication::translate("UIQMakeProjectSettings", "O2", 0, QApplication::UnicodeUTF8));
    cbOptimization->addItem(QApplication::translate("UIQMakeProjectSettings", "O3", 0, QApplication::UnicodeUTF8));
    lDestdir->setText(QApplication::translate("UIQMakeProjectSettings", "DESTDIR :", 0, QApplication::UnicodeUTF8));
    tbDestdir->setText(QApplication::translate("UIQMakeProjectSettings", "...", 0, QApplication::UnicodeUTF8));
    gbFlags->setTitle(QApplication::translate("UIQMakeProjectSettings", "Control compilation flags :", 0, QApplication::UnicodeUTF8));
    cb_ordered->setText(QApplication::translate("UIQMakeProjectSettings", "ordered", 0, QApplication::UnicodeUTF8));
    rb_debug_and_release->setText(QApplication::translate("UIQMakeProjectSettings", "debug_and_release", 0, QApplication::UnicodeUTF8));
    rb_debug->setText(QApplication::translate("UIQMakeProjectSettings", "debug", 0, QApplication::UnicodeUTF8));
    rb_release->setText(QApplication::translate("UIQMakeProjectSettings", "release", 0, QApplication::UnicodeUTF8));
    cb_build_all->setText(QApplication::translate("UIQMakeProjectSettings", "build_all", 0, QApplication::UnicodeUTF8));
    rb_warn_on->setText(QApplication::translate("UIQMakeProjectSettings", "warn on", 0, QApplication::UnicodeUTF8));
    rb_warn_off->setText(QApplication::translate("UIQMakeProjectSettings", "warn off", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("UIQMakeProjectSettings", "Extra libraries", 0, QApplication::UnicodeUTF8));
    checkBox_30->setText(QApplication::translate("UIQMakeProjectSettings", "QtTest", 0, QApplication::UnicodeUTF8));
    checkBox_29->setText(QApplication::translate("UIQMakeProjectSettings", "QtUiTools", 0, QApplication::UnicodeUTF8));
    checkBox_32->setText(QApplication::translate("UIQMakeProjectSettings", "QAxServer", 0, QApplication::UnicodeUTF8));
    checkBox_31->setText(QApplication::translate("UIQMakeProjectSettings", "QAxContainer", 0, QApplication::UnicodeUTF8));
    checkBox_28->setText(QApplication::translate("UIQMakeProjectSettings", "QtDesigner", 0, QApplication::UnicodeUTF8));
    checkBox_27->setText(QApplication::translate("UIQMakeProjectSettings", "QtAssistant", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("UIQMakeProjectSettings", "Compiler flags :", 0, QApplication::UnicodeUTF8));
    checkBox_17->setText(QApplication::translate("UIQMakeProjectSettings", "rtti", 0, QApplication::UnicodeUTF8));
    checkBox_18->setText(QApplication::translate("UIQMakeProjectSettings", "stl", 0, QApplication::UnicodeUTF8));
    checkBox_16->setText(QApplication::translate("UIQMakeProjectSettings", "exceptions", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("UIQMakeProjectSettings", "Application/library type :", 0, QApplication::UnicodeUTF8));
    checkBox_6->setText(QApplication::translate("UIQMakeProjectSettings", "windows", 0, QApplication::UnicodeUTF8));
    checkBox_7->setText(QApplication::translate("UIQMakeProjectSettings", "console", 0, QApplication::UnicodeUTF8));
    checkBox_12->setText(QApplication::translate("UIQMakeProjectSettings", "designer", 0, QApplication::UnicodeUTF8));
    checkBox_14->setText(QApplication::translate("UIQMakeProjectSettings", "no_lflags_merge", 0, QApplication::UnicodeUTF8));
    checkBox_3->setText(QApplication::translate("UIQMakeProjectSettings", "qt", 0, QApplication::UnicodeUTF8));
    checkBox_8->setText(QApplication::translate("UIQMakeProjectSettings", "x11", 0, QApplication::UnicodeUTF8));
    checkBox_15->setText(QApplication::translate("UIQMakeProjectSettings", "resources", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("UIQMakeProjectSettings", "Win32 app only :", 0, QApplication::UnicodeUTF8));
    checkBox_4->setText(QApplication::translate("UIQMakeProjectSettings", "opengl", 0, QApplication::UnicodeUTF8));
    checkBox_5->setText(QApplication::translate("UIQMakeProjectSettings", "thread", 0, QApplication::UnicodeUTF8));
    checkBox_13->setText(QApplication::translate("UIQMakeProjectSettings", "uic3", 0, QApplication::UnicodeUTF8));
    checkBox_9->setText(QApplication::translate("UIQMakeProjectSettings", "dll", 0, QApplication::UnicodeUTF8));
    checkBox_11->setText(QApplication::translate("UIQMakeProjectSettings", "plugin", 0, QApplication::UnicodeUTF8));
    checkBox_10->setText(QApplication::translate("UIQMakeProjectSettings", "staticlib", 0, QApplication::UnicodeUTF8));
    label_2->setStyleSheet(QString());
    label_2->setText(QApplication::translate("UIQMakeProjectSettings", "Lib only :", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("UIQMakeProjectSettings", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><span style=\" font-family:'Courier New,courier';\">CONFIG</span> variable :</p></body></html>", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("UIQMakeProjectSettings", "Default", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("UIQMakeProjectSettings", "QDirModel here :", 0, QApplication::UnicodeUTF8));
    groupBox_7->setTitle(QString());
    comboBox_4->clear();
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "LIBS", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "INCLUDEPATH", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "TRANSLATIONS", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "RESOURCES", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "DEPENDPATH", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "DEF_FILE", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "RC_FILE", 0, QApplication::UnicodeUTF8));
    comboBox_4->addItem(QApplication::translate("UIQMakeProjectSettings", "RES_FILE", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("UIQMakeProjectSettings", "Add", 0, QApplication::UnicodeUTF8));
    pushButton_3->setText(QApplication::translate("UIQMakeProjectSettings", "Edit", 0, QApplication::UnicodeUTF8));
    pushButton_4->setText(QApplication::translate("UIQMakeProjectSettings", "Delete", 0, QApplication::UnicodeUTF8));
    pushButton_5->setText(QApplication::translate("UIQMakeProjectSettings", "Clear", 0, QApplication::UnicodeUTF8));
    lContents->setText(QApplication::translate("UIQMakeProjectSettings", "Contents :", 0, QApplication::UnicodeUTF8));
    lScopes->setText(QApplication::translate("UIQMakeProjectSettings", "Scopes Explorer :", 0, QApplication::UnicodeUTF8));
    tbAdd->setText(QApplication::translate("UIQMakeProjectSettings", "&Add", 0, QApplication::UnicodeUTF8));
    tbEdit->setText(QApplication::translate("UIQMakeProjectSettings", "&Edit", 0, QApplication::UnicodeUTF8));
    tbRemove->setText(QApplication::translate("UIQMakeProjectSettings", "&Remove", 0, QApplication::UnicodeUTF8));
    tbClear->setText(QApplication::translate("UIQMakeProjectSettings", "&Clear", 0, QApplication::UnicodeUTF8));
    tbUp->setText(QApplication::translate("UIQMakeProjectSettings", "&Up", 0, QApplication::UnicodeUTF8));
    tbDown->setText(QApplication::translate("UIQMakeProjectSettings", "&Down", 0, QApplication::UnicodeUTF8));
    lInformations->setText(QApplication::translate("UIQMakeProjectSettings", "Main", 0, QApplication::UnicodeUTF8));
    listWidget->clear();

    QListWidgetItem *__item = new QListWidgetItem(listWidget);
    __item->setText(QApplication::translate("UIQMakeProjectSettings", "Main", 0, QApplication::UnicodeUTF8));

    QListWidgetItem *__item1 = new QListWidgetItem(listWidget);
    __item1->setText(QApplication::translate("UIQMakeProjectSettings", "Config", 0, QApplication::UnicodeUTF8));

    QListWidgetItem *__item2 = new QListWidgetItem(listWidget);
    __item2->setText(QApplication::translate("UIQMakeProjectSettings", "Include File", 0, QApplication::UnicodeUTF8));

    QListWidgetItem *__item3 = new QListWidgetItem(listWidget);
    __item3->setText(QApplication::translate("UIQMakeProjectSettings", "Advanced", 0, QApplication::UnicodeUTF8));

    QListWidgetItem *__item4 = new QListWidgetItem(listWidget);
    __item4->setText(QApplication::translate("UIQMakeProjectSettings", "Translations", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(UIQMakeProjectSettings);
    } // retranslateUi

};

namespace Ui {
    class UIQMakeProjectSettings: public Ui_UIQMakeProjectSettings {};
} // namespace Ui

#endif // UI_UIQMAKEPROJECTSETTINGS_H
