#include "pActionGroup.h"
#include "pActionManager.h"

#include <QDesktopWidget>
#include <QApplication>

QPointer<pActionGroup> pActionGroup::mDefaultGroup = 0L;

pActionGroup* pActionGroup::defaultGroup()
{
	if ( !mDefaultGroup )
		mDefaultGroup = new pActionGroup( tr( "Global" ), QApplication::desktop() );
	return mDefaultGroup;
}

pActionGroup::pActionGroup( const QString& s, QWidget* w )
	: QActionGroup( w )
{
	// need widget
	Q_ASSERT( w != 0 );

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
