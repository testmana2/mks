#include "QtAssistant.h"
#include "Workspace.h"
#include "TabToolBar.h"
#include "QtAssistantChild.h"
// assistant include
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
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->appendTab( QtAssistantChild::self( mWorkspace )->dock(), QPixmap( ":/Icons/Icons/helpassistant.png" ), infos().Caption );
	mPluginInfos.Installed = true;
	return true;
}
//
bool QtAssistant::uninstall()
{
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->removeTab( QtAssistantChild::self( mWorkspace )->dock() );
	delete QtAssistantChild::self( mWorkspace )->dock();
	delete QtAssistantChild::self( mWorkspace );
	mPluginInfos.Installed = false;
	return true;
}
//
Q_EXPORT_PLUGIN2( WorkspaceQtAssistant, QtAssistant )
