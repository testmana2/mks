#include "QtAssistant.h"
#include "Workspace.h"
#include "TabToolBar.h"
#include "QtAssistantChild.h"
// assistant include
#include "mainwindow.h"
#include "helpdialog.h"
#include "config.h"
//
#include <QDockWidget>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocale>
//
//class Q_MONKEY_EXPORT Workspace;
//
QtAssistant::~QtAssistant()
{
	if ( isInstalled() )
		uninstall();
}
//
void QtAssistant::initialize( Workspace* w )
{
	WorkspacePlugin::initialize( w );
	// initialize assistant resource
	Q_INIT_RESOURCE( assistant );
	// initialize assistant config
	QString resourceDir;
	if( resourceDir.isNull() )
		resourceDir = QLibraryInfo::location( QLibraryInfo::TranslationsPath );
	// setting assistant translation
	QTranslator translator( 0 );
	translator.load( QLatin1String( "assistant_" ) + QLocale::system().name(), resourceDir );
	qApp->installTranslator( &translator );
	// setting qt translation
	QTranslator qtTranslator( 0 );
	qtTranslator.load( QLatin1String( "qt_" ) + QLocale::system().name(), resourceDir );
	qApp->installTranslator( &qtTranslator );
	//
	Config* conf = Config::loadConfig( QString() );
	if ( !conf )
	{
		qWarning( qPrintable( tr( "Can't load/create the default profile, aborting..." ) ) );
		deleteLater();
		return;
	}
	// initialisation de la fenetre mere
	mMain = new MainWindow;
	mMain->setObjectName( "Assistant" );
	// initialisation du child
	mChild = QtAssistantChild::self( mWorkspace, mMain );
	mChild->setObjectName( "AssistantChild" );
}
//
QString QtAssistant::name() const
{
	return "Qt Assistant";
}
//
QString QtAssistant::description() const
{
	return "This plugin manage Qt Assistant inside Monkey Studio";
}
//
bool QtAssistant::install()
{
	QDockWidget* dw = qobject_cast<QDockWidget*>( mMain->helpDialog()->parentWidget() );
	if ( !dw )
		return false;
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->appendTab( dw, QPixmap( ":/Icons/Icons/helpassistant.png" ), tr( "Qt Assistant" ) );
	mInstalled = true;
	return true;
}
//
bool QtAssistant::uninstall()
{
	delete mChild;
	delete mMain;
	mInstalled = false;
	return true;
}
//
Q_EXPORT_PLUGIN2( WorkspaceQtAssistant, QtAssistant )
