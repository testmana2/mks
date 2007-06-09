#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H
#include <QDebug>
#include <QDialog>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>

#include "ActionManager.h"
#include "Settings.h"
#include "Workspace.h"
#include "MenuBar.h"
#include "KeyInput.h"

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
    QDialog wgt;
    QVBoxLayout vbox ( &wgt);
    QListWidget list ( &wgt);
    vbox.addWidget ( &list);
    QHBoxLayout confbox (&wgt);
    KeyInput 
    QHBoxLayout hbox (&wgt);
    vbox.addLayout ( &hbox);
     hbox.addSpacing (60);
    QPushButton okButton (&wgt);
    hbox.addWidget ( &okButton);
    wgt.exec ();
}

#endif //ACTIONMANAGER_HK
