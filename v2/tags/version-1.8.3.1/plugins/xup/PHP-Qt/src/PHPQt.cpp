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
#include "PHPQt.h"
#include "PHPQtProjectItem.h"
//#include "UISettingsPHPQt.h"
#include "UIPHPQtEditor.h"

#include <MonkeyCore.h>
#include <UIMain.h>

#include <QDir>

PHPQt::PHPQt()
{
	mItem = 0;
	// set plugin infos
	mPluginInfos.Caption = tr( "PHP-Qt Project" );
	mPluginInfos.Description = tr( "PHP-Qt Project support for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Enabled = false;
}

PHPQt::~PHPQt()
{
	if ( isEnabled() )
	{
		setEnabled( false );
	}
}

bool PHPQt::setEnabled( bool enabled )
{
	if ( enabled && !isEnabled() )
	{
		// register phpqt item
		mItem = new PHPQtProjectItem;
		mItem->registerProjectType();

		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !enabled && isEnabled() )
	{
		// unregister qmake item, unregistering auto delete the item
		mItem->unRegisterProjectType();
		delete mItem;

		// set plugin disabled
		mPluginInfos.Enabled = false;
	}

	// return default value
	return true;
}

QWidget* PHPQt::settingsWidget()
{
	return 0/*new UISettingsPHPQt()*/;
}

bool PHPQt::editProject( XUPProjectItem* project )
{
		if ( !project )
		{
		return false;
	}

	return UIPHPQtEditor( project, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

Q_EXPORT_PLUGIN2( ProjectPHPQt, PHPQt )
