#include "MonkeyActGroup.h"
#include "ActionManager.h"
#include <QWidget>

MonkeyActGroup::MonkeyActGroup ( const QString & name, QWidget * parentWidget ) : QActionGroup ( parentWidget)
{
	setObjectName ( name);
	ActionManager::self()->addActGroup ( this);
}

MonkeyActGroup::~MonkeyActGroup ()
{
	ActionManager::self()->deleteActGroup (this);
}

/*void MonkeyActGroup::appendAction ( MonkeyAction* act)
{
	actions.append (act);
    ((QWidget*)parent())->addAction ( act );
}

void MonkeyActGroup::removeAction ( MonkeyAction* act)
{
	actions.removeAll ( act);
}
*/