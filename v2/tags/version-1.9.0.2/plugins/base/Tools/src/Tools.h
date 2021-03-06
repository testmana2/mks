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
#ifndef TOOLS_H
#define TOOLS_H

#include <pluginsmanager/BasePlugin.h>

#include <QPointer>

class ToolsManager;

class Tools : public QObject, public BasePlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin )

protected:
    QPointer<ToolsManager> mToolsManager;
    
    virtual void fillPluginInfos();

    virtual bool install();
    virtual bool uninstall();
};

#endif // TOOLS_H
