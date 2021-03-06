/****************************************************************************
**
**      Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : Entity.cpp
** Date      : 2008-01-14T00:40:05
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
/*!
    \file Entity.cpp
    \date 2008-01-14T00:40:08
    \author Andrei Kopats
    \brief Implementation of Entity class
*/

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QToolTip>
#include <QDateTime>
#include <QFileInfo>

#include "Entity.h"

QIcon Entity::iNAMESPACE;
QIcon Entity::iCLASS;
QIcon Entity::iENUM;
QIcon Entity::iFUNCTION;
QIcon Entity::iMEMBER;
QIcon Entity::iSTRUCT;
QIcon Entity::iTYPEDEF;
QIcon Entity::iMACRO;
QIcon Entity::iENUMERATOR;

/*!
    Class constructor. Will read initial info to self.
    \param entry Tag entry, from which Entity should be constructed
    \param fileName File, where entity containing
    \param time Time of last update of entity (update of tag for entity)
*/
Entity::Entity ( tagEntryInfo* entry, QString fileName, QDateTime time)
{
    updateSelf (entry, fileName, time);
}

/*!
    Class constructor. Will create entity be not full info.

    This construstor using for Entityes, for which currenlty are no full info
    \param ttype Entity type. Possible it will be UNKNOWN
    \param tname Name of entity
*/
Entity::Entity (EntityType ttype, QString tname)
{
    type = ttype; 
    name = tname;
    line = 0;
    file = QString::null;
    updateTime = QDateTime();
    updateGUI ();
}

/*!
    Load static icons, which using by all members of class
*/
void Entity::initIcons ()
{
    iNAMESPACE = QIcon(":/icons/NAMESPACE.png");
    iCLASS = QIcon (":/icons/CLASS.png");
    iENUM = QIcon (":/icons/ENUM.png");
    iFUNCTION = QIcon (":/icons/FUNCTION.png");
    iMEMBER = QIcon (":/icons/MEMBER.png");
    iSTRUCT = QIcon (":/icons/STRUCT.png");
    iTYPEDEF = QIcon (":/icons/TYPEDEF.png");
    iMACRO = QIcon (":/icons/MACRO.png");
    iENUMERATOR = QIcon (":/icons/ENUMERATOR.png");
}

/*!
    Get child of entity (according with model) as pointer to Entity instance
    \param i Child number
*/
Entity* Entity::child (int i)
{
    return (Entity*)QTreeWidgetItem::child(i);
}

/*!
    Converts kind of entity be Exuberant Ctags encoding to EntityType
    \param kind Ctags type varible
    \return Entity type
    \retval EntityType variable
*/
EntityType Entity::getEntityType (char kind)
{
    switch (kind)
    {
        case 'c':
            return CLASS;
        case 'C':
            return CONSTRUCTOR;
        case 'd':
            return MACRO;
        case 'D':
            return DESTRUCTOR;
        case 'e':
            return ENUMERATOR;
        case 'f':
            return FUNCTION;
        case 'g':
            return ENUM;
        case 'm':
            return MEMBER;
        case 'n':
            return NAMESPACE;
        case 'p':
            return PROTOTYPE;
        case 's':
            return STRUCT;
        case 't':
            return TYPEDEF;
        case 'u':
            return UNION;
        case 'v':
            return VARIABLE;
        case 'x':
            return EXTERN;
        default:
            qWarning ( qPrintable (QString ("finded unknown entity type ") + kind));
            return UNKNOWN;
    }
}

/*!
    Converts kind of scope in Ctags encoding to type of Entity, which representing scope
    \param kindName Ctags type variable
    \return Entity type
    \retval EntityType variable
*/
EntityType Entity::getScopeType ( QString kindName )
{
    if ( kindName == "class")
        return CLASS;
    else if ( kindName == "struct")
        return STRUCT;
    else if ( kindName == "enum")
        return ENUM;
    else if (kindName == "namespace")
        return NAMESPACE;
    else if (kindName == "union")
        return UNION;
    else
        qWarning ( qPrintable ("Finded unknown scope type "+kindName ));
    return UNKNOWN;
}

/*!
    Cleanup information about file, which are older, than some time limit
    Used for updating information without recreation of tree
        1) Add/replace current information
        2) Remove old
    Will process reqursively all childs
    \param file File, for which information should be deleted
    \param olderThan time stamp. Information (Entityes), which have older time should be deleted
*/
void Entity::deleteFileInfo ( QString file, QDateTime olderThan )
{
    Entity* chld;
    for (int i = 0; i<childCount(); i++)
    {
        chld = child (i);
        chld->deleteFileInfo ( file, olderThan );
        if ( chld->file == file && 
            (  chld->updateTime < olderThan || olderThan.isNull() )) //need to delete this entity
        {
            //if entity was have childs, that must be deleted, it's had deleted by chld->deleteFileInfo (... )
            //if entity have childs now, it must not be deleted.
            if ( chld->childCount() == 0 )
            {
                delete ( chld);
                i--;
            }
            else //we can't destroy item, having childs, because childs must have parent
            {
                file = QString::null;
                chld->line = 0;//will not try to show null line
                chld->updateTime = QDateTime (); //null time
                chld->signature = QString::null; //without signature
            }
        }
    }
}

/*!
    Read own information from Tag
    \param entry Tag, containing information about self
    \param fileName Name of file, containing self
    \param time Time stamp of information
*/
void Entity::updateSelf (tagEntryInfo* entry, QString fileName, QDateTime time)
{
    name = entry->name;  //  do possible make this assigmengs automaticaly ???
    file = fileName;
    type = getEntityType (entry->kind );
    line = entry->lineNumber;
    varType = entry->extensionFields.typeRef [0];
    updateTime = time;
    if ( entry->extensionFields.signature != NULL )
    {    
        signature = entry->extensionFields.signature;
    }
    updateGUI ();
}

/*!
    Update own text and icon according with internal information
*/
void Entity::updateGUI ()
{
    QString tip;
    QString newText = name;
    switch (type )
    {
        case NAMESPACE :
            setIcon (0,iNAMESPACE);
            tip = "Namespace";
            newText = name+ " namespace";
            break;
        case CLASS:
            setIcon (0,iCLASS);
            tip = "Class";
            newText = name + " class";
            break;
        case CONSTRUCTOR:
            tip = "Constructor";
            break;
        case DESTRUCTOR:
            tip = "Destructor";
            break;
        case ENUM:
            tip = "Enum";
            setIcon (0,iENUM);
            newText = name + " enum";
            break;
        case ENUMERATOR:
            setIcon (0,iENUMERATOR);
            break;
        case EXTERN:
            tip = "Extern";
            break;
        case FUNCTION:
        case PROTOTYPE:
            setIcon (0,iFUNCTION);
            break;
        case MACRO:
            setIcon (0,iMACRO);
            break;
        case MEMBER:
        case VARIABLE:
            tip = QString("<i>%1</i>").arg(varType);
            setIcon (0,iMEMBER);
            break;
        case STRUCT:
            tip = "Struct";
            setIcon (0,iSTRUCT);
            newText = name + " struct";
            break;
        case TYPEDEF:
            tip = "Typedef";
            setIcon (0,iTYPEDEF);
            newText = name + " typedef";
            break;
        case UNION:
            tip = "Union";
            newText = name + " union";
            break;
        case UNKNOWN:
            break;
        default:
            qDebug ("Not right entity type in the constructor");
    }
    tip =    tip+
            ' '+
            name + signature + 
            "<br>";
    if ( line != 0)
        tip = tip + QFileInfo (file).fileName() + QString(":%1").arg(line);
    else
        tip += "Location of this element unknown";
    setText ( 0, newText + signature);
    setToolTip (0,tip);
}
