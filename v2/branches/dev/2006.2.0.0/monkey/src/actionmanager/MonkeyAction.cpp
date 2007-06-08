#include "MonkeyAction.h"
#include "MonkeyActGroup.h"
#include "ActionManager.h"

MonkeyAction::MonkeyAction ( MonkeyActGroup * parent, const QString & text, const QKeySequence & defaultShortcut ): QAction (text, parent)
{
	parent->appendAction ( this);
	setObjectName ( text);
	setData ( QVariant (defaultShortcut.toString()));
	setShortcut ( ActionManager::getShortCut ( this) );
}

MonkeyAction::~MonkeyAction ()
{
	MonkeyActGroup* group = (MonkeyActGroup*)actionGroup ();
	group->removeAction ( this);
}

void MonkeyAction::setShortcut ( const QKeySequence & shortcut )
{
//just for make method private
}