#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H
#include <QDebug>
#include <QDialog>
#include <QTreeWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>

#include "ActionManager.h"
#include "Settings.h"
#include "Workspace.h"
#include "MenuBar.h"
#include "KeySequenceInput.h"

ActionManager* ActionManager::mSelf = 0L;

ActionManager* ActionManager::self()
{
	if ( !mSelf )
		mSelf = new ActionManager();
	return mSelf;
}

ActionManager::ActionManager ()
{
    //Can't use MonkeyAction there, because MonkeyAction use ActionManager. It's reqursion
    QMenu* menu = Workspace::self()->menuBar()->menu ("Tools");
    QAction * act = new QAction ("Shortcut settings", this);
    menu->addAction ( act);
    connect ( act, SIGNAL ( triggered()), this, SLOT ( showSettings ()));
}

void ActionManager::addActGroup ( MonkeyActGroup* grp)
{
	actionGroups.append ( grp);
}

void ActionManager::deleteActGroup ( MonkeyActGroup* grp)
{
	actionGroups.removeAll (grp);
}

QKeySequence ActionManager::getShortCut ( MonkeyAction* act)
{
	QString shortcutName = "Shortcuts";//\\"+act->actionGroup()->objectName ()+'\\'+act->objectName();
	qDebug ( qPrintable ( shortcutName));
	qDebug ( qPrintable (Settings::current()->value( "shortcutName").toString ()+"kj"));
	return QKeySequence ( Settings::current()->value( "shortcutName").toString () );
}

QDialog* ActionManager::shotcutsConfig ()
{
	for ( int grpn = 0; grpn< actionGroups.size (); grpn++ )
	{
		MonkeyActGroup* actgrp = actionGroups[grpn];
		QList<QAction*> actions;
		for (int actn = 0; actn < actions.size(); actn ++)
			qDebug ( qPrintable ( actions[actn]->text())); 
	}
	return NULL;
}

void ActionManager::showSettings ()
{
    QDialog wgt (NULL,Qt::Dialog);
    wgt.setFixedSize ( 400, 480);
    QVBoxLayout vbox ( &wgt);
    QTreeWidget list ( &wgt);
    list.setColumnCount (2);
    list.header()->hide();
    //list.setHeaderLabels ( QStringList()<<"Action"<<Shortcut);
    list.header()->setResizeMode(QHeaderView::ResizeToContents);
    MonkeyActGroup* actgrp;
    QString grpName;
    QList<QAction*> actions;
    for ( int grpn = 0; grpn< actionGroups.size (); grpn++ )
	{
        QTreeWidgetItem* grpitem = new QTreeWidgetItem (&list, QStringList (actionGroups[grpn]->objectName()));
        grpitem->setExpanded ( true);
        list.addTopLevelItem ( grpitem);
		actions = actionGroups[grpn]->actions();
		for (int actn = 0; actn < actions.size(); actn ++)
			grpitem->addChild ( new QTreeWidgetItem ( QStringList ()<<( grpName+ actions[actn]->text())<<(actions[actn]->property("Default shortcut").toString()))); 
	}
    
    vbox.addWidget ( &list);
    //actions[actn]->property("Default shortcut").toString()
    QHBoxLayout confbox (&wgt);
    vbox.addLayout (&confbox);
    QPushButton defaultbtn ("Set to default", &wgt);
    confbox.addWidget ( &defaultbtn);
    QPushButton clearbtn ("Clear", &wgt);
    confbox.addWidget ( &clearbtn);
    KeySequenceInput kinput (&wgt);
    kinput.setEnabled (false);
    confbox.addWidget ( &kinput);
    QHBoxLayout hbox (&wgt);
    vbox.addLayout ( &hbox);
     hbox.addSpacing (290);
    QPushButton okButton ("OK", &wgt);
    hbox.addWidget ( &okButton);
    wgt.exec ();
}

#endif //ACTIONMANAGER_HK
