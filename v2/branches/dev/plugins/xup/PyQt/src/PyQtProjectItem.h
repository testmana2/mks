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
#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#include "xupmanager/core/XUPProjectItem.h"

class CLIToolPlugin;

class PyQtProjectItem : public XUPProjectItem
{
    Q_OBJECT

public:
    Q_INVOKABLE PyQtProjectItem();
    
    virtual QString projectType() const;
    virtual void installCommands();
    
protected:
    enum Binary {
        Python,
        LRelease,
        LUpdate,
        Rcc,
        Uic
    };
    
    CLIToolPlugin* interpreter() const;
    
    QString pyQtFilePath() const;
    QString pyQtBinaryFilePath( PyQtProjectItem::Binary binary ) const;
};

#endif // PYQTPROJECTITEM_H
