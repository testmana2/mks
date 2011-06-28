/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "QMake.h"
#include "QtVersionManager.h"
#include "QMakeProjectItem.h"
#include "UISettingsQMake.h"

#include <MonkeyCore.h>
#include <UIMain.h>

#include <QDir>

QPointer<QtVersionManager> QMake::mQtVersionManager = 0;

QWidget* QMake::settingsWidget()
{
	return new UISettingsQMake();
}

QtVersionManager* QMake::versionManager()
{
	return mQtVersionManager;
}

void QMake::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "QMake Project" );
	mPluginInfos.Description = tr( "QMake Project support for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Roper Alexander aka minirop <minirop@peyj.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = true;
	mPluginInfos.dependencies
		<< "MSVCMake"
		<< "GNUMake"
		<< "BeaverDebugger"
		;
	mPluginInfos.iconsPath = ":/qmakeitems";
}

bool QMake::install()
{
	// create qt version manager
	mQtVersionManager = new QtVersionManager( this );
	
	// register item
	DocumentFilterMap filters;
	int weight = 0;
	
	filters[ "QMAKE_PROJECT" ].weight = weight++;
	filters[ "QMAKE_PROJECT" ].label = tr( "Qt Project" );
	filters[ "QMAKE_PROJECT" ].filters << "*.pro";
	
	filters[ "QMAKE_INCLUDE_PROJECT" ].weight = weight++;
	filters[ "QMAKE_INCLUDE_PROJECT" ].label = tr( "Qt Include Project" );
	filters[ "QMAKE_INCLUDE_PROJECT" ].filters << "*.pri";
	
	MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &QMakeProjectItem::staticMetaObject, filters );
	return true;
}

bool QMake::uninstall()
{
	// unregister item
	MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
	// delete qt version manager
	delete mQtVersionManager;
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
