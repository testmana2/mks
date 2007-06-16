#include "pAction.h"
#include "pActionManager.h"

pAction::pAction( const QString& n, const QString& s, const QKeySequence& sc, pActionGroup* o )
	: QAction( o )
{
	// action group is require
	Q_ASSERT( o != 0 );

	// action name is require
	Q_ASSERT( !n.isEmpty() );

	// set object name
	setObjectName( n );

	// add to group
	o->addAction( this );

	// set action text
	setText( s );

	// set action shortcut
	setProperty( "Default Shortcut", sc.toString() );
	setShortcut( pActionManager::getShortCut( o->objectName(), n, sc ) );
}
