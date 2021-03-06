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
#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include <MonkeyExport.h>
#include <pIconManager.h>

#include "main.h"
#include "coremanager/MonkeyCore.h"
#include "settingsmanager/Settings.h"

#include <QtPlugin>
#include <QPointer>
#include <QAction>

#ifdef __COVERAGESCANNER__
#include <QCoreApplication>
#include <QDir>
#endif

class Q_MONKEY_EXPORT BasePlugin : public QObject
{
    Q_OBJECT
    Q_ENUMS( Type )
    
    friend class PluginsManager; // for call fillPluginInfo()
    
public:
    // plugin type enums
    enum Type
    {
        iBase = 0x0,
        iChild = 0x1,
        iCLITool = 0x2,
        iDebugger = 0x4,
        iXUP = 0x20,
        iLast = 0x80
    };
    
    Q_DECLARE_FLAGS( Types, Type )
    
    // plugin info structure
    struct Q_MONKEY_EXPORT PluginInfos
    {
        PluginInfos()
        {
            Type = BasePlugin::iBase;
            FirstStartEnabled = false;
            HaveSettingsWidget = false;
            Pixmap = pIconManager::pixmap( "monkey2.png", ":/application" );
            ApplicationVersionRequired = PACKAGE_VERSION;
        }
        
        QString Caption; // the string to show as caption
        QString Description; // the plugin description
        QString Author; // the plugin author
        BasePlugin::Types Type; // the plugin type ( can be or-ded )
        QStringList Languages; // language this plugin is for, default empty mean all
        QString Name; // the plugin name for version control
        QString Version; // the plugin version for version control
        QString License; // the plugin license
        bool FirstStartEnabled; // to know if this plugin is enabled
        bool HaveSettingsWidget; // plugin has settings widget
        QPixmap Pixmap; // plugin icon
        QString ApplicationVersionRequired; // the minimum mks version this plugin require, plugin must not enable itself if minimum version is not reached !
        QStringList dependencies; // the plugin to enable as dependency for this plugin
    };
    
    BasePlugin();
    virtual ~BasePlugin();
    
    virtual PluginInfos infos() const
    { return mPluginInfos; };
    
    static QString typeToString( BasePlugin::Type type );   
    static QString completeTypeToString( BasePlugin::Types type );
    
    QString captionVersionString() const;
    
    QAction* stateAction() const;
    
    bool neverEnable() const
    { return settingsValue( "NeverEnable", false ).toBool(); }
    
    void setNeverEnable( bool never )
    { return setSettingsValue( "NeverEnable", never ); }
    
    virtual QWidget* settingsWidget() const
    { return 0; }
    
    virtual bool isEnabled() const
    { return stateAction()->isChecked(); }
    
    // Method does some work and calls install() or uninstall()
    virtual bool setEnabled( bool );
    
    virtual QString settingsKey( const QString& key ) const;
    virtual QVariant settingsValue( const QString& key, const QVariant& value = QVariant() ) const;
    virtual void setSettingsValue( const QString& key, const QVariant& value ) const;
    
    // coverage support members
#ifdef __COVERAGESCANNER__
    virtual void saveCodeCoverage( const QString& n, const QString& s );
#endif
    
protected:
    mutable QPointer<QAction> mAction;
    PluginInfos mPluginInfos;
    
    // Internal functions for install/uninstall plugin. Called by setEnabled()
    virtual bool install() = 0;
    virtual bool uninstall() = 0;
    
    // Must be implemented by plugin, fills plugin info */
    virtual void fillPluginInfos() = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( BasePlugin::Types )
Q_DECLARE_METATYPE( BasePlugin* )
Q_DECLARE_METATYPE( BasePlugin::PluginInfos )
Q_DECLARE_INTERFACE( BasePlugin, "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" )

#endif // BASEPLUGIN_H
