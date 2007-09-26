#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "pMenuBar.h"
#include "UIPluginsSettings.h"

#include <QPluginLoader>

PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{
	mCompiler = 0L;
	mDebugger = 0L;
	mInterpreter = 0L;
}

QList<BasePlugin*> PluginsManager::plugins() const
{ return mPlugins; }

ProjectItem* PluginsManager::projectItem( const QString& s )
{
	foreach ( ProjectPlugin* pp, plugins<ProjectPlugin*>( "", BasePlugin::iProject ) )
		foreach ( QString k, pp->suffixes().keys() )
			if ( QDir::match( pp->suffixes().value( k ), s ) )
				return pp->getProjectItem( s );
	return 0;
}

void PluginsManager::loadsPlugins( const QString& s )
{
	// get application path
	QDir d( s.isEmpty() ? qApp->applicationDirPath() : s );
#if defined( Q_OS_WIN )
	// move up if in debug/release folder
	if ( d.dirName().toLower() == "debug" || d.dirName().toLower() == "release" )
		d.cdUp();
#elif defined( Q_OS_MAC )
	// move up 3 times if in MacOS folder
	if ( d.dirName() == "MacOS" )
	{
		d.cdUp();
		d.cdUp();
		d.cdUp();
	}
#endif
	
	// go to plugins directory
	d.cd( "plugins" );
	
	// loads static plugins
	foreach ( QObject* o, QPluginLoader::staticInstances() )
		if ( !addPlugin( o ) )
			qWarning( qPrintable( tr( "Failed to load static plugin" ) ) );
		
	// load all plugins
	foreach ( QFileInfo f, pMonkeyStudio::getFiles( d ) )
	{
		QPluginLoader l( f.absoluteFilePath() );
		if ( !addPlugin( l.instance() ) )
		{
			// try unload it and reload it in case of old one in memory
			l.unload();
			l.load();
			
			// if can t load it, check next
			if ( !addPlugin( l.instance() ) )			
				qWarning( qPrintable( tr( "Failed to load plugin ( %1 ): Error: %2" ).arg( f.absoluteFilePath(), l.errorString() ) ) );
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
		if ( !pSettings::instance()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() )
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

void PluginsManager::setCurrentCompiler( CompilerPlugin* c )
{
	// if same cancel
	if ( mCompiler == c )
		return;
	
	// disabled all compiler
	foreach ( BasePlugin* bp, plugins<BasePlugin*>( s, BasePlugin::iCompiler ) )
		bp->setEnabled( false );
	
	// enabled the one we choose
	mCompiler = c;
	if ( mCompiler )
		mCompiler->setEnabled( true );
	
	// enable menu according to current compiler
	pMenuBar::instance()->menu( "mBuild" )->setEnabled( mCompiler );
}

const QString PluginsManager::currentCompiler()
{ return mCompiler; }
	
void PluginsManager::setCurrentDebugger( DebuggerPlugin* d )
{
	// if same cancel
	if ( mDebugger == d )
		return;
	
	// disabled all debugger
	foreach ( BasePlugin* bp, plugins<BasePlugin*>( s, BasePlugin::iDebugger ) )
		bp->setEnabled( false );
	
	// enabled the one we choose
	mCompiler = c;
	if ( mCompiler )
		mCompiler->setEnabled( true );
	
	// enable menu according to current compiler
	pMenuBar::instance()->menu( "mBuild" )->setEnabled( mCompiler );
}

const QString PluginsManager::currentDebugger()
{
	foreach ( BasePlugin* bp, plugins<BasePlugin*>( QString::null, BasePlugin::iDebugger ) )
		if ( bp->isEnabled() )
			return bp->infos().Name;
	return QString();
}
	
void PluginsManager::setCurrentInterpreter( const QString& s )
{
	foreach ( BasePlugin* bp, plugins<BasePlugin*>( s, BasePlugin::iInterpreter ) )
		bp->setEnabled( false );
	if ( BasePlugin* bp = plugin<BasePlugin*>( s, BasePlugin::iInterpreter ) )
		bp->setEnabled( true );
	
	mb->menu( "mInterpreter" )->setEnabled( !pm->currentInterpreter().isEmpty() );
}

const QString PluginsManager::currentInterpreter()
{
	foreach ( BasePlugin* bp, plugins<BasePlugin*>( QString::null, BasePlugin::iInterpreter ) )
		if ( bp->isEnabled() )
			return bp->infos().Name;
	return QString();
}

void PluginsManager::manageRequested()
{
	UIPluginsSettings::instance()->exec(); 
}
