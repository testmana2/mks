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
void showMessage( QSplashScreen* s, const QString& m )
{
	s->showMessage( m, Qt::AlignRight | Qt::AlignTop, Qt::red );
}
//
void Monkey::initialize()
{
	QSplashScreen splash( QPixmap( ":/Icons/Icons/helpsplashscreen.png" ) );
	QFont f( splash.font() );
	f.setBold( true );
	splash.setFont( f );
	splash.show();
	// init application
	showMessage( &splash, tr( "Initializing Application..." ) );
	setStyle( Settings::current()->value( "Style", "plastique" ).toString() );
	setOrganizationName( ORGANIZATION );
	setOrganizationDomain( QString( "www.%1.org" ).arg( ORGANIZATION ).toLower() );
	setApplicationName( PROGRAM_NAME );
	addLibraryPath( "plugins" );
	connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
	// init translation
	showMessage( &splash, tr( "Initializing Translation..." ) );
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
	showMessage( &splash, tr( "Initializing Main Window..." ) );
	UIMain::self()->setWindowTitle( QString( "%1 v%2 - %3" ).arg( PROGRAM_TITLE, PROGRAM_VERSION, COPYRIGHTS ) );
	// menu tools
	showMessage( &splash, tr( "Initializing Tools Manager..." ) );
	ToolsManager::self( UIMain::self() );
	// init recents
	showMessage( &splash, tr( "Initializing Recents Manager..." ) );
	RecentsManager::self( UIMain::self() );
	// init pluginsmanager
	showMessage( &splash, tr( "Initializing Plugins Manager..." ) );
	PluginsManager::self( UIMain::self() )->loadsPlugins();
	// ready
	showMessage( &splash, tr( "%1 v%2 Ready !" ).arg( PROGRAM_TITLE, PROGRAM_VERSION ) );
	StatusBar::self()->setText( StatusBar::tStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_TITLE, PROGRAM_VERSION ), 15000 );
	// finish splashscreen
	UIMain::self()->show();
	splash.finish( UIMain::self() );
}
