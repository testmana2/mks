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
#include "MakefileProject.h"
#include "MakefileProjectItem.h"

#include <MonkeyCore.h>

#include <QDir>

void MakefileProject::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "Makefile Project" );
	mPluginInfos.Description = tr( "Allows quickly build Makefile based projects" );
	mPluginInfos.Author = "Andrei Kopats aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
}

bool MakefileProject::install()
{
	// register phpqt item
	MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &MakefileProjectItem::staticMetaObject );
	// values
	const Pair_String_StringList_List mSuffixes = Pair_String_StringList_List()
		<< qMakePair( tr( "Makefile Project" ), QStringList( "*Makefile*" ) );

	// register values
	MonkeyCore::projectTypesIndex()->registerSuffixes( PLUGIN_NAME, mSuffixes );
	
	return true;
}

bool MakefileProject::uninstall()
{
	// unregister qmake item, unregistering auto delete the item
	MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
	return true;
}

Q_EXPORT_PLUGIN2( ProjectMakefileProject, MakefileProject )