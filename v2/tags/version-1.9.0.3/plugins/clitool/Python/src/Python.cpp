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
#include "Python.h"

void Python::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "Python" );
    mPluginInfos.Description = tr( "This plugin provide Python interpreter and python parser." );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Michon Aurelien aka aurelien <aurelien.french@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iCLITool;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.1.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = true;
    mPluginInfos.Pixmap = pIconManager::pixmap( "python.png", ":/icons" );
}

bool Python::install()
{
    return true;
}

bool Python::uninstall()
{
    return true;
}

pCommand Python::defaultCommand() const
{
    const QString python = "python";
    pCommand cmd( "Interpret", python, false, availableParsers(), "$cpp$" );
    cmd.setName( PLUGIN_NAME );
    return cmd;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( InterpreterPython, Python )
#endif
