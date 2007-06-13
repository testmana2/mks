#include "MonkeyActGroup.h"
#include "ActionManager.h"

MonkeyActGroup::MonkeyActGroup ( const QString text, const QString & name, QWidget * parentWidget ) : QActionGroup ( parentWidget)
{
	setObjectName ( name);
	setProperty ("text", QVariant (text));
	ActionManager::self()->addActGroup ( this);
}

MonkeyActGroup::~MonkeyActGroup ()
{
	ActionManager::self()->deleteActGroup (this);
}

MonkeyAction*  MonkeyActGroup::addAction ( MonkeyAction * action )
{
	return (MonkeyAction*)QActionGroup::addAction ( action);
};
