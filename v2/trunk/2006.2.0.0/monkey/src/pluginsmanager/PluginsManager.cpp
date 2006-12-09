#include "PluginsManager.h"
#include "ChildPlugin.h"
#include "WorkspacePlugin.h"
#include "CompilerPlugin.h"
#include "DebuggerPlugin.h"
#include "Workspace.h"
//
#include <QPluginLoader>
#include <QApplication>
//
QPointer<PluginsManager> PluginsManager::mSelf = 0L;
//
PluginsManager* PluginsManager::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new PluginsManager( p );
	return mSelf;
}
//
PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{
}
//
PluginsManager::~PluginsManager()
{
}
//
void PluginsManager::loadsPlugins( const QString& s )
{
	// load static plugins
	foreach ( QObject* o, QPluginLoader::staticInstances() )
		if ( !addPlugin( o ) )
			qWarning( qPrintable( tr( "Failed to load static plugin" ) ) );
	// dor application path
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
	// go into plugins dir
	d.cd( "plugins" );
	// check all subdirs to load plugins
	loadsPlugins( d );
}
//
void PluginsManager::loadsPlugins( QDir d )
{
	// looking fodlers to load
	foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
	{
		d.cd( s );
		loadsPlugins( d );
		d.cdUp();
	}
	// looking files to load
	foreach ( QString s, d.entryList( QDir::Files ) )
	{
		QPluginLoader l( d.absoluteFilePath( s ) );
		if ( !addPlugin( l.instance() ) )
			qWarning( qPrintable( tr( "Failed to load plugin: %1" ).arg( s ) ) );
	}
}
//
bool PluginsManager::addPlugin( QObject* o )
{
	BasePlugin* p = qobject_cast<BasePlugin*>( o );
	if ( !p )
		return false;
	//
	p->initialize( Workspace::self() );
	if ( !p->isInstalled() && !p->install() )
		qWarning( qPrintable( tr( "Failed to install plugin: %1" ).arg( p->name() ) ) );
	//
	qWarning( "Plugin: %s, type: %d", qPrintable( p->name() ), p->type() );
	mPlugins << p;
	return true;
}
//
bool PluginsManager::childPluginOpenFile( const QString& s, AbstractProject* p )
{
	QString mExtension = QFileInfo( s ).completeSuffix();
	foreach ( BasePlugin* bp, mPlugins )
	{
		if ( bp->type() == BasePlugin::iChild )
		{
			ChildPlugin* cp = (ChildPlugin*)bp;
			if ( cp && cp->extensions().contains( mExtension, Qt::CaseInsensitive ) )
				return cp->openFile( s, p );
		}
	}
	return false;
}
//
QStringList PluginsManager::childsFilters() const
{
	QStringList l;
	foreach ( BasePlugin* bp, mPlugins )
	{
		if ( bp->type() == BasePlugin::iChild )
		{
			ChildPlugin* cp = (ChildPlugin*)bp;
			if ( cp )
				l << cp->filters();
		}
	}
	return l;
}
