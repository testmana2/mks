#include "Monkey.h"
#include "Settings.h"
#include "main.h"
#include "UITranslator.h"
#include "UIMain.h"
#include "ToolsManager.h"
#include "RecentsManager.h"
#include "PluginsManager.h"
#include "StatusBar.h"
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
	initialize();
}
//
void Monkey::initialize()
{
	QSplashScreen splash( QPixmap( ":/Icons/Icons/helpsplashscreen.png" ) );
	splash.show();
	// init application
	splash.showMessage( tr( "Initializing Application..." ), Qt::AlignRight | Qt::AlignTop );
	setStyle( Settings::current()->value( "Style", "plastique" ).toString() );
	setOrganizationName( ORGANIZATION );
	setOrganizationDomain( QString( "www.%1.org" ).arg( ORGANIZATION ).toLower() );
	setApplicationName( PROGRAM_NAME );
	addLibraryPath( "plugins" );
	connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
	// init translation
	splash.showMessage( tr( "Initializing Translation..." ), Qt::AlignRight | Qt::AlignTop );
	QString mLanguage = Settings::current()->value( "Language" ).toString();
	if ( mLanguage.isEmpty() )
	{
		if ( !UITranslator::self( &splash )->exec() )
			mLanguage = "english";
	}
	if ( mLanguage != "english" )
	{
		QTranslator t;
		t.load( QString( "%1/monkey2_%3" ).arg( Settings::current()->value( "Paths/Translations" ).toString(), mLanguage ) );
		installTranslator( &t );
	}
	// init main window
	splash.showMessage( tr( "Initializing Main Window..." ), Qt::AlignRight | Qt::AlignTop );
	UIMain::self()->setWindowTitle( QString( "%1 v%2 - %3" ).arg( PROGRAM_TITLE, PROGRAM_VERSION, COPYRIGHTS ) );
	// menu tools
	splash.showMessage( tr( "Initializing Tools Manager..." ), Qt::AlignRight | Qt::AlignTop );
	ToolsManager::self( UIMain::self() );
	// init recents
	splash.showMessage( tr( "Initializing Recents Manager..." ), Qt::AlignRight | Qt::AlignTop );
	RecentsManager::self( UIMain::self() );
	// init pluginsmanager
	splash.showMessage( tr( "Initializing Plugins Manager..." ), Qt::AlignRight | Qt::AlignTop );
	PluginsManager::self( UIMain::self() )->loadsPlugins();
	// ready
	splash.showMessage( tr( "%1 v%2 Ready !" ).arg( PROGRAM_TITLE, PROGRAM_VERSION ), Qt::AlignRight | Qt::AlignTop );
	StatusBar::self()->setText( StatusBar::tStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_TITLE, PROGRAM_VERSION ), 15000 );
	// finish splashscreen
	UIMain::self()->show();
	splash.finish( UIMain::self() );
}
