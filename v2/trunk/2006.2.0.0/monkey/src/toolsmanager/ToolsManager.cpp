#include "ToolsManager.h"
#include "Settings.h"
#include "MenuBar.h"
#include "UIToolsEdit.h"
//
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTimer>
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
	initializeTools();
	connect( menuBar()->menu( "mTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
}
//
void ToolsManager::initializeTools()
{
	// got menu
	QMenu* m = menuBar()->menu( "mTools" );
	// create action
	int n = settings()->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		settings()->setArrayIndex( i );
		QAction* ac = new QAction( QIcon( settings()->value( "FileIcon" ).toString() ), settings()->value( "Caption" ).toString(), m );
		ac->setStatusTip( settings()->value( "FilePath" ).toString() );
		ac->setData( settings()->value( "WorkingPath" ).toString() );
		m->addAction( ac );
	}
	settings()->endArray();
}
//
void ToolsManager::toolsMenu_triggered( QAction* a )
{
	if ( a != menuBar()->action( "mTools/aEdit" ) )
	{
		bool b = false;
		if ( a->data().toString().isEmpty() && QFile::exists( a->statusTip() ) )
			b = QDesktopServices::openUrl( QUrl::fromLocalFile( a->statusTip() ) );
		else if ( a->data().toString().isEmpty() )
			b = QProcess::startDetached( a->statusTip() );
		else
		{
			QProcess* p = new QProcess( this );
			connect( p, SIGNAL( finished( int ) ), p, SLOT( deleteLater() ) );
			p->setWorkingDirectory( a->data().toString() );
			p->start( a->statusTip() );
			b = p->waitForStarted();
		}
		if ( !b )
			qWarning( qPrintable( tr( "can't start: %1" ).arg( a->statusTip() ) ) );
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
			if ( ac != ae && ac != as )
				delete ac;
		// initialize
		initializeTools();
	}
}
