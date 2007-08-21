#ifndef MONKEYACTION_H
#define MONKEYACTION_H

#include <QAction>

/*
Class provides configurable action, that can be used with ActionManager
When object will be created, it's will register self, and will recieve shorcut, 
that are confirured for it. If no shortcut is configured, will be used default, 
gived as parameter. 
See ActionManager.h for more comments

Author - Kopats Andrei. 
hlamer at tut dot by
Writed for the Monkey Developer Studio project
(www.monkeystudio.org)
Date: 14/06/2007
License: GPL
*/

class MonkeyActGroup;

class MonkeyAction : public QAction
{

public:
	MonkeyAction ( MonkeyActGroup * parent, const QString & text, const QString name, const QKeySequence & defaultShortcut );
};
#endif //MONKEYACTION_H
