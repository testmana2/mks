#ifndef ENTITY_H
#define ENTITY_H
#include <QString>
#include <QList>
#include <QTreeWidget>
#include <QObject>
#include <QDateTime>
#include <QIcon>

#include "Ctags.h"

enum EntityType { 
	NAMESPACE, //0
	CLASS, //1
	CONSTRUCTOR, //2
	DESTRUCTOR, //3
	ENUM, //4
	ENUMERATOR, //5
	EXTERN, //6
	FUNCTION, //7
	MACRO, //8
	MEMBER, //9
	PROTOTYPE, //10
	STRUCT, //11
	TYPEDEF, //12
	UNION, //13
	VARIABLE, //14

	ROOT_ENTITY,  //15  //top level entiry, possible file, or project

	UNKNOWN //16
};

class Entity : public QTreeWidgetItem
{
private:
	static QIcon iNAMESPACE;
	static QIcon iCLASS;
	static QIcon iENUM;
	static QIcon iFUNCTION;
	static QIcon iMEMBER;
	static QIcon iSTRUCT;
	static QIcon iTYPEDEF;
	
public:
	Entity ( TagEntry* entry, QString fileName, QDateTime time);
	Entity (EntityType ttype, QString tname);
	QDateTime updateTime ;
	QString name;
	EntityType type;
	QString file;
	int line;
	QString signature;

	Entity* child (int i);
	
	//function will create or update info about file in the entity

	//convert string kindName to the EntityType
	static EntityType getScopeType ( QString kindName );
	//convert TagEntry.kind  to the EntiryType
	static EntityType getEntityType ( char kind );
	static void initIcons ();
	void deleteFileInfo ( QString file, QDateTime olderThan );
	void updateSelf (TagEntry* entry, QString fileName, QDateTime time);
	void updateGUI ();
};

#endif //ENTITY_H
