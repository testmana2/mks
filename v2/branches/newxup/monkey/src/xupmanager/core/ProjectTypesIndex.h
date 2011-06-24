#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include "MonkeyExport.h"

#include <QStringList>
#include <QPair>
#include <QMap>
#include <QApplication>

typedef QPair<QString, QStringList> Pair_String_StringList;
typedef QList<Pair_String_StringList> Pair_String_StringList_List;

typedef QPair<QString, QString> Pair_String_String;
typedef QList<Pair_String_String> Pair_String_String_List;

class XUPProjectItem;

class Q_MONKEY_EXPORT ProjectTypesIndex: public QObject
{
	Q_OBJECT
	friend class XUPProjectItem;

public:
	ProjectTypesIndex( QObject* parent = 0 );
	
	// register the proejct type
	void registerType( const QString& projectType, const QMetaObject* projectMetaObject );
	
	// unregister the projecttype
	void unRegisterType( const QString& projectType );
	
	// check if filename matches to some project type
	bool fileIsAProject( const QString& fileName ) const;
	
	// return a valid project item for fileName
	XUPProjectItem* newProjectItem( const QString& fileName ) const;
	
	// register project type suffixes
	void registerSuffixes( const QString& projectType, const Pair_String_StringList_List& suffixes );
	
	// return a filter of all project type suffixes: ie. for giving it to open/save file dialog
	QString projectsFilter() const;
	
protected:
	QMap<QString, const QMetaObject*> mRegisteredProjectItems; // project type, project item
	QMap<QString, Pair_String_StringList_List> mSuffixes; // project type, suffixe label, suffixes
};

#endif // XUPPROJECTITEMINFOS_H
