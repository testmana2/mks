#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
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

Entity::Entity ( sTagEntryInfo* entry, QString fileName, QDateTime time)
{
    updateSelf (entry, fileName, time);
}

Entity::Entity (EntityType ttype, QString tname)
{
    type = ttype; 
    name = tname;
    line = 0;
    file = QString::null;
    updateTime = QDateTime();
    updateGUI ();
}


void Entity::initIcons ()
{
    iNAMESPACE = QIcon(":/icons/NAMESPACE.png");
    iCLASS = QIcon (":/icons/CLASS.png");
    iENUM = QIcon (":/icons/ENUM.png");
    iFUNCTION = QIcon (":/icons/FUNCTION.png");
    iMEMBER = QIcon (":/icons/MEMBER.png");
    iSTRUCT = QIcon (":/icons/STRUCT.png");
    iTYPEDEF = QIcon (":/icons/TYPEDEF.png");
}

Entity* Entity::child (int i)
{
    return (Entity*)QTreeWidgetItem::child(i);
}

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



void Entity::deleteFileInfo ( QString file, QDateTime olderThan )
{
    Entity* chld;
    for (int i = 0; i<childCount(); i++)
    {
        chld = child (i);
        chld->deleteFileInfo ( file, olderThan );
        if ( chld->file == file and 
            (  chld->updateTime < olderThan or olderThan.isNull() )) //need to delete this entity
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

void Entity::updateSelf (sTagEntryInfo* entry, QString fileName, QDateTime time)
{
    name = entry->name;  //  do possible make this assigmengs automaticaly ???
    file = fileName;
    type = getEntityType (entry->kind );
    line = entry->lineNumber;
    updateTime = time;
    if ( entry->extensionFields.signature != NULL )
    {    
        signature = entry->extensionFields.signature;
    }
    updateGUI ();
}

void Entity::updateGUI ()
{
    QString tip;
    QString newText = name;
    switch (type )
    {
        case NAMESPACE :
            setIcon (0,iNAMESPACE);
            tip = "Namespace";
            newText = QString("namespace ")+name;
            break;
        case CLASS:
            setIcon (0,iCLASS);
            tip = "Class";
            newText = QString("class ")+name;
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
            newText = QString("enum ")+name;
            break;
        case ENUMERATOR:
            tip = "Enumerator";
            break;
        case EXTERN:
            tip = "Extern";
            break;
        case FUNCTION:
            tip = "Function";
            setIcon (0,iFUNCTION);
            break;
        case MACRO:
            tip = "Macro";
            newText = QString("macro ")+name;
            break;
        case MEMBER:
            tip = "Member";
            setIcon (0,iMEMBER);
            break;
        case PROTOTYPE:
            tip = "Prototype";
            break;
        case STRUCT:
            tip = "Struct";
            setIcon (0,iSTRUCT);
            newText = QString("struct ")+name;
            break;
        case TYPEDEF:
            tip = "Typedef";
            setIcon (0,iTYPEDEF);
            newText = QString("typedef ")+name;
            break;
        case UNION:
            tip = "Union";
            newText = QString("union ")+name;
            break;
        case VARIABLE:
            tip = "Variable";
            break;
        case UNKNOWN:
            break;
        default:
            qDebug ("Not right entity type in the constructor");
    }
    tip =    tip+
            ' '+
            name+
            '\n';
    if ( line != 0)
        tip = tip + QFileInfo (file).fileName() + QString(":%1").arg(line);
    else
        tip += "Location of this element unknown";
    setText ( 0, newText + signature);
    setToolTip (0,tip);
}
