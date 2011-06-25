#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include "MonkeyExport.h"

#include <QPair>
#include <QStringList>
#include <QHash>

typedef QPair<QString, QStringList> Pair_String_StringList;
typedef QList<Pair_String_StringList> Pair_String_StringList_List;

typedef QPair<QString, QString> Pair_String_String;
typedef QList<Pair_String_String> Pair_String_String_List;

class XUPProjectItem;

class Q_MONKEY_EXPORT ProjectTypesIndex: public QObject
{
	Q_OBJECT

public:
	ProjectTypesIndex( QObject* parent = 0 );
	
	// register the proejct type
	void registerType( const QString& projectType, const QMetaObject* projectMetaObject, const Pair_String_StringList_List& suffixes );
	
	// unregister the projecttype
	void unRegisterType( const QString& projectType );
	
	// return the registered suffixes for project type
	Pair_String_StringList_List typeSuffixes( const QString& projectType ) const;
	
	// check if filename matches to some project type
	bool fileIsAProject( const QString& fileName ) const;
	
	// return a valid project item for fileName
	XUPProjectItem* newProjectItem( const QString& fileName ) const;
	
	// return a filter of all project type suffixes: ie. for giving it to open/save file dialog
	QString projectFilters() const;
	
protected:
	QHash<QString, const QMetaObject*> mRegisteredProjectItems; // project type, project item
	QHash<QString, Pair_String_StringList_List> mSuffixes; // project type, suffixe label, suffixes
};

#endif // XUPPROJECTITEMINFOS_H
