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
/*!
    \file GNUDebugger.cpp
    \date 14/08/08
    \author Xiantia
    \version 1.3.2
    \brief Main class, interface of pluginFactory
*/

#include "GNUDebugger.h"
#include "./ui/UIGNUDebuggerSetting.h"

#include <UIMain.h>
#include <MonkeyCore.h>
#include <Settings.h>
#include <pMonkeyStudio.h>
#include <QueuedStatusBar.h>

#include <QIcon>
    
/*!
        \brief Main class, interface of pluginFactory
        \details Create main DockGNUDebugger class adn configure this
*/
GNUDebugger::GNUDebugger()
{
    aSeparator = 0;
    // set plugin infos
    mPluginInfos.Caption = tr( "GNU Debugger" );
    mPluginInfos.Description = tr( "Plugin for Gdb intergration" );
    mPluginInfos.Author = "Pinon Yannick aka Xiantia  <private mail>";
    mPluginInfos.Type = BasePlugin::iDebugger;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.3.2";
    mPluginInfos.Enabled = false;

    // get the new instance of GdbPattermFile
    patternFile = GdbPatternFile::instance(this);
    GdbSetting::instance(this);
    GdbSetting::instance()->load();

    // find gdbParsing.txt

    // if you start Mks for the first time
    // setting is not found (monkeystudio.ini)
    if(GdbSetting::instance()->getPathParseFile().isEmpty())
    {
        // auto find file

        // load pattern
        QStringList pluginsPath = MonkeyCore::settings()->storagePaths( Settings::SP_PLUGINS );
        QFileInfoList files;
        // get all files in plugins path nammed 'gdbparsing.txt'
        QDir pluginsDir;
        for ( int i = 0; i < pluginsPath.count(); i++ )
        {
            QString path = pluginsPath.at( i );
            if ( QFileInfo( path ).isRelative() )
                path = QDir::cleanPath( QApplication::applicationDirPath() +"/" + path );
            pluginsDir.setPath( path );
            files << pMonkeyStudio::getFiles( pluginsDir, QString( "gdbparsing.txt" ), true );
        }

        
        if ( files.isEmpty())
            MonkeyCore::statusBar()->appendMessage( tr( "gdbparsing.txt not found. Debugger can not work ! " ) + GdbSetting::instance()->getPathParseFile(), 0 ,QPixmap(), QBrush(QColor(255,80,80)));
        else
        {
            GdbSetting::instance()->setPathParseFile( files.first().absoluteFilePath());
            patternFile->load( GdbSetting::instance()->getPathParseFile() );
        }
    }
    else
    {
        // setting is not empty
        // aptent load 
        // load txt file if possible, else warn user in status bar
        if ( ! patternFile->load( GdbSetting::instance()->getPathParseFile() ) )
            MonkeyCore::statusBar()->appendMessage( tr( "gdbparsing.txt not found. Debugger can not work ! " ) + GdbSetting::instance()->getPathParseFile(), 5000 ,QPixmap(), QBrush(QColor(255,80,80)));
    }
}

/*!
    \details Save current settings 
*/
GNUDebugger::~GNUDebugger()
{
    // save current setting
    GdbSetting::instance()->save();

    if ( isEnabled() )
        setEnabled( false );
}

/*!
    \details Set enabled or disable plugin
    \param b is trus for enabled plugin
*/
bool GNUDebugger::setEnabled( bool b )
{
    if ( b && !isEnabled() )
    {
        // get instance
        mDockGNUDebugger = DockGNUDebugger::instance();
        // add dock to dock toolbar entry
        MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDockGNUDebugger, infos().Caption, QIcon( pixmap() ) );
        // add actions to main window
        actionList["aLoadTarget"] = MonkeyCore::menuBar()->action( "mDebugger/aSelectTarget", tr( "Load target" ), QIcon( ":/icons/open.png" ) );
        actionList["aRestart"] = MonkeyCore::menuBar()->action( "mDebugger/aStartorrestart", tr( "Start or restart target" ), QIcon( ":/icons/update.png" ) );
        actionList["aContinue"] = MonkeyCore::menuBar()->action( "mDebugger/aContinue", tr( "Continue to next breakpoint" ), QIcon( ":/icons/continue.png" ) );
        actionList["aStepOver"] = MonkeyCore::menuBar()->action( "mDebugger/aStepover", tr( "Step over" ), QIcon( ":/icons/stepover.png" ) );
        actionList["aStepInto"] = MonkeyCore::menuBar()->action( "mDebugger/aStepinto", tr( "Step into" ), QIcon( ":/icons/stepinto.png" ) );
        actionList["aStepFinish"] = MonkeyCore::menuBar()->action( "mDebugger/aStepfinish", tr( "Step Finish" ), QIcon( ":/icons/stepfinish.png" ) );
        actionList["aExitGdb"] = MonkeyCore::menuBar()->action( "mDebugger/aQuit", tr( "Quit debug mode" ), QIcon( ":/icons/close.png" ) );
        // connections
        connect( actionList["aLoadTarget"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionLoadTarget() ) );
        connect( actionList["aRestart"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionRestart() ) );
        connect( actionList["aContinue"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionContinue() ) );
        connect( actionList["aStepOver"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionStepOver() ) );
        connect( actionList["aStepInto"] , SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionStepInto() ) );
        connect( actionList["aStepFinish"] , SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionStepFinish() ) );
        connect( actionList["aExitGdb"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionExit() ) );

        // init action disable
        actionList["aRestart"]->setEnabled( false );
        actionList["aContinue"]->setEnabled( false );
        actionList["aStepOver"]->setEnabled( false );
        actionList["aStepInto"]->setEnabled( false );
        actionList["aStepFinish"]->setEnabled( false );
        actionList["aExitGdb"]->setEnabled( false );

        // separator and add icon to toolbar
        aSeparator = MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction();
        foreach ( QAction* a, actionList )
            MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( a );

        // set action to gdbControl
        // enable / disable action
        mDockGNUDebugger->setAction( &actionList );
        // set plugin enabled
        mPluginInfos.Enabled = true;
    }
    else if ( !b && isEnabled() )
    {
        // delete actions
        qDeleteAll( actionList );
        delete aSeparator;
        actionList.clear();
        // delete dock
        delete mDockGNUDebugger;
        //DockGNUDebugger::instance()->deleteLater(); <- not work, more slow
        // set plugin disabled
        mPluginInfos.Enabled = false;
    }
    // return default value
    return true;
}

QPixmap GNUDebugger::pixmap() const
{ return QPixmap( ":/icons/debugger.png" ); }

void GNUDebugger::saveSettings()
{}

void GNUDebugger::restoreSettings()
{}

QWidget* GNUDebugger::settingsWidget()
{
    return new UIGNUDebuggerSetting;
}
// DebuggerPlugin
Q_EXPORT_PLUGIN2( BaseGNUDebugger, GNUDebugger )
