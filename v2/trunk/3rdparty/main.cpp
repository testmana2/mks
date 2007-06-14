#include <QApplication>
#include <QErrorMessage>
#include <QDockWidget>
#include <QTextEdit>

/*
#include <QMainWindow>
#include <QMenuBar>

#include "pActionManager.h"
#include "pSettings.h"
#include "pAction.h"
*/

#include "pTabbedMainWindow.h"
#include "pMenuBar.h"
#include "pDockToolBar.h"
#include "pTabbedWorkspace.h"


int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	pTabbedMainWindow p;
	p.show();

	// set tabbed sample settings
	p.tabbedWorkspace()->setTabShape( QTabBar::RoundedNorth );
	p.tabbedWorkspace()->setTabMode( pTabbedWorkspace::tmMDI );
	p.tabbedWorkspace()->setDocumentMode( pTabbedWorkspace::dmMaximized );

	// set sample menu
	p.menuBar()->menu( "mTools" )->setTitle( "Tools" );
	p.menuBar()->action( "mTools/aTest" )->setText( "Test" );

	// add sample dock widget
	for ( int i = 0; i < 5; i++ )
		p.dockToolBar( Qt::TopToolBarArea )->addDock( new QDockWidget, QString( "Qt Assistant %1" ).arg( i ), QPixmap( "icon.png" ) );

	// add sample document
	for ( int i = 0; i < 10; i++ )
	{
		QTextEdit* e = new QTextEdit;
		e->setWindowTitle( QString( "Tab: %1" ).arg( i ) );
		e->setPlainText( e->windowTitle() );
		p.tabbedWorkspace()->addTab( e, e->windowTitle() );
	}

	// restore window state
	p.restoreState();

/*
	// fill menu bar
	pMenuBar* b = pMenuBar::instance();
	QMenu* mn = b->menu( "mFile" );
	mn->setTitle( "Titi" );
	QAction* a = b->action( "mFile/aShortcuts" );
	a->setText( "Edit shortcuts..." );

	// init action manager
	pActionManager::instance( &app );
	pActionManager::instance()->setSettings( pSettings::instance() );

	// connect for showinf shortcut manager
	QObject::connect( a, SIGNAL( triggered() ), pActionManager::instance(), SLOT( showSettings() ) );
*/
	return app.exec();
}
