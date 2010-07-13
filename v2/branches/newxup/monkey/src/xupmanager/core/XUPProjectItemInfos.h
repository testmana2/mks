#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include "MonkeyExport.h"

#include <QStringList>
#include <QPair>
#include <QMap>
#include <QApplication>

typedef QPair<QString, QStringList> PairStringStringList;
typedef QList<PairStringStringList> StringStringListList;

typedef QPair<QString, QString> PairStringString;
typedef QList<PairStringString> StringStringList;

class XUPProjectItem;

class Q_MONKEY_EXPORT XUPProjectItemInfos
{
	friend class XUPProjectItem;

public:
	XUPProjectItemInfos();
	
	inline QString tr( const char* text ) const { return qApp->translate( "XUPProjectItemInfos", text ); }
	
	// return true if proejct type is a registered proejct type else false
	bool isRegisteredType( int projectType ) const;
	// register the proejct type
	void registerType( int projectType, XUPProjectItem* projectItem );
	// unregister the projecttype
	void unRegisterType( int projectType );
	
	// return a valid project item for fileName
	XUPProjectItem* newProjectItem( const QString& fileName ) const;
	
	// register the operators for project type
	void registerOperators( int projectType, const QStringList& operators );
	// return the operators list for proejct type
	QStringList operators( int projectType ) const;
	
	// register the filtered variable list for project type: ie the list a variable shown in filtered view
	void registerFilteredVariables( int projectType, const QStringList& variables );
	// return the filtered variable list for project type
	QStringList filteredVariables( int projectType ) const;
	
	// register the file based variable list for project type: ie variables that contains filepath
	void registerFileVariables( int projectType, const QStringList& variables );
	// return the file based variables list for project type
	QStringList fileVariables( int projectType ) const;
	
	// register the path based variable list for project type: ie variables that contains path
	void registerPathVariables( int projectType, const QStringList& variables );
	// return the path based variables list for project type
	QStringList pathVariables( int projectType ) const;
	
	// register project type suffixes
	void registerSuffixes( int projectType, const StringStringListList& suffixes );
	// return the project type suffixes
	StringStringListList suffixes( int projectType ) const;
	
	// return a filter of all project type suffixes: ie. for giving it to open/save file dialog
	QString projectsFilter() const;
	// return the project type to use for opening the filename project or -1 if no project type can handle the suffixe
	int projectTypeForFileName( const QString& fileName ) const;
	// return true if variable name is variable wich values are files
	bool isFileBased( int projectType, const QString& variableName ) const;
	// return true if variable name is variable wich values are paths
	bool isPathBased( int projectType, const QString& variableName ) const;
	// return a list of all know variable for this kind of project
	QStringList knowVariables( int projectType ) const;
	
protected:
	QMap<int, XUPProjectItem*> mRegisteredProjectItems; // project type, project item
	QMap<int, QStringList> mOperators; // project type, operators
	QMap<int, QStringList> mFilteredVariables; // project type, filtered variable name
	QMap<int, QStringList> mFileVariables; // project type, filename based variable name
	QMap<int, QStringList> mPathVariables; // project type, pathname based variable name
	QMap<int, StringStringListList> mSuffixes; // project type, suffixe label, suffixes
};

#endif // XUPPROJECTITEMINFOS_H
