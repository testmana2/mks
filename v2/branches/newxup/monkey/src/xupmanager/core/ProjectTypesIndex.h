#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include "MonkeyExport.h"

#include <QStringList>
#include <QPair>
#include <QMap>
#include <QApplication>

/* TODO PasNox: it's nearly impossible to understand this data types for homo sapiens having standard IQ
  Replace please pairs with 
	struct 
	{
		QString sensibleName;
		QString sensibleName;
	} sensibleTypeName;
	And do not create please so complex data types.
*/
typedef QPair<QString, QStringList> PairStringStringList;
typedef QList<PairStringStringList> StringStringListList;

typedef QPair<QString, QString> PairStringString;
typedef QList<PairStringString> StringStringList;

class XUPProjectItem;

class Q_MONKEY_EXPORT ProjectTypesIndex: public QObject
{
	Q_OBJECT
	friend class XUPProjectItem;

public:
	ProjectTypesIndex();
	
	// register the proejct type
	void registerType( QString projectType, XUPProjectItem* projectItem );
	// unregister the projecttype
	void unRegisterType( QString projectType );
	
	// check if filename matches to some project type
	bool fileIsAProject( const QString& fileName ) const;
	
	// return a valid project item for fileName
	XUPProjectItem* newProjectItem( const QString& fileName ) const;
	
	// register project type suffixes
	void registerSuffixes( QString projectType, const StringStringListList& suffixes );
	
	// return a filter of all project type suffixes: ie. for giving it to open/save file dialog
	QString projectsFilter() const;
	
protected:
	QMap<QString, XUPProjectItem*> mRegisteredProjectItems; // project type, project item
	QMap<QString, StringStringListList> mSuffixes; // project type, suffixe label, suffixes
};

#endif // XUPPROJECTITEMINFOS_H
