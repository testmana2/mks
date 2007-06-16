#include "pAction.h"

const char* pAction::_DEFAULT_SHORTCUT_PROPERTY_ = "Default Shortcut";
const char* pAction::_GROUP_PROPERTY_ = "Group";
const char* pAction::_SETTINGS_SCOPE_ = "Shortcuts Manager";

pAction::pAction( const QString& n, const QString& s, const QKeySequence& sc, const QString& g )
	: QAction( pActionManager::instance() )
{
	// set object name
	setObjectName( n );

	// add to group
	pActionManager::addAction( g, this );

	// set action text
	setText( s );

	// set action shortcut
	setProperty( _DEFAULT_SHORTCUT_PROPERTY_, sc.toString() );
	setShortcut( pActionManager::getShortcut( g, this, sc ) );
}

pAction::pAction( const QString& n, const QIcon& i, const QString& s, const QKeySequence& sc, const QString& g )
	: QAction( pActionManager::instance() )
{
	// set object name
	setObjectName( n );

	// add to group
	pActionManager::addAction( g, this );

	// set icon
	setIcon( i );

	// set action text
	setText( s );

	// set action shortcut
	setProperty( _DEFAULT_SHORTCUT_PROPERTY_, sc.toString() );
	setShortcut( pActionManager::getShortcut( g, this, sc ) );
}
