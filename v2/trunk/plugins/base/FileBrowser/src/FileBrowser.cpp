/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : FileBrowser.cpp
** Date      : 2007-11-04T22:50:35
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "FileBrowser.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "pDockFileBrowser.h"

#include <QIcon>

FileBrowser::FileBrowser()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "File Browser" );
	mPluginInfos.Description = tr( "Plugin for browsing file outside the project" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "FileBrowser";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

FileBrowser::~FileBrowser()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool FileBrowser::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		UIMain::instance()->dockToolBar( Qt::LeftToolBarArea )->addDock( pDockFileBrowser::instance(), infos().Caption, QIcon( ":/icons/browser.png" ) );
		// connect signals so we will be able to save/restore state of dock settings
		connect( pDockFileBrowser::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pDockFileBrowser::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// it will remove itself from dock toolbar when deleted
		pDockFileBrowser::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void FileBrowser::saveSettings()
{
	// save current drive and path
	setSettingsValue( "Drive", pDockFileBrowser::instance()->currentDrive() );
	setSettingsValue( "Path", pDockFileBrowser::instance()->currentPath() );
}

void FileBrowser::restoreSettings()
{
	QString s;
	// restore last drive if available
	s = settingsValue( "Drive" ).toString();
	if ( !s.isEmpty() )
		pDockFileBrowser::instance()->setCurrentDrive( s );
	// restore last path if available
	s = settingsValue( "Path" ).toString();
	if ( !s.isEmpty() )
		pDockFileBrowser::instance()->setCurrentPath( s );
}

Q_EXPORT_PLUGIN2( BaseFileBrowser, FileBrowser )
