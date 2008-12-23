/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : MonkeyCore.cpp
** Date      : 2008-01-14T00:36:51
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "MonkeyCore.h"
#include "../pMonkeyStudio.h"
#include "../settingsmanager/Settings.h"
#include "../pluginsmanager/PluginsManager.h"
#include "../maininterface/UIMain.h"
#include "../recentsmanager/pRecentsManager.h"
#include "../toolsmanager/pToolsManager.h"
#include "../xupmanager/gui/XUPProjectManager.h"
#include "../workspace/pFileManager.h"
#include "../workspace/pWorkspace.h"
#include "../consolemanager/pConsoleManager.h"
#include "../queuedstatusbar/QueuedStatusBar.h"

#include "../maininterface/ui/UISettings.h"
#include "../maininterface/ui/UITranslator.h"

#include <pIconManager.h>

#include <QSplashScreen>
#include <QPixmap>
#include <QString>
#include <QDate>

void showMessage( QSplashScreen* s, const QString& m )
{ s->showMessage( m, Qt::AlignRight | Qt::AlignBottom, s->property( "isXMas" ).toBool() ? Qt::red : Qt::white ); }

QHash<const QMetaObject*, QObject*> MonkeyCore::mInstances;

void MonkeyCore::init()
{
	// create splashscreen
	bool isXMas = QDate::currentDate().month() == 12;
	
	QSplashScreen splash( pIconManager::pixmap( isXMas ? "splashscreen_christmas.png" : "splashscreen.png", ":/application" ) );
	splash.setProperty( "isXMas", isXMas );
	
	QFont ft( splash.font() );
#ifndef Q_OS_WIN
	ft.setPointSize( ft.pointSize() -2 );
#endif
	ft.setBold( true );
	splash.setFont( ft );
	splash.show();
	
	// init pSettings
	pSettings::setIniInformations();
	
	// restore application style
	showMessage( &splash, tr( "Initializing Style..." ) );
	qApp->setStyle( settings()->value( "MainWindow/Style", "system" ).toString() );
	
	// set default settings if first time running
	if ( settings()->value( "FirstTimeRunning", true ).toBool() )
		settings()->setDefaultSettings();
	
	// init translation
	showMessage( &splash, tr( "Initializing Translation..." ) );
	if ( !settings()->value( "Translations/Accepted" ).toBool() )
		UITranslator::instance()->exec();
	pMonkeyStudio::loadTranslations();
	
	// init shortcuts editor
	showMessage( &splash, tr( "Initializing Actions Manager..." ) );
	MonkeyCore::actionsManager()->setSettings( settings() );
	
	// start console manager
	showMessage( &splash, tr( "Initializing Console..." ) );
	consoleManager();

	// init main window
	showMessage( &splash, tr( "Initializing Main Window..." ) );
	mainWindow()->initGui();
	
	// init pluginsmanager
	showMessage( &splash, tr( "Initializing Plugins..." ) );
	pluginsManager()->loadsPlugins();
	
	// restore window state
	showMessage( &splash, tr( "Restoring Workspace..." ) );
	mainWindow()->setSettings( settings() );
	
	// restore session
	showMessage( &splash, tr( "Restoring Session..." ) );
	if ( pMonkeyStudio::restoreSessionOnStartup() )
	{
		workspace()->fileSessionRestore_triggered();
	}
	
	// show main window
	mainWindow()->menu_Docks_aboutToShow();
	mainWindow()->show();
	
	// ready
	showMessage( &splash, tr( "%1 v%2 Ready !" ).arg( PACKAGE_NAME, PACKAGE_VERSION ) );

	// finish splashscreen
	splash.finish( mainWindow() );
	
	// show settings dialog the first time user start program
	if ( settings()->value( "FirstTimeRunning", true ).toBool() )
	{
		// execute settings dialog
		if ( UISettings::instance()->exec() )
			settings()->setValue( "FirstTimeRunning", false );
	}
	
	// prepare apis
	pMonkeyStudio::prepareAPIs();
}

Settings* MonkeyCore::settings()
{
	if ( !mInstances.contains( &Settings::staticMetaObject ) )
		mInstances[&Settings::staticMetaObject] = new Settings( qApp );
	return qobject_cast<Settings*>( mInstances[&Settings::staticMetaObject] );
}

PluginsManager* MonkeyCore::pluginsManager()
{
	if ( !mInstances.contains( &PluginsManager::staticMetaObject ) )
		mInstances[&PluginsManager::staticMetaObject] = new PluginsManager( qApp );
	return qobject_cast<PluginsManager*>( mInstances[&PluginsManager::staticMetaObject] );
}

UIMain* MonkeyCore::mainWindow()
{
	if ( !mInstances.contains( &UIMain::staticMetaObject ) )
		mInstances[&UIMain::staticMetaObject] = new UIMain();
	return qobject_cast<UIMain*>( mInstances[&UIMain::staticMetaObject] );
}

pMenuBar* MonkeyCore::menuBar()
{ return mainWindow()->menuBar(); }

pActionsManager* MonkeyCore::actionsManager()
{
	return menuBar()->actionsManager();
}

pRecentsManager* MonkeyCore::recentsManager()
{
	if ( !mInstances.contains( &pRecentsManager::staticMetaObject ) )
		mInstances[&pRecentsManager::staticMetaObject] = new pRecentsManager( qApp );
	return qobject_cast<pRecentsManager*>( mInstances[&pRecentsManager::staticMetaObject] );
}

pToolsManager* MonkeyCore::toolsManager()
{
	if ( !mInstances.contains( &pToolsManager::staticMetaObject ) )
		mInstances[&pToolsManager::staticMetaObject] = new pToolsManager( qApp );
	return qobject_cast<pToolsManager*>( mInstances[&pToolsManager::staticMetaObject] );
}

XUPProjectManager* MonkeyCore::projectsManager()
{
	if ( !mInstances.contains( &XUPProjectManager::staticMetaObject ) )
		mInstances[&XUPProjectManager::staticMetaObject] = new XUPProjectManager( mainWindow() );
	return qobject_cast<XUPProjectManager*>( mInstances[&XUPProjectManager::staticMetaObject] );
}

pFileManager* MonkeyCore::fileManager()
{
	if ( !mInstances.contains( &pFileManager::staticMetaObject ) )
		mInstances[&pFileManager::staticMetaObject] = new pFileManager( qApp );
	return qobject_cast<pFileManager*>( mInstances[&pFileManager::staticMetaObject] );
}

pWorkspace* MonkeyCore::workspace()
{
	if ( !mInstances.contains( &pWorkspace::staticMetaObject ) )
		mInstances[&pWorkspace::staticMetaObject] = new pWorkspace( mainWindow() );
	return qobject_cast<pWorkspace*>( mInstances[&pWorkspace::staticMetaObject] );
}

pConsoleManager* MonkeyCore::consoleManager()
{
	if ( !mInstances.contains( &pConsoleManager::staticMetaObject ) )
		mInstances[&pConsoleManager::staticMetaObject] = new pConsoleManager( qApp );
	return qobject_cast<pConsoleManager*>( mInstances[&pConsoleManager::staticMetaObject] );
}

QueuedStatusBar* MonkeyCore::statusBar()
{
	if ( !mInstances.contains( &QueuedStatusBar::staticMetaObject ) )
		mInstances[&QueuedStatusBar::staticMetaObject] = new QueuedStatusBar( mainWindow() );
	return qobject_cast<QueuedStatusBar*>( mInstances[&QueuedStatusBar::staticMetaObject] );
}
