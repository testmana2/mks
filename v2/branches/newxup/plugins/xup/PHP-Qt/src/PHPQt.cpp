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

#include <MonkeyCore.h>

void PHPQt::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "PHP-Qt Project" );
	mPluginInfos.Description = tr( "PHP-Qt Project support for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.dependencies << "PHP";
}

bool PHPQt::install()
{
	// register item
	DocumentFilterMap filters( ":/phpqtitems" );
	int weight = 0;
	
	filters[ "PROJECT" ].weight = weight++;
	filters[ "PROJECT" ].label = tr( "PHP-Qt Project" );
	filters[ "PROJECT" ].icon = "project.png";
	filters[ "PROJECT" ].type = DocumentFilter::Project;
	filters[ "PROJECT" ].filters << "*.xphpqt";
	
	filters[ "FORMS" ].weight = weight++;
	filters[ "FORMS" ].label = tr( "Qt Forms" );
	filters[ "FORMS" ].icon = "forms.png";
	filters[ "FORMS" ].type = DocumentFilter::File;
	filters[ "FORMS" ].filters << "*.ui";
	filters[ "FORMS" ].filtered = true;
	
	filters[ "PHP_FILES" ].weight = weight++;
	filters[ "PHP_FILES" ].label = tr( "PHP Files" );
	filters[ "PHP_FILES" ].icon = "php.png";
	filters[ "PHP_FILES" ].type = DocumentFilter::File;
	filters[ "PHP_FILES" ].filters << "*.php*";
	filters[ "PHP_FILES" ].filtered = true;
	
	MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &PHPQtProjectItem::staticMetaObject, filters );
	return true;
}

bool PHPQt::uninstall()
{
	// unregister item
	MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
	return true;
}

Q_EXPORT_PLUGIN2( ProjectPHPQt, PHPQt )
