/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#include "pluginsmanager/PluginsManager.h"
#include "PluginsMenu.h"
#include "pMonkeyStudio.h"
#include "ui/UIPluginsSettings.h"
#include "coremanager/MonkeyCore.h"
#include "main.h"

#include <pVersion.h>

#include <QPluginLoader>

#include <QDebug>

PluginsManager::PluginsManager( QObject* p )
    : QObject( p )
{
    mMenuHandler = new PluginsMenu( this );
}

QList<BasePlugin*> PluginsManager::plugins() const
{ return mPlugins; }

void PluginsManager::loadsPlugins()
{
    // loads static plugins
    foreach ( QObject* o, QPluginLoader::staticInstances() )
        if ( !addPlugin( o ) )
            qWarning("%s", tr( "Failed to load static plugin" ).toLocal8Bit().constData() );
    // load dynamic plugins
    QDir d;
    foreach ( const QString s, MonkeyCore::settings()->storagePaths( Settings::SP_PLUGINS ) )
    {
        d.setPath( QDir::isRelativePath( s ) ? qApp->applicationDirPath().append( "/%1" ).arg( s ) : s );
        // load all plugins
        foreach ( const QFileInfo& f, pMonkeyStudio::getFiles( d ) )
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
                qWarning("%s", tr( "Failed to load plugin ( %1 ): Error: %2" ).arg( f.absoluteFilePath(), l.errorString() ).toLocal8Bit().constData() );
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
    
    // generally it should be called from constructor, but can't call virtual method
    bp->fillPluginInfos();
    
    // inforce application minimum requirement
    const pVersion appVersion( PACKAGE_VERSION );
    const pVersion pluginVersion( bp->infos().ApplicationVersionRequired );
    
    if ( appVersion < pluginVersion )
    {
        qWarning( "Uncompatible plugin %s: require version %s, found version %s", bp->infos().Name.toLocal8Bit().constData(), pluginVersion.toString().toLocal8Bit().constData(), appVersion.toString().toLocal8Bit().constData() );
        return false;
    }

    // check dupplicates
    foreach ( BasePlugin* p, mPlugins )
    {
        if ( p->infos().Name == bp->infos().Name )
        {
            qWarning("%s", tr( "Skipping duplicate plugin: %1, type: %2" ).arg( p->infos().Name ).arg( p->infos().Type ).toLocal8Bit().constData() );
            return false;
        }
    }
    
    // show plugin infos
    qWarning("%s", tr( "Found plugin: %1, type: %2" ).arg( bp->infos().Name ).arg( bp->infos().Type ).toLocal8Bit().constData() );
    
    // add it to plugins list, AppDebug is prepend as first plugin to allow track quicker
    if ( bp->infos().Name == "AppDebug" ) {
        mPlugins.prepend( bp );
    }
    else {
        mPlugins << bp;
    }
    
    mMenuHandler->addPlugin( bp );
    
    // return
    return true;
}

void PluginsManager::enableUserPlugins()
{
    foreach ( BasePlugin* bp, mPlugins )
    {
        // check first start state
        if ( MonkeyCore::settings()->value( "FirstTimeRunning", true ).toBool() )
        {
            if ( !bp->infos().FirstStartEnabled )
            {
                MonkeyCore::settings()->setValue( QString( "Plugins/%1" ).arg( bp->infos().Name ), false );
            }
        }
        
        // check in settings if we must install this plugin
        if ( !MonkeyCore::settings()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() )
        {
            qWarning("%s", tr( "User wantn't to intall plugin: %1" ).arg( bp->infos().Name ).toLocal8Bit().constData() );
        }
        // if not enabled, enable it
        else if ( !bp->isEnabled() )
        {
            if ( bp->setEnabled( true ) )
            {
                qWarning("%s", tr( "Successfully enabled plugin: %1" ).arg( bp->infos().Name ).toLocal8Bit().constData() );
            }
            else
            {
                qWarning("%s", tr( "Unsuccessfully enabled plugin: %1" ).arg( bp->infos().Name ).toLocal8Bit().constData() );
            }
        }
        else
        {
            qWarning("%s", tr( "Already enabled plugin: %1" ).arg( bp->infos().Name ).toLocal8Bit().constData() );
        }
    }
}

pAbstractChild* PluginsManager::documentForFileName( const QString& fileName )
{
    foreach ( ChildPlugin* plugin, plugins<ChildPlugin*>( PluginsManager::stEnabled ) )
    {
        pAbstractChild* document = plugin->createDocument( fileName );
        
        if ( document )
        {
            return document;
        }
    }
    
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

void PluginsManager::manageRequested()
{ ( new UIPluginsSettings() )->show(); }

void PluginsManager::clearPlugins()
{
    BasePlugin* appDebug = plugin<BasePlugin*>( stAll, "AppDebug" );
    
    if ( appDebug ) {
        mPlugins.move( mPlugins.indexOf( appDebug ), mPlugins.count() -1 );
    }
    
    foreach ( BasePlugin* bp, mPlugins )
    {
        qWarning( "Clearing plugin...%s", bp->infos().Name.toLocal8Bit().constData() );
        bp->setEnabled( false );
    }
    
    qDeleteAll( mPlugins );
    mPlugins.clear();
}
