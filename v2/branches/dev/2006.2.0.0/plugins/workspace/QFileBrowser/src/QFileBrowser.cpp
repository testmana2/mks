#include "QFileBrowser.h"
#include "Workspace.h"
#include "TabToolBar.h"
#include "QFileBrowserChild.h"

#include <QDockWidget>

QFileBrowser::~QFileBrowser()
{
	if(isInstalled())
	{
		uninstall();
	}
}

void QFileBrowser::initialize(Workspace* w)
{
	WorkspacePlugin::initialize(w);
	mPluginInfos.Caption = tr("QFileBrowser");
	mPluginInfos.Description = tr("Plugin for browsing file outside the project");

	mPluginInfos.Type = BasePlugin::iWorkspace;
	mPluginInfos.Name = "QFileBrowser";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Installed = false;
}

bool QFileBrowser::install()
{
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->appendTab( QFileBrowserChild::self( mWorkspace )->dock(), QPixmap( ":/images/file.png" ), infos().Caption );
	mPluginInfos.Installed = true;
	return true;
}

bool QFileBrowser::uninstall()
{
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->removeTab( QFileBrowserChild::self( mWorkspace )->dock() );
	delete QFileBrowserChild::self( mWorkspace )->dock();
	delete QFileBrowserChild::self( mWorkspace );
	mPluginInfos.Installed = false;
	return true;
}

Q_EXPORT_PLUGIN2( WorkspaceQFileBrowser, QFileBrowser )
