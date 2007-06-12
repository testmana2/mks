#include "MonkeyAction.h"
#include "MonkeyActGroup.h"
#include "ActionManager.h"

MonkeyAction::MonkeyAction ( MonkeyActGroup * parent, const QString & text,const QString name,  const QKeySequence & defaultShortcut ): QAction (parent)
{
	
	setObjectName (name);
	setShortcutContext( Qt::ApplicationShortcut );
    setShortcut ( defaultShortcut);
 	setShortcut ( ActionManager::getShortCut ( parent->objectName(), name, defaultShortcut) );
    setText (text);
    parent->addAction ( this);
	setProperty ("Default shortcut", QVariant (defaultShortcut.toString()));
	
}

MonkeyAction::~MonkeyAction ()
{
	MonkeyActGroup* group = (MonkeyActGroup*)actionGroup ();
	group->removeAction ( this);
}
