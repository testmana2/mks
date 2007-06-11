#include <QApplication>
//
#include <QMainWindow>
#include <QMenuBar>
#include "pTabbedWorkspace.h"
#include <QTextEdit>
#include "pMenuBar.h"
#include "pActionManager.h"

#include <QErrorMessage>
//
int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	//app.setStyle( "plastique" );
	//
	QMainWindow* m = new QMainWindow;
	m->setAttribute( Qt::WA_DeleteOnClose );
	m->setMenuBar( pMenuBar::instance( m ) );
	pTabbedWorkspace* tw = new pTabbedWorkspace( m );
	m->setCentralWidget( tw );
	// construct menu
	pMenuBar* b = pMenuBar::instance();
	QMenu* mn = b->menu( "mFile" );
	mn->setTitle( "Titi" );
	QAction* a = b->action( "mFile/aShortcuts" );
	a->setText( "Edit shortcuts..." );
	QObject::connect( a, SIGNAL( triggered() ), pActionManager::instance(), SLOT( showSettings() ) );
	//
	m->show();
	//
	tw->setTabShape( QTabBar::RoundedNorth );
	tw->setTabMode( pTabbedWorkspace::tmMDI );
	tw->setDocumentMode( pTabbedWorkspace::dmMaximized );
	//
	for ( int i = 0; i < 10; i++ )
	{
		QTextEdit* e = new QTextEdit;
		e->setWindowTitle( QString( "Tab: %1" ).arg( i ) );
		e->setPlainText( e->windowTitle() );
		tw->addTab( e, e->windowTitle() );
	}
	tw->setCurrentIndex( 5 );
	tw->document( 3 )->close();
	delete tw->document( 4 );
	//
	return app.exec();
}
