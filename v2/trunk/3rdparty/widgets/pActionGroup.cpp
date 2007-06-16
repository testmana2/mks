#include "pActionGroup.h"
#include "pActionManager.h"
#include "pAction.h"

#include <QDesktopWidget>
#include <QApplication>

QPointer<pActionGroup> pActionGroup::mDefaultGroup = 0L;

pActionGroup* pActionGroup::defaultGroup()
{
	if ( !mDefaultGroup )
		mDefaultGroup = new pActionGroup( "agGlobal", tr( "Global" ), QApplication::desktop() );
	return mDefaultGroup;
}

pActionGroup::pActionGroup( const QString& n, const QString& s, QWidget* w )
	: QActionGroup( w )
{
	// need widget
	Q_ASSERT( w != 0 );

	// need object name
	Q_ASSERT( !n.isEmpty() );

	// set object name
	setObjectName( n );

	// set group name
	setProperty( "Caption", s );

	// add group to groupmanager
	pActionManager::instance()->addGroup( this );
}

pActionGroup::~pActionGroup()
{
	// remove from group manager
	pActionManager::instance()->removeGroup( this );
}

pAction* pActionGroup::addAction( pAction * a )
{
	// add action and return it
	return qobject_cast<pAction*>( QActionGroup::addAction( a ) );
}

QAction* pActionGroup::addAction( QAction* )
{ /*Q_ASSERT( 0 );*/ }

QAction* pActionGroup::addAction( const QString& )
{ /*Q_ASSERT( 0 );*/ }

QAction* pActionGroup::addAction( const QIcon&, const QString& )
{ /*Q_ASSERT( 0 );*/ }
