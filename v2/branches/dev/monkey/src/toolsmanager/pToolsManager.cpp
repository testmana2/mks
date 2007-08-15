#include "pToolsManager.h"
#include "pSettings.h"
#include "pMenuBar.h"
#include "UIToolsEdit.h"
#include "UIDesktopMenu.h"

#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTimer>

pToolsManager::pToolsManager( QObject* p )
	: QObject( p )
{
	// initialise action
	initializeTools();
	// action connection
	connect( pMenuBar::instance()->menu( "mTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
}

const QList<pTool> pToolsManager::tools( ToolType t )
{
	// tools list
	QList<pTool> l;
	// got settings
	pSettings* s = pSettings::instance();
	// create action
	int n = s->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		s->setArrayIndex( i );
		if ( t == ttAll || ( t == ttUserEntry && !s->value( "DesktopEntry" ).toBool() ) || t == ttDesktopEntry && s->value( "DesktopEntry" ).toBool() )
			l << pTool( s->value( "Caption" ).toString(), s->value( "FileIcon" ).toString(), s->value( "FilePath" ).toString(), s->value( "WorkingPath" ).toString(), s->value( "DesktopEntry" ).toBool() );
	}
	s->endArray();
	// return list
	return l;
}

void pToolsManager::initializeTools()
{
	// got menu
	QMenu* m = pMenuBar::instance()->menu( "mTools" );
	// initialize tools
	foreach ( pTool t, tools() )
	{
		QAction* ac = new QAction( QIcon( t.FileIcon ), t.Caption, m );
		ac->setStatusTip( t.FilePath );
		ac->setData( t.WorkingPath );
		ac->setProperty( "DesktopEntry", t.DesktopEntry );
		m->addAction( ac );
	}
}

void pToolsManager::toolsMenu_triggered( QAction* a )
{
	if ( a == pMenuBar::instance()->action( "mTools/aEdit" ) || a == pMenuBar::instance()->action( "mTools/aEditDesktop" ) )
	{
		if ( ( a == pMenuBar::instance()->action( "mTools/aEdit" ) ? UIToolsEdit::instance()->exec() : UIDesktopMenu::instance()->exec() ) )
		{
			// got menubar
			pMenuBar* mb = pMenuBar::instance();
			// got all menu action
			QList<QAction*> l = mb->menu( "mTools" )->actions();
			// got action to not delete
			QAction* ae = mb->action( "mTools/aEdit" );
			QAction* aed = mb->action( "mTools/aEditDesktop" );
			QAction* as = mb->action( "mTools/aSeparator1" );
			// delete unneeded action
			foreach ( QAction* ac, l )
				if ( ac != ae && ac != aed && ac != as )
					delete ac;
			// initialize
			initializeTools();
		}
	}
	else
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
}
