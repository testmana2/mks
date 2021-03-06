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
/*!
    \file MessageBox.cpp
    \date 2008-01-14T00:40:08
    \author Andrei Kopats
    \brief implementation of main class of MessageBox plugin
*/
#include "MessageBox.h"
#include "ui/MessageBoxDocks.h"
#include "ui/UIMessageBoxSettings.h"

#include <coremanager/MonkeyCore.h>
#include <maininterface/UIMain.h>
#include <consolemanager/pConsoleManager.h>
#include <pDockToolBar.h>

#include <QIcon>
#include <QTabWidget>

void MessageBox::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "Message Box" );
    mPluginInfos.Description = tr( "Plugin for catching internal command console" );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.0.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = true;
    mPluginInfos.Pixmap = pIconManager::pixmap( "warning.png", ":/icons" );
}


/*!
    Install plugin
    
    If plugin is installed - it visible on main window and it's actions are in the main menu
    \return Status of process 
    \retval true Successfully enabled
    \retval false Some error ocurred
*/
bool MessageBox::install()
{
    // create actions
    pMenuBar* mb = MonkeyCore::menuBar();

    mb->beginGroup( "mView" );
        QAction* aErrorOrWarning = mb->action( "aShowNextErrorOrWarning", tr( "Next Error/Warning..." ), pIconManager::icon( "misc.png" ), QString::null, tr( "Show the next build step error/warning." ) );
        QAction* aWarning = mb->action( "aShowNextWarning", tr( "Next Warning..." ), pIconManager::icon( "warning.png", ":/icons" ), tr( "Ctrl+Shift++" ), tr( "Show the next build step warning." ) );
        QAction* aError = mb->action( "aShowNextError", tr( "Next Error..." ), pIconManager::icon( "error.png", ":/icons" ), tr( "Ctrl+Alt++" ), tr( "Show the next build step error." ) );
    mb->endGroup();
    
    // create docks
    mMessageBoxDocks = new MessageBoxDocks( this );
    
    // add docks to main window
    MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mMessageBoxDocks->mBuildStep, mMessageBoxDocks->mBuildStep->windowTitle(), mMessageBoxDocks->mBuildStep->windowIcon() );
    MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mMessageBoxDocks->mOutput, mMessageBoxDocks->mOutput->windowTitle(), mMessageBoxDocks->mOutput->windowIcon() );
    
    // connections
    connect( aErrorOrWarning, SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextErrorOrWarning() ) );
    connect( aWarning, SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextWarning() ) );
    connect( aError, SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextError() ) );
    connect( MonkeyCore::consoleManager(), SIGNAL( started() ), this, SLOT( onConsoleStarted() ) );
    
    return true;
}

/*!
    Unnstall plugin
    
    If plugin is installed - it visible on main window and it's actions are in the main menu
    \return Status of process 
    \retval true Successfully enabled
    \retval false Some error ocurred
*/
bool MessageBox::uninstall()
{
    // delete actions
    pMenuBar* mb = MonkeyCore::menuBar();

    mb->beginGroup( "mView" );
        QAction* aErrorOrWarning = mb->action( "aShowNextErrorOrWarning" );
        disconnect( aErrorOrWarning, SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextErrorOrWarning() ) );
        delete aErrorOrWarning;

        QAction* aWarning = mb->action( "aShowNextWarning" );
        disconnect( aWarning, SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextWarning() ) );
        delete aWarning;
        
        QAction* aError = mb->action( "aShowNextError" );
        disconnect( aError, SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextError() ) );
        delete aError;
    mb->endGroup();
    
    // disconnections
    disconnect( MonkeyCore::consoleManager(), SIGNAL( started() ), this, SLOT( onConsoleStarted() ) );
    
    // delete docks
    delete mMessageBoxDocks;
    
    return true;
}

/*!
    Get settings widget for configuring plugin
    \return Pointer to widget
*/
QWidget* MessageBox::settingsWidget() const
{ return new UIMessageBoxSettings( const_cast<MessageBox*>( this ) ); }

void MessageBox::onConsoleStarted()
{
    if ( settingsValue( "ActivateDock", true ).toBool() )
    {
        UIMessageBoxSettings::Dock dock = (UIMessageBoxSettings::Dock)settingsValue( "ActivatedDock", UIMessageBoxSettings::Output ).toInt();
        switch ( dock )
        {
            case UIMessageBoxSettings::BuildStep:
                mMessageBoxDocks->mBuildStep->show();
                break;
            case UIMessageBoxSettings::Output:
                mMessageBoxDocks->mOutput->show();
                break;
        }
    }
}

Q_EXPORT_PLUGIN2( BaseMessageBox, MessageBox )
