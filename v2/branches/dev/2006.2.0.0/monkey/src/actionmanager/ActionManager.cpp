#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H
#include <QDebug>

#include "ActionManager.h"
#include "Settings.h"


ActionManager* ActionManager::mSelf = 0L;

ActionManager* ActionManager::self()
{
	if ( !mSelf )
		mSelf = new ActionManager();
	return mSelf;
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

#endif //ACTIONMANAGER_HK
