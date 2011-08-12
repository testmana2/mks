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

// Q_GLOBAL_STATIC( Type, accessorMemberName )
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
	mQtVersionManager = new QtVersionManager( this );
	
	const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	mFilters = DocumentFilterMap( ":/qmakeitems" );
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
	
	mFilters[ "PROJECT" ].weight = weight++;
	mFilters[ "PROJECT" ].label = tr( "Qt Project" );
	mFilters[ "PROJECT" ].icon = "project.png";
	mFilters[ "PROJECT" ].type = DocumentFilter::Project;
	mFilters[ "PROJECT" ].filters << "*.pro";
	
	mFilters[ "INCLUDE_PROJECT" ].weight = weight++;
	mFilters[ "INCLUDE_PROJECT" ].label = tr( "Qt Include Project" );
	mFilters[ "INCLUDE_PROJECT" ].icon = "project.png";
	mFilters[ "INCLUDE_PROJECT" ].type = DocumentFilter::Project;
	mFilters[ "INCLUDE_PROJECT" ].filters << "*.pri";
	
	mFilters[ "FORMS3" ].weight = weight++;
	mFilters[ "FORMS3" ].label = QMakeProjectItem::tr( "Qt3 Forms" );
	mFilters[ "FORMS3" ].icon = "forms3.png";
	mFilters[ "FORMS3" ].filters << "*.ui";
	mFilters[ "FORMS3" ].type = DocumentFilter::File;
	mFilters[ "FORMS3" ].filtered = true;
	
	mFilters[ "FORMS" ].weight = weight++;
	mFilters[ "FORMS" ].label = QMakeProjectItem::tr( "Qt Forms" );
	mFilters[ "FORMS" ].icon = "forms.png";
	mFilters[ "FORMS" ].filters << "*.ui";
	mFilters[ "FORMS" ].type = DocumentFilter::File;
	mFilters[ "FORMS" ].filtered = true;
	
	mFilters[ "HEADERS" ].weight = weight++;
	mFilters[ "HEADERS" ].label = QMakeProjectItem::tr( "C++ Headers" );
	mFilters[ "HEADERS" ].icon = "headers.png";
	mFilters[ "HEADERS" ].filters << hf;
	mFilters[ "HEADERS" ].type = DocumentFilter::File;
	mFilters[ "HEADERS" ].filtered = true;
	
	mFilters[ "SOURCES" ].weight = weight++;
	mFilters[ "SOURCES" ].label = QMakeProjectItem::tr( "C++ Sources" );
	mFilters[ "SOURCES" ].icon = "sources.png";
	mFilters[ "SOURCES" ].filters << sf;
	mFilters[ "SOURCES" ].type = DocumentFilter::File;
	mFilters[ "SOURCES" ].filtered = true;
	
	mFilters[ "OBJECTIVE_SOURCES" ].weight = weight++;
	mFilters[ "OBJECTIVE_SOURCES" ].label = QMakeProjectItem::tr( "Objective-C++ Sources" );
	mFilters[ "OBJECTIVE_SOURCES" ].icon = "obj_sources.png";
	mFilters[ "OBJECTIVE_SOURCES" ].filters << "*.m" << "*.mm";
	mFilters[ "OBJECTIVE_SOURCES" ].type = DocumentFilter::File;
	mFilters[ "OBJECTIVE_SOURCES" ].filtered = true;
	
	mFilters[ "YACCSOURCES" ].weight = weight++;
	mFilters[ "YACCSOURCES" ].label = QMakeProjectItem::tr( "YACC Sources" );
	mFilters[ "YACCSOURCES" ].icon = "yacc_sources.png";
	mFilters[ "YACCSOURCES" ].filters << yf;
	mFilters[ "YACCSOURCES" ].type = DocumentFilter::File;
	mFilters[ "YACCSOURCES" ].filtered = true;
	
	mFilters[ "LEXSOURCES" ].weight = weight++;
	mFilters[ "LEXSOURCES" ].label = QMakeProjectItem::tr( "LEX Sources" );
	mFilters[ "LEXSOURCES" ].icon = "lex_sources.png";
	mFilters[ "LEXSOURCES" ].filters << lf;
	mFilters[ "LEXSOURCES" ].type = DocumentFilter::File;
	mFilters[ "LEXSOURCES" ].filtered = true;
	
	mFilters[ "TRANSLATIONS" ].weight = weight++;
	mFilters[ "TRANSLATIONS" ].label = QMakeProjectItem::tr( "Qt Translations" );
	mFilters[ "TRANSLATIONS" ].icon = "translations.png";
	mFilters[ "TRANSLATIONS" ].filters << "*.ts";
	mFilters[ "TRANSLATIONS" ].type = DocumentFilter::File;
	mFilters[ "TRANSLATIONS" ].filtered = true;
	
	mFilters[ "RESOURCES" ].weight = weight++;
	mFilters[ "RESOURCES" ].label = QMakeProjectItem::tr( "Qt Resources" );
	mFilters[ "RESOURCES" ].icon = "resources.png";
	mFilters[ "RESOURCES" ].filters << "*.qrc";
	mFilters[ "RESOURCES" ].type = DocumentFilter::File;
	mFilters[ "RESOURCES" ].filtered = true;
	
	mFilters[ "DEF_FILE" ].weight = weight++;
	mFilters[ "DEF_FILE" ].label = QMakeProjectItem::tr( "DEF Files" );
	mFilters[ "DEF_FILE" ].icon = "def_file.png";
	mFilters[ "DEF_FILE" ].filters << "*.def";
	mFilters[ "DEF_FILE" ].type = DocumentFilter::File;
	mFilters[ "DEF_FILE" ].filtered = true;
	
	mFilters[ "RC_FILE" ].weight = weight++;
	mFilters[ "RC_FILE" ].label = QMakeProjectItem::tr( "RC Files" );
	mFilters[ "RC_FILE" ].icon = "rc_file.png";
	mFilters[ "RC_FILE" ].filters << "*.rc";
	mFilters[ "RC_FILE" ].type = DocumentFilter::File;
	mFilters[ "RC_FILE" ].filtered = true;
	
	mFilters[ "RES_FILE" ].weight = weight++;
	mFilters[ "RES_FILE" ].label = QMakeProjectItem::tr( "RES Files" );
	mFilters[ "RES_FILE" ].icon = "res_file.png";
	mFilters[ "RES_FILE" ].filters << "*.res";
	mFilters[ "RES_FILE" ].type = DocumentFilter::File;
	mFilters[ "RES_FILE" ].filtered = true;
	
	mFilters[ "SUBDIRS" ].weight = weight++;
	mFilters[ "SUBDIRS" ].label = QMakeProjectItem::tr( "Qt Sub Projects" );
	mFilters[ "SUBDIRS" ].icon = "project.png";
	mFilters[ "SUBDIRS" ].filters << mFilters[ "PROJECT" ].filters << mFilters[ "INCLUDE_PROJECT" ].filters;
	mFilters[ "SUBDIRS" ].type = DocumentFilter::File;
	mFilters[ "SUBDIRS" ].filtered = false;
	
	mFilters[ "OTHER_FILES" ].weight = weight++;
	mFilters[ "OTHER_FILES" ].label = QMakeProjectItem::tr( "Other Files" );
	mFilters[ "OTHER_FILES" ].icon = "file.png";
	mFilters[ "OTHER_FILES" ].filters << "*.txt" << "*README*" << "*Change*";
	mFilters[ "OTHER_FILES" ].type = DocumentFilter::File;
	mFilters[ "OTHER_FILES" ].filtered = true;
	
	//
	mFilters[ "INCLUDEPATH" ].weight = weight++;
	mFilters[ "INCLUDEPATH" ].label = QMakeProjectItem::tr( "Includes Paths" );
	mFilters[ "INCLUDEPATH" ].icon = "includepath.png";
	mFilters[ "INCLUDEPATH" ].filters.clear();
	mFilters[ "INCLUDEPATH" ].type = DocumentFilter::Path;
	mFilters[ "INCLUDEPATH" ].filtered = false; // was true
	
	mFilters[ "DEPENDPATH" ].weight = weight++;
	mFilters[ "DEPENDPATH" ].label = QMakeProjectItem::tr( "Depends Paths" );
	mFilters[ "DEPENDPATH" ].icon = "dependpath.png";
	mFilters[ "DEPENDPATH" ].filters.clear();
	mFilters[ "DEPENDPATH" ].type = DocumentFilter::Path;
	mFilters[ "DEPENDPATH" ].filtered = false; // was true
	
	mFilters[ "VPATH" ].weight = weight++;
	mFilters[ "VPATH" ].label = QMakeProjectItem::tr( "Virtuals Paths" );
	mFilters[ "VPATH" ].icon = "vpath.png";
	mFilters[ "VPATH" ].filters.clear();
	mFilters[ "VPATH" ].type = DocumentFilter::Path;
	mFilters[ "VPATH" ].filtered = false; // was true
	
	//
	mFilters[ "LIBS" ].weight = weight++;
	mFilters[ "LIBS" ].label = QMakeProjectItem::tr( "Libraries Files" );
	mFilters[ "LIBS" ].icon = "libs.png";
	mFilters[ "LIBS" ].filters.clear();
	mFilters[ "LIBS" ].type = DocumentFilter::Standard;
	mFilters[ "LIBS" ].filtered = false; // was true
	
	mFilters[ "DEFINES" ].weight = weight++;
	mFilters[ "DEFINES" ].label = QMakeProjectItem::tr( "Defines" );
	mFilters[ "DEFINES" ].icon = "defines.png";
	mFilters[ "DEFINES" ].filters.clear();
	mFilters[ "DEFINES" ].type = DocumentFilter::Standard;
	mFilters[ "DEFINES" ].filtered = false; // was true
	
	MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &QMakeProjectItem::staticMetaObject, mFilters );
	return true;
}

bool QMake::uninstall()
{
	MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
	mFilters.clear();
	delete mQtVersionManager;
	return true;
}

Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
