#include "MonkeyAction.h"
#include "MonkeyActGroup.h"
#include "ActionManager.h"

MonkeyAction::MonkeyAction ( MonkeyActGroup * parent, const QString & text, const QString name, const QKeySequence & defaultShortcut ) : QAction ( parent )
{
	setObjectName ( name );
	setShortcut ( ActionManager::getShortCut ( parent->objectName(), name, defaultShortcut ) );
	setText ( text );
	parent->addAction ( this );
	setProperty ( "Default shortcut", QVariant ( defaultShortcut.toString() ) );
}
