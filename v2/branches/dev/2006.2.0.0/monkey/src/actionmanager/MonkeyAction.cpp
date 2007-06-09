#include "MonkeyAction.h"
#include "MonkeyActGroup.h"
#include "ActionManager.h"

MonkeyAction::MonkeyAction ( MonkeyActGroup * parent, const QString & text, const QKeySequence & defaultShortcut ): QAction (parent)
{
	setShortcutContext( Qt::ApplicationShortcut );
    setShortcut ( defaultShortcut);
 	//setShortcut ( ActionManager::getShortCut ( this) );
    setText (text);
    parent->appendAction ( this);
	setObjectName ( text);
	setData ( QVariant (defaultShortcut.toString()));
}

MonkeyAction::~MonkeyAction ()
{
	MonkeyActGroup* group = (MonkeyActGroup*)actionGroup ();
	group->removeAction ( this);
}
