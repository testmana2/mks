#ifndef MONKEYACTGROUP_H
#define MONKEYACTGROUP_H

#include <QActionGroup>
#include <QWidget>

class MonkeyAction;

class ActionManager;

/*
Class herited from QActionGroup. When creating object of class- it's will 
automaticaly register self in the ActionManager groups list, and actions from this group 
will be configurable. 
Group should contain only MonkeyActions's
See ActionManager.h for more comments

Author - Kopats Andrei. 
hlamer at tut dot by
Writed for the Monkey Developer Studio project
(www.monkeystudio.org)
Date: 14/06/2007
License: GPL
*/

class MonkeyActGroup : public QActionGroup
{

public:

	MonkeyActGroup ( const QString text, const QString & name, QWidget * parentWidget );
	~MonkeyActGroup ();
	
	MonkeyAction* addAction ( MonkeyAction * action );
	
protected: //just for disabling of using methods.
	virtual QAction* addAction ( QAction * action ) = 0;
	virtual QAction * addAction ( const QString & text ) = 0;
	virtual QAction * addAction ( const QIcon & icon, const QString & text ) = 0;
};
#endif //MONKEYACTGROUP_H
