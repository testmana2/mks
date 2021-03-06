/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : EntityContainer.h
** Date      : 2008-01-14T00:40:07
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
	\file EntityContainer.h
	\date 2008-01-14T00:40:08
	\author Andrei Kopats
	\brief Header file for EntityContainer class
*/
#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H
#include <QTreeWidget>
#include "Entity.h"

class Navigator;

/*!
	\brief Root root of tree, displaying file items
	
	Class herited from QTreeWidget using for build tree for file and for displaying
	this tree on UI
*/
class EntityContainer  : public QTreeWidget 
{
Q_OBJECT
public:
	EntityContainer ( QWidget* parent, Navigator* navigator);
	~EntityContainer ();
	void updateFileInfo ( QString fileName );	
	void deleteFileInfo ( QString file, QDateTime olderThan );

private:
	Navigator* mNavigator;
	
	void addTagsFromRecord (QString fileName, FileRecord*  fileRecord);

	Entity* childEntity (int);

	//will return pointer to the finded entity, or NULL.
	Entity* findEntity ( EntityType ttype, QString tname);
	//will return poiner to the entity, which are for this scope, or NULL, if child have no scope
	Entity* getScopeEntity ( QString scope0, QString scope1);

	//parEnt may be == NULL or Entity*.  result of findScopeEntity function;
	void addChild ( Entity* parEnt,tagEntryInfo* entry, QString fileName, QDateTime time );	

	void addChildInEntity ( Entity* parEnt, tagEntryInfo* entry, QString fileName, QDateTime time );
	//setting. If true - entity will allow to add to self only compex entityes and his members
	bool complex_only;

	Entity* findEntityInContainer ( EntityType type, const QString& name, const QString& signature);
	Entity* findEntityInEntity (Entity* where, EntityType type, const QString& name, const QString& signature);

	void addChildInContainer ( tagEntryInfo* entry, QString fileName, QDateTime time );

public slots:
	void makeGoto ();
};
#endif //ENTITYCONTAINER_H 
