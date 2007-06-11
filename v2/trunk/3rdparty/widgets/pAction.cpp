#include "pAction.h"
#include "pActionManager.h"

pAction::pAction( const QString& s, const QKeySequence& sc, pActionGroup* o )
	: QAction( o )
{
	// action group is require
	Q_ASSERT( o != 0 );

	// set shortcut available as applicatino context
	setShortcutContext( Qt::ApplicationShortcut );

	// set action shortcut
	setProperty( "Default Shortcut", QVariant( sc.toString() ) );
	setShortcut( sc );
	//setShortcut ( ActionManager::getShortCut ( this) );

	// set action text
	setText( s );

	// add to group
	o->addAction( this );
}

pAction::~pAction()
{
	pActionGroup* g = qobject_cast<pActionGroup*>( actionGroup() );
	if ( g )
		g->removeAction( this );
}
