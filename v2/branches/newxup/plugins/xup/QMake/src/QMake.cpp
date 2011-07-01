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
#include <pMonkeyStudio.h>

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
}

bool QMake::install()
{
	// create qt version manager
	mQtVersionManager = new QtVersionManager( this );
	
	// register item
	const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	DocumentFilterMap filters( ":/qmakeitems" );
	QStringList hf; // HEADERS filters
	QStringList sf; // SOURCES filters
	QStringList yf; // YACC filters
	QStringList lf; // LEX filters
	int weight = 0;
	
	foreach ( const QString& f, cf ) {
		if ( f.startsWith( "*.h", Qt::CaseInsensitive ) ) {
			hf << f;
		}
		
		if ( f.startsWith( "*.c", Qt::CaseInsensitive ) ) {
			sf << f;
		}
	}
	
	foreach ( const QString& s, sf ) {
		const QString y = QString( s ).replace( "c", "y", Qt::CaseInsensitive );
		const QString l = QString( s ).replace( "c", "l", Qt::CaseInsensitive );
		
		if ( !yf.contains( y ) ) {
			yf << s;
		}
		
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( l ) ) {
			lf << s;
		}
	}
	
	filters[ "PROJECT" ].weight = weight++;
	filters[ "PROJECT" ].label = tr( "Qt Project" );
	filters[ "PROJECT" ].icon = "project.png";
	filters[ "PROJECT" ].type = DocumentFilter::Project;
	filters[ "PROJECT" ].filters << "*.pro";
	
	filters[ "INCLUDE_PROJECT" ].weight = weight++;
	filters[ "INCLUDE_PROJECT" ].label = tr( "Qt Include Project" );
	filters[ "INCLUDE_PROJECT" ].icon = "project.png";
	filters[ "INCLUDE_PROJECT" ].type = DocumentFilter::Project;
	filters[ "INCLUDE_PROJECT" ].filters << "*.pri";
	
	filters[ "FORMS3" ].weight = weight++;
	filters[ "FORMS3" ].label = QMakeProjectItem::tr( "Qt3 Forms" );
	filters[ "FORMS3" ].icon = "forms3.png";
	filters[ "FORMS3" ].filters << "*.ui";
	filters[ "FORMS3" ].type = DocumentFilter::File;
	filters[ "FORMS3" ].filtered = true;
	
	filters[ "FORMS" ].weight = weight++;
	filters[ "FORMS" ].label = QMakeProjectItem::tr( "Qt Forms" );
	filters[ "FORMS" ].icon = "forms.png";
	filters[ "FORMS" ].filters << "*.ui";
	filters[ "FORMS" ].type = DocumentFilter::File;
	filters[ "FORMS" ].filtered = true;
	
	filters[ "HEADERS" ].weight = weight++;
	filters[ "HEADERS" ].label = QMakeProjectItem::tr( "Headers" );
	filters[ "HEADERS" ].icon = "headers.png";
	filters[ "HEADERS" ].filters << hf;
	filters[ "HEADERS" ].type = DocumentFilter::File;
	filters[ "HEADERS" ].filtered = true;
	
	filters[ "SOURCES" ].weight = weight++;
	filters[ "SOURCES" ].label = QMakeProjectItem::tr( "Sources" );
	filters[ "SOURCES" ].icon = "sources.png";
	filters[ "SOURCES" ].filters << sf;
	filters[ "SOURCES" ].type = DocumentFilter::File;
	filters[ "SOURCES" ].filtered = true;
	
	filters[ "OBJECTIVE_SOURCES" ].weight = weight++;
	filters[ "OBJECTIVE_SOURCES" ].label = QMakeProjectItem::tr( "Objective Sources" );
	filters[ "OBJECTIVE_SOURCES" ].icon = "obj_sources.png";
	filters[ "OBJECTIVE_SOURCES" ].filters << "*.m" << "*.mm";
	filters[ "OBJECTIVE_SOURCES" ].type = DocumentFilter::File;
	filters[ "OBJECTIVE_SOURCES" ].filtered = true;
	
	filters[ "YACCSOURCES" ].weight = weight++;
	filters[ "YACCSOURCES" ].label = QMakeProjectItem::tr( "YACC Sources" );
	filters[ "YACCSOURCES" ].icon = "yacc_sources.png";
	filters[ "YACCSOURCES" ].filters << yf;
	filters[ "YACCSOURCES" ].type = DocumentFilter::File;
	filters[ "YACCSOURCES" ].filtered = true;
	
	filters[ "LEXSOURCES" ].weight = weight++;
	filters[ "LEXSOURCES" ].label = QMakeProjectItem::tr( "LEX Sources" );
	filters[ "LEXSOURCES" ].icon = "lex_sources.png";
	filters[ "LEXSOURCES" ].filters << lf;
	filters[ "LEXSOURCES" ].type = DocumentFilter::File;
	filters[ "LEXSOURCES" ].filtered = true;
	
	filters[ "TRANSLATIONS" ].weight = weight++;
	filters[ "TRANSLATIONS" ].label = QMakeProjectItem::tr( "Qt Translations" );
	filters[ "TRANSLATIONS" ].icon = "translations.png";
	filters[ "TRANSLATIONS" ].filters << "*.ts";
	filters[ "TRANSLATIONS" ].type = DocumentFilter::File;
	filters[ "TRANSLATIONS" ].filtered = true;
	
	filters[ "RESOURCES" ].weight = weight++;
	filters[ "RESOURCES" ].label = QMakeProjectItem::tr( "Qt Resources" );
	filters[ "RESOURCES" ].icon = "resources.png";
	filters[ "RESOURCES" ].filters << "*.qrc";
	filters[ "RESOURCES" ].type = DocumentFilter::File;
	filters[ "RESOURCES" ].filtered = true;
	
	filters[ "DEF_FILE" ].weight = weight++;
	filters[ "DEF_FILE" ].label = QMakeProjectItem::tr( "Def. Files" );
	filters[ "DEF_FILE" ].icon = "def_file.png";
	filters[ "DEF_FILE" ].filters << "*.def";
	filters[ "DEF_FILE" ].type = DocumentFilter::File;
	filters[ "DEF_FILE" ].filtered = true;
	
	filters[ "RC_FILE" ].weight = weight++;
	filters[ "RC_FILE" ].label = QMakeProjectItem::tr( "Rc. Files" );
	filters[ "RC_FILE" ].icon = "rc_file.png";
	filters[ "RC_FILE" ].filters << "*.rc";
	filters[ "RC_FILE" ].type = DocumentFilter::File;
	filters[ "RC_FILE" ].filtered = true;
	
	filters[ "RES_FILE" ].weight = weight++;
	filters[ "RES_FILE" ].label = QMakeProjectItem::tr( "Res. Files" );
	filters[ "RES_FILE" ].icon = "res_file.png";
	filters[ "RES_FILE" ].filters << "*.res";
	filters[ "RES_FILE" ].type = DocumentFilter::File;
	filters[ "RES_FILE" ].filtered = true;
	
	filters[ "SUBDIRS" ].weight = weight++;
	filters[ "SUBDIRS" ].label = QMakeProjectItem::tr( "Sub Projects" );
	filters[ "SUBDIRS" ].icon = "project.png";
	filters[ "SUBDIRS" ].filters << filters[ "PROJECT" ].filters << filters[ "INCLUDE_PROJECT" ].filters;
	filters[ "SUBDIRS" ].type = DocumentFilter::File;
	filters[ "SUBDIRS" ].filtered = false;
	
	filters[ "OTHER_FILES" ].weight = weight++;
	filters[ "OTHER_FILES" ].label = QMakeProjectItem::tr( "Other Files" );
	filters[ "OTHER_FILES" ].icon = "file.png";
	filters[ "OTHER_FILES" ].filters << "*.txt" << "*README*" << "*Change*";
	filters[ "OTHER_FILES" ].type = DocumentFilter::File;
	filters[ "OTHER_FILES" ].filtered = true;
	
	//
	filters[ "INCLUDEPATH" ].weight = weight++;
	filters[ "INCLUDEPATH" ].label = QMakeProjectItem::tr( "Includes Paths" );
	filters[ "INCLUDEPATH" ].icon = "includepath.png";
	filters[ "INCLUDEPATH" ].filters.clear();
	filters[ "INCLUDEPATH" ].type = DocumentFilter::Path;
	filters[ "INCLUDEPATH" ].filtered = true;
	
	filters[ "DEPENDPATH" ].weight = weight++;
	filters[ "DEPENDPATH" ].label = QMakeProjectItem::tr( "Depends Paths" );
	filters[ "DEPENDPATH" ].icon = "dependpath.png";
	filters[ "DEPENDPATH" ].filters.clear();
	filters[ "DEPENDPATH" ].type = DocumentFilter::Path;
	filters[ "DEPENDPATH" ].filtered = true;
	
	filters[ "VPATH" ].weight = weight++;
	filters[ "VPATH" ].label = QMakeProjectItem::tr( "Virtuals Paths" );
	filters[ "VPATH" ].icon = "vpath.png";
	filters[ "VPATH" ].filters.clear();
	filters[ "VPATH" ].type = DocumentFilter::Path;
	filters[ "VPATH" ].filtered = true;
	
	//
	filters[ "LIBS" ].weight = weight++;
	filters[ "LIBS" ].label = QMakeProjectItem::tr( "Libraries Files" );
	filters[ "LIBS" ].icon = "libs.png";
	filters[ "LIBS" ].filters.clear();
	filters[ "LIBS" ].type = DocumentFilter::Standard;
	filters[ "LIBS" ].filtered = true;
	
	filters[ "DEFINES" ].weight = weight++;
	filters[ "DEFINES" ].label = QMakeProjectItem::tr( "Defines" );
	filters[ "DEFINES" ].icon = "defines.png";
	filters[ "DEFINES" ].filters.clear();
	filters[ "DEFINES" ].type = DocumentFilter::Standard;
	filters[ "DEFINES" ].filtered = true;
	
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
