/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : PluginsManager.cpp
** Date      : 2008-01-14T00:37:01
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
#include "PluginsManager.h"
#include "../pMonkeyStudio.h"
#include "ui/UIPluginsSettings.h"
#include "../coremanager/MonkeyCore.h"

#include <QPluginLoader>

#include <QDebug>

PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{
	mBuilder = 0;
	mCompiler = 0;
	mDebugger = 0;
	mInterpreter = 0;
}

QList<BasePlugin*> PluginsManager::plugins() const
{ return mPlugins; }

void PluginsManager::loadsPlugins()
{
	// loads static plugins
	foreach ( QObject* o, QPluginLoader::staticInstances() )
		if ( !addPlugin( o ) )
			qWarning( qPrintable( tr( "Failed to load static plugin" ) ) );
	// load dynamic plugins
	QDir d;
	foreach ( const QString s, MonkeyCore::settings()->storagePaths( Settings::SP_PLUGINS ) )
	{
		d.setPath( QDir::isRelativePath( s ) ? qApp->applicationDirPath().append( "/%1" ).arg( s ) : s );
		// load all plugins
		foreach ( QFileInfo f, pMonkeyStudio::getFiles( d ) )
		{
			// don't proced no library file
			if ( !QLibrary::isLibrary( f.absoluteFilePath() ) )
				continue;
#ifdef Q_OS_MAC
			// don't proceed Qt plugins on mac
			if ( f.absoluteFilePath().contains( "/qt/" ) )
				continue;
#endif
			// load plugin
			QPluginLoader l( f.absoluteFilePath() );
			// try unload it and reload it in case of old one in memory
			if ( !l.instance() )
			{
				l.unload();
				l.load();
			}
			// continue on no plugin
			if ( !l.instance() )
			{
				qWarning( qPrintable( tr( "Failed to load plugin ( %1 ): Error: %2" ).arg( f.absoluteFilePath(), l.errorString() ) ) );
				continue;
			}
			// try to add plugin to plugins list, else unload it
			else if ( !addPlugin( l.instance() ) )
				l.unload();
		}
	}
	// installs user requested plugins
	enableUserPlugins();
}

bool PluginsManager::addPlugin( QObject* o )
{
	// try to cast instance to BasePlugin
	BasePlugin* bp = qobject_cast<BasePlugin*>( o );
	
	// if not return
	if ( !bp )
		return false;

	// check dupplicates
	foreach ( BasePlugin* p, mPlugins )
	{
		if ( p->infos().Name == bp->infos().Name )
		{
			qWarning( qPrintable( tr( "Skipping duplicate plugin: %1, type: %2" ).arg( p->infos().Name ).arg( p->infos().Type ) ) );
			return false;
		}
	}
	
	// show plugin infos
	qWarning( qPrintable( tr( "Found plugin: %1, type: %2" ).arg( bp->infos().Name ).arg( bp->infos().Type ) ) );
	
	// add it to plugins list
	mPlugins << bp;
	
	// return
	return true;
}

void PluginsManager::enableUserPlugins()
{
	foreach ( BasePlugin* bp, mPlugins )
	{		
		// check in settings if we must install this plugin
		if ( !MonkeyCore::settings()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() )
			qWarning( qPrintable( tr( "User wantn't to intall plugin: %1" ).arg( bp->infos().Name ) ) );
		// if not enabled, enable it
		else if ( !bp->isEnabled() )
		{
			if ( bp->setEnabled( true ) )
				qWarning( qPrintable( tr( "Successfully enabled plugin: %1" ).arg( bp->infos().Name ) ) );
			else
				qWarning( qPrintable( tr( "Unsuccessfully enabled plugin: %1" ).arg( bp->infos().Name ) ) );
		}
		else
			qWarning( qPrintable( tr( "Already enabled plugin: %1" ).arg( bp->infos().Name ) ) );
	}
}

pAbstractChild* PluginsManager::openChildFile( const QString& s )
{
	foreach ( ChildPlugin* cp, plugins<ChildPlugin*>( PluginsManager::stEnabled ) )
		if ( cp->canOpen( s ) )
			return cp->openFile( s );
	return 0;
}

QMap<QString, QStringList> PluginsManager::childSuffixes() const
{
	QMap<QString, QStringList> l;
	foreach ( ChildPlugin* cp, const_cast<PluginsManager*>( this )->plugins<ChildPlugin*>( PluginsManager::stEnabled ) )
		foreach ( QString k, cp->suffixes().keys() )
			l[k] << cp->suffixes().value( k );
	return l;
}

QString PluginsManager::childFilters() const
{
	QString f;
	QMap<QString, QStringList> l = childSuffixes();
	foreach ( QString k, l.keys() )
		f += QString( "%1 (%2);;" ).arg( k ).arg( l.value( k ).join( " " ) );
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	return f;
}

void PluginsManager::setCurrentBuilder( BuilderPlugin* b )
{
	// if same cancel
	if ( mBuilder == b )
		return;
	
	// disabled all builder
	foreach ( BuilderPlugin* bp, plugins<BuilderPlugin*>( PluginsManager::stAll ) )
		bp->setEnabled( false );
	
	// enabled the one we choose
	mBuilder = b;
	if ( mBuilder )
		mBuilder->setEnabled( true );
	
	// enable menu according to current builder
	MonkeyCore::menuBar()->menu( "mBuilder" )->setEnabled( mBuilder || mCompiler );
	MonkeyCore::menuBar()->menu( "mBuilder" )->menuAction()->setVisible( mBuilder || mCompiler );
}

BuilderPlugin* PluginsManager::currentBuilder()
{ return mBuilder; }

void PluginsManager::setCurrentCompiler( CompilerPlugin* c )
{
	// if same cancel
	if ( mCompiler == c )
		return;
	
	// disabled all builder
	foreach ( CompilerPlugin* cp, plugins<CompilerPlugin*>( PluginsManager::stAll ) )
		cp->setEnabled( false );
	
	// enabled the one we choose
	mCompiler = c;
	if ( mCompiler )
		mCompiler->setEnabled( true );
	
	// enable menu according to current compiler
	MonkeyCore::menuBar()->menu( "mBuilder" )->setEnabled( mCompiler || mBuilder );
	MonkeyCore::menuBar()->menu( "mBuilder" )->menuAction()->setVisible( mBuilder || mCompiler );
}

CompilerPlugin* PluginsManager::currentCompiler()
{ return mCompiler; }

void PluginsManager::setCurrentDebugger( DebuggerPlugin* d )
{
	// if same cancel
	if ( mDebugger == d )
		return;
	
	// disabled all debugger
	foreach ( DebuggerPlugin* dp, plugins<DebuggerPlugin*>( PluginsManager::stAll ) )
		dp->setEnabled( false );
	
	// enabled the one we choose
	mDebugger = d;
	if ( mDebugger )
		mDebugger->setEnabled( true );
	
	// enable menu according to current debugger
	MonkeyCore::menuBar()->menu( "mDebugger" )->setEnabled( mDebugger );
	MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setVisible( mDebugger );
}

DebuggerPlugin* PluginsManager::currentDebugger()
{ return mDebugger; }
	
void PluginsManager::setCurrentInterpreter( InterpreterPlugin* i )
{
	// if same cancel
	if ( mInterpreter == i )
		return;
	
	// disabled all debugger
	foreach ( InterpreterPlugin* ip, plugins<InterpreterPlugin*>( PluginsManager::stAll ) )
		ip->setEnabled( false );
	
	// enabled the one we choose
	mInterpreter = i;
	if ( mInterpreter )
		mInterpreter->setEnabled( true );
	
	// enable menu according to current interpreter
	MonkeyCore::menuBar()->menu( "mInterpreter" )->setEnabled( mInterpreter );
	MonkeyCore::menuBar()->menu( "mInterpreter" )->menuAction()->setVisible( mInterpreter );
}

InterpreterPlugin* PluginsManager::currentInterpreter()
{ return mInterpreter; }

void PluginsManager::manageRequested()
{ ( new UIPluginsSettings() )->show(); }

void PluginsManager::clearPlugins()
{
	foreach ( BasePlugin* bp, mPlugins )
	{
		qWarning( "Clearing plugin...%s", qPrintable( bp->infos().Name ) );
		bp->setEnabled( false );
	}
	qDeleteAll( mPlugins );
	mPlugins.clear();
}
