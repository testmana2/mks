#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include "MonkeyExport.h"

#include <QPair>
#include <QStringList>
#include <QMap>
#include <QHash>

typedef QPair<QString, QStringList> Pair_String_StringList;
typedef QList<Pair_String_StringList> Pair_String_StringList_List;

typedef QPair<QString, QString> Pair_String_String;
typedef QList<Pair_String_String> Pair_String_String_List;

class XUPProjectItem;

struct DocumentFilter
{
	DocumentFilter( int weight = -1, const QString& label = QString::null, const QString& icon = QString::null, const QStringList& filters = QStringList() )
	{
		this->weight = weight;
		this->label = label;
		this->icon = icon;
		this->filters = filters;
	}
	
	int weight; // the weight is sued for sorting items in model
	QString label;
	QString icon;
	QStringList filters;
};

typedef QMap<QString, DocumentFilter> DocumentFilterMap; // filter name, filter

class Q_MONKEY_EXPORT ProjectTypesIndex: public QObject
{
	Q_OBJECT

public:
	ProjectTypesIndex( QObject* parent = 0 );
	
	// register the proejct type
	void registerType( const QString& projectType, const QMetaObject* projectMetaObject, const DocumentFilterMap& filters );
	
	// unregister the projecttype
	void unRegisterType( const QString& projectType );
	
	// return the registered filters for project type
	DocumentFilterMap typeFilters( const QString& projectType ) const;
	
	// check if filename matches to some project type
	bool fileIsAProject( const QString& fileName ) const;
	
	// return a valid project item for fileName
	XUPProjectItem* newProjectItem( const QString& fileName ) const;
	
	// return a map that can be used to create file dialog filter
	QMap<QString, QStringList> suffixes() const;
	
protected:
	QHash<QString, const QMetaObject*> mRegisteredProjectItems; // project type, project item
	QHash<QString, DocumentFilterMap> mFilters; // project type, filters
};

#endif // XUPPROJECTITEMINFOS_H
