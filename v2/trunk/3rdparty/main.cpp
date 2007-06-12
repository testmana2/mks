#include <QApplication>
//
#include <QMainWindow>
#include <QMenuBar>
#include "pTabbedWorkspace.h"
#include <QTextEdit>
#include "pMenuBar.h"
#include "pActionManager.h"
#include "pSettings.h"

#include "pDockToolBar.h"
#include "pDockToolBarManager.h"
#include <QDockWidget>

#include <QErrorMessage>
//
int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	//app.setStyle( "plastique" );
	// main window
	QMainWindow* m = new QMainWindow;
	m->setAttribute( Qt::WA_DeleteOnClose );

	// main window menu bar
	m->setMenuBar( pMenuBar::instance( m ) );
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

	// main window document manager
	pTabbedWorkspace* tw = new pTabbedWorkspace( m );
	// set document manager settings
	tw->setTabShape( QTabBar::RoundedNorth );
	tw->setTabMode( pTabbedWorkspace::tmMDI );
	tw->setDocumentMode( pTabbedWorkspace::dmMaximized );
	// add sample document to document manager
	for ( int i = 0; i < 10; i++ )
	{
		QTextEdit* e = new QTextEdit;
		e->setWindowTitle( QString( "Tab: %1" ).arg( i ) );
		e->setPlainText( e->windowTitle() );
		tw->addTab( e, e->windowTitle() );
	}

	// show main window
	m->setCentralWidget( tw );
	m->show();

	pDockToolBarManager* tm = pDockToolBarManager::instance( m );
	for ( int i = 0; i < 5; i++ )
		tm->bar( Qt::LeftToolBarArea )->addDock( new QDockWidget, QString( "My Dock %1" ).arg( i ) );

	

	// some testing members
	tw->setCurrentIndex( 5 );
	tw->document( 3 )->close();
	delete tw->document( 4 );
	tw->document( 2 )->deleteLater();

	// execute application
	return app.exec();
}
