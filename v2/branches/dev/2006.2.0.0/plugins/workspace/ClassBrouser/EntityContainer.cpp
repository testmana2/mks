#include <QString>
#include <QDateTime>
#include <QMessageBox>

#include "EntityContainer.h"
#include "Ctags.h"
#include "Entity.h"

EntityContainer::EntityContainer ( QWidget* parent, QString tname, int tdisplayMask): QTreeWidget (parent)
{
    displayMask = tdisplayMask; // -1 == 0xff...ff
	setHeaderLabel ( tname );
	connect (this, SIGNAL (doubleClicked ( const QModelIndex)),this, SLOT (make_goto()));
	Entity::initIcons ();
}

EntityContainer::~EntityContainer ()
{
	disconnect (this, SIGNAL (doubleClicked ( const QModelIndex)),this, SLOT (make_goto()));
}


Entity* EntityContainer::childEntity (int i)
{
	return (Entity*)topLevelItem(i);
}

void EntityContainer::deleteFileInfo ( QString file, QDateTime olderThan )
{
	Entity* chld;
	for ( int i = 0; i < topLevelItemCount (); i++)
	{
		chld = childEntity(i);
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

void EntityContainer::addTagsFromRecord ( FileRecord*  fileRecord )
{

	TagEntryListItem* item = fileRecord->firstTagEntry;
	QString fileName = fileRecord->file ;
	Entity* newEnt;
	Entity* parEnt;
	TagEntry* entry;
    EntityType entType;
	while ( item != NULL )
	{
		entry = &item->entry;	
		item = item->next ;
        entType = Entity::getEntityType (entry->kind);
        if ( not (entType & displayMask) )
            continue; // if mask not set for it's entity - ignore it
		parEnt = getScopeEntity ( entry->scope[0], entry->scope[1]);
		qDebug ("Adding child");
		addChild ( parEnt, entry,fileRecord->file,fileRecord->time );
	}
};

void EntityContainer::updateFileInfo ( QString fileName )
{
	Ctags* tags = Ctags::self();
	FileRecord* rd = tags->GetTagsForFile (fileName); 
	addTagsFromRecord(rd);//add of update existing entityes
	deleteFileInfo ( rd->file, rd->time);//deltete not updated
}

Entity*EntityContainer::getScopeEntity ( QString scope0, QString scope1)
{
	//qDebug ( qPrintable ("trying to find scope| " + scope0 +"|"+ scope1 +"| in " ));
	if	( scope0 == NULL and scope1 == NULL )
		return NULL;

	EntityType ttype = Entity::getScopeType ( scope0 );
	Entity* scopeEntity = NULL;

	QStringList scopes = scope1.split ("::"); // Ctags::FileEntry ->  [Ctags, FileEntry]
	//step 1.   To find first level scope in this
	for ( int i = 0; i < topLevelItemCount(); i++)
	{
		if ( /* (childEntity (i)->type == ttype   or scopes.size() >1)  //we do not know type of not top level items
				and*/ childEntity (i)->name == scopes[0] )// FIXME uncomment scope type checking if will have problems, and will have another problems;)
		{
			scopeEntity = childEntity (i);
			break;//finded
		}
	}
	if ( scopeEntity == NULL) //if not finded - need to create
	{
		scopeEntity = new Entity ( UNKNOWN, scopes[0]);
		addTopLevelItem ( scopeEntity);
	}
	scopes.removeAt (0);
	
	//step 2.  To find not first and not last scopes with unknown type
	Entity* findResult = NULL;
	while ( scopes.size() > 0)
	{
		//qDebug ( qPrintable ("while for parent "+name ));
		findResult = NULL;
		for ( int i = 0; i< scopeEntity->childCount(); i++) //find in the childrens
			if ( scopeEntity->child(i)->name == scopes[0] )  //check self
			{
				//qDebug ( qPrintable ("finded"+tname+" . It self" + name ));	
				findResult = scopeEntity->child(i);
				break;
			}
		if ( findResult == NULL )
		{
			qDebug ( qPrintable ("Not finded parent scope for "+scope0+'|'+scope1 ));
			findResult = new Entity ( ttype, scopes[0]);
			scopeEntity->addChild ( findResult);
		}
		//qDebug ( qPrintable ("Finded parent scope for "+scopes[0]+" in "+name+ " it's "+findResult->name ));
		scopeEntity = findResult;
		scopes.removeAt (0);//this scope finded, will find next level scope
	}
	return scopeEntity;
}

Entity* EntityContainer::findEntityInContainer ( EntityType type, QString name )
{
	for ( int i = 0; i < topLevelItemCount (); i++)
	{
		if (  (childEntity (i)->type == type   or type == UNKNOWN ) 
				and childEntity (i)->name == name )
		{
			return childEntity (i);
		}
	}
	return NULL; //not finded
}

Entity* EntityContainer::findEntityInEntity (Entity* where, EntityType type, QString name )
{
	for ( int i = 0; i < where->childCount(); i++)
	{
		if (  (where->child(i)->type == type   or type == UNKNOWN ) 
				and where->child(i)->name == name )
		{
			return where->child(i);
		}
	}
	return NULL; //not finded
}

void EntityContainer::addChild ( Entity* parEnt,TagEntry* entry, QString fileName, QDateTime time )
{
	if (parEnt)
		addChildInEntity ( parEnt, entry, fileName, time );
	else
		addChildInContainer ( entry, fileName, time);
}

void EntityContainer::addChildInContainer ( TagEntry* entry, QString fileName, QDateTime time )
{
	Entity* existing = findEntityInContainer ( Entity::getEntityType ( entry->kind), entry->name);
	if ( not existing )
		addTopLevelItem ( new Entity ( entry, fileName, time));
	else
		existing->updateSelf (entry, fileName, time);
}

void EntityContainer::addChildInEntity ( Entity* parEnt, TagEntry* entry, QString fileName, QDateTime time )
{
	Entity* existing = findEntityInEntity( parEnt,Entity::getEntityType ( entry->kind), entry->name);
	if ( not existing )
		parEnt->addChild( new Entity ( entry, fileName, time));
	else
		existing->updateSelf ( entry, fileName, time);
}

void EntityContainer::make_goto ()
{
	Entity* activeEntity = (Entity*)currentItem();
	if (activeEntity == NULL)
		qWarning ( "Not finded active entity");
	QMessageBox(QMessageBox::NoIcon,QString ("Hello, world"),"Make goto to the "+ activeEntity->file+QString(":%1").arg(activeEntity->line), QMessageBox::Ok).exec();
}

void EntityContainer::setDisplayMask (int mask)
{
	displayMask = mask;
}
int EntityContainer::getDisplayMask ()
{
	return displayMask;
}