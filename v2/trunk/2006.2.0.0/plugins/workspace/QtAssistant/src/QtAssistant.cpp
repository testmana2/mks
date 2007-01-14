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
	// plugin infos
	mPluginInfos.Caption = tr( "Qt Assistant" );
	mPluginInfos.Description = tr( "This plugin embbeded the Qt Asssitant" );
	mPluginInfos.Type = PluginInfos::iWorkspace;
	mPluginInfos.Name = "QtAssistant";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Installed = false;
	// initialize assistant resource
	Q_INIT_RESOURCE( assistant );
	Config* conf = Config::loadConfig( QString() );
	if ( !conf )
	{
		qWarning( qPrintable( tr( "Can't load/create the default profile, aborting..." ) ) );
		deleteLater();
		return;
	}
}
//
bool QtAssistant::install()
{
	if ( !mMain )
	{
		// init main window
		mMain = new MainWindow;
		mMain->setObjectName( "Assistant" );
		// initialisation du child
		mChild = QtAssistantChild::self( mWorkspace, mMain );
		mChild->setObjectName( "AssistantChild" );
	}
	//
	QDockWidget* dw = qobject_cast<QDockWidget*>( mMain->helpDialog()->parentWidget() );
	if ( !dw )
		return false;
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->appendTab( dw, QPixmap( ":/Icons/Icons/helpassistant.png" ), infos().Caption );
	mPluginInfos.Installed = true;
	return true;
}
//
bool QtAssistant::uninstall()
{
	delete mChild;
	delete mMain->helpDialog();
	delete mMain;
	mPluginInfos.Installed = false;
	return true;
}
//
Q_EXPORT_PLUGIN2( WorkspaceQtAssistant, QtAssistant )
