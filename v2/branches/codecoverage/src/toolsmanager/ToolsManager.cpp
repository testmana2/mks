#include "ToolsManager.h"
#include "Settings.h"
#include "MenuBar.h"
#include "UIToolsEdit.h"
//
#include <QProcess>
//
QPointer<ToolsManager> ToolsManager::mSelf = 0L;
//
ToolsManager* ToolsManager::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new ToolsManager( p );
	return mSelf;
}
//
ToolsManager::ToolsManager( QObject* p )
	: QObject( p )
{
	initialize();
}
//
Settings* ToolsManager::settings() const
{
	return Settings::current();
}
//
MenuBar* ToolsManager::menuBar() const
{
	return MenuBar::self();
}
//
void ToolsManager::initialize()
{
	// got menu
	QMenu* m = menuBar()->menu( "mTools" );
	// create action
	int n = settings()->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		settings()->setArrayIndex( i );
		QAction* ac = new QAction( QIcon( settings()->value( "Icon" ).toString() ), settings()->value( "Caption" ).toString(), m );
		ac->setStatusTip( settings()->value( "Command" ).toString() );
		m->addAction( ac );
	}
	settings()->endArray();
	connect( menuBar()->menu( "mTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
}
//
void ToolsManager::toolsMenu_triggered( QAction* a )
{
	if ( a != menuBar()->action( "mTools/aEdit" ) )
	{
		if ( !QProcess::startDetached( a->statusTip() ) )
			qWarning( qPrintable( a->statusTip().prepend( tr( "can't start: %1" ).arg( a->statusTip() ) ) ) );
	}
	else if ( UIToolsEdit::self()->exec() )
	{
		// got all menu action
		QList<QAction*> l = menuBar()->menu( "mTools" )->actions();
		// got action to not delete
		QAction* ae = menuBar()->action( "mTools/aEdit" );
		QAction* as = menuBar()->action( "mTools/aSeparator1" );
		// delete unneeded action
		foreach ( QAction* ac, l )
		{
			if ( ac != ae && ac != as )
				delete ac;
		}
		// initialize
		initialize();
	}
}
