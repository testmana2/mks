#include "MonkeyActGroup.h"
#include "ActionManager.h"

MonkeyActGroup::MonkeyActGroup ( const QString & name, QObject * parent ) : QActionGroup ( parent)
{
	setObjectName ( name);
	ActionManager::self()->addActGroup ( this);
}

MonkeyActGroup::~MonkeyActGroup ()
{
	ActionManager::self()->deleteActGroup (this);
}

void MonkeyActGroup::appendAction ( MonkeyAction* act)
{
	actions.append (act);
}

void MonkeyActGroup::removeAction ( MonkeyAction* act)
{
	actions.removeAll ( act);
}