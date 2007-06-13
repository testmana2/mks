#include <QApplication>
//
#include <QMainWindow>
#include <QMenuBar>
#include "pTabbedWorkspace.h"
#include <QTextEdit>
#include "pMenuBar.h"
#include "pActionManager.h"
#include "pSettings.h"
#include "pAction.h"

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

	// create dock toolbar manager
	pDockToolBarManager* tm = pDockToolBarManager::instance( m );
	tm->setSettings( pSettings::instance() );

	// add content to docktoolbar manager
	for ( int i = 0; i < 5; i++ )
		tm->bar( Qt::TopToolBarArea )->addDock( new QDockWidget, QString( "Qt Assistant %1" ).arg( i ), QPixmap( "icon.png" ) );

	// some testing members
	tw->setCurrentIndex( 5 );
	tw->document( 3 )->close();
	delete tw->document( 4 );
	tw->document( 2 )->deleteLater();

	// add action to pdocktoolbar
	pAction* pa = new pAction( "Action", QKeySequence( "Ctrl+S" ) );
	pa->setIcon( QPixmap( "icon.png" ) );
	tm->bar( Qt::TopToolBarArea )->addAction( pa );

	tm->restoreState();

	

	//foreach ( QDockWidget* d, tm->bar( Qt::TopToolBarArea )->docks() )
		//tm->bar( Qt::RightToolBarArea )->addDock( d, d->windowTitle() );

	// execute application
	int r = app.exec();

	tm->saveState();

	return r;
}
