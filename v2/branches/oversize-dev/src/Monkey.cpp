#include "Monkey.h"
#include "main.h"
#include "Settings.h"
#include "UIMain.h"
#include "StatusBar.h"
#include "MenuBar.h"
#include "MenuTools.h"
#include "Workspace.h"
#include "ProjectManager.h"
#include "UITranslator.h"
//
#include <QSplashScreen>
#include <QTranslator>
//
QPointer<Monkey> Monkey::mSelf = 0L;
//
Monkey* Monkey::self( int& c, char** v )
{
	if ( !mSelf )
		mSelf = new Monkey( c, v );
	return mSelf;
}
//
Monkey::Monkey( int& argc, char** argv )
	: QApplication( argc, argv )
{
	installEventFilter( this );
}
//
Monkey::~Monkey()
{
}
//
void Monkey::initialize()
{
	QSplashScreen splash( QPixmap( ":/Icons/Icons/helpsplashscreen.png" ) );
	splash.show();
	// init application
	splash.showMessage( tr( "Initializing Application..." ), Qt::AlignRight | Qt::AlignTop );
	setStyle( settings()->value( "Style", "plastique" ).toString() );
	setOrganizationName( ORGANIZATION );
	setOrganizationDomain( QString( "www.%1.org" ).arg( ORGANIZATION ).toLower() );
	setApplicationName( PROGRAM_NAME );
	addLibraryPath( "plugins" );
	connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
	// init translation
	splash.showMessage( tr( "Initializing Translation..." ), Qt::AlignRight | Qt::AlignTop );
	QString mLanguage = settings()->value( "Language" ).toString();
	if ( mLanguage.isEmpty() )
	{
		if ( !UITranslator::self( &splash )->exec() )
			mLanguage = "english";
	}
	if ( mLanguage != "english" )
	{
		QTranslator t;
		t.load( QString( "%1/monkey2_%3" ).arg( settings()->value( "Paths/Translations" ).toString(), mLanguage ) );
		installTranslator( &t );
	}
	// init main window
	splash.showMessage( tr( "Initializing Main Window..." ), Qt::AlignRight | Qt::AlignTop );
	main()->setWindowTitle( QString( "%1 v%2 - %3" ).arg( PROGRAM_TITLE, PROGRAM_VERSION, COPYRIGHTS ) );
	main()->show();
	// init menu / menu tools
	splash.showMessage( tr( "Initializing Menu & Tools Menu..." ), Qt::AlignRight | Qt::AlignTop );
	main()->setMenuBar( menuBar() );
	menuBar()->initialize();
	menuTools()->initialize();
	// init recents
	splash.showMessage( tr( "Initializing Recents Files & Projects..." ), Qt::AlignRight | Qt::AlignTop );
	settings()->setMaxRecentFiles( settings()->maxRecentFiles() );
	settings()->setMaxRecentProjects( settings()->maxRecentProjects() );
	menuBar()->updateRecentFiles();
	menuBar()->updateRecentProjects();
	// init statusbar
	splash.showMessage( tr( "Initializing Status Bar..." ), Qt::AlignRight | Qt::AlignTop );
	main()->setStatusBar( statusBar() );
	statusBar()->initialize();
	// init workspace / projectmanager
	splash.showMessage( tr( "Initializing Workspace & Project Manager..." ), Qt::AlignRight | Qt::AlignTop );
	main()->setCentralWidget( workspace() );
	workspace()->initialize();
	projectManager()->initialize();
	// restore session
	splash.showMessage( tr( "Restoring Session..." ), Qt::AlignRight | Qt::AlignTop );
	settings()->restoreState( main() );
	// ready
	splash.showMessage( tr( "%1 v%2 Ready !" ).arg( PROGRAM_TITLE, PROGRAM_VERSION ), Qt::AlignRight | Qt::AlignTop );
	statusBar()->setText( StatusBar::ltStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_TITLE, PROGRAM_VERSION ), 15000 );
	// finish splashscreen
	splash.finish( main() );
}
//
Settings* Monkey::settings()
{
	return Settings::current();
}
//
UIMain* Monkey::main()
{
	return UIMain::self();
}
//
StatusBar* Monkey::statusBar()
{
	return StatusBar::self();
}
//
MenuBar* Monkey::menuBar()
{
	return MenuBar::self();
}
//
MenuTools* Monkey::menuTools()
{
	return MenuTools::self();
}
//
Workspace* Monkey::workspace()
{
	return Workspace::self();
}
//
ProjectManager* Monkey::projectManager()
{
	return ProjectManager::self();
}
