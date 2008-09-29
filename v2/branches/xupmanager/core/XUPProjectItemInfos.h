#ifndef XUPPROJECTITEMINFOS_H
#define XUPPROJECTITEMINFOS_H

#include <QStringList>
#include <QPair>
#include <QMap>
#include <QApplication>

typedef QPair<QString, QStringList> PairStringStringList;
typedef QList<PairStringStringList> StringStringListList;

typedef QPair<QString, QString> PairStringString;
typedef QList<PairStringString> StringStringList;

class XUPProjectItemInfos
{
	friend class XUPProjectItem;

public:
	XUPProjectItemInfos();
	
	inline QString tr( const char* text ) const { return qApp->translate( "XUPProjectItemInfos", text ); }
	
	bool isRegisteredType( int projectType ) const;
	void registerType( int projectType );
	void unRegisterType( int projectType );
	
	void registerOperators( int projectType, const QStringList& operators );
	QStringList operators( int projectType ) const;
	
	void registerFilteredVariables( int projectType, const QStringList& variables );
	QStringList filteredVariables( int projectType ) const;
	
	void registerFileVariables( int projectType, const QStringList& variables );
	QStringList fileVariables( int projectType ) const;
	
	void registerPathVariables( int projectType, const QStringList& variables );
	QStringList pathVariables( int projectType ) const;
	
	void registerSuffixes( int projectType, const StringStringListList& suffixes );
	StringStringListList suffixes( int projectType ) const;
	
	void registerVariableLabels( int projectType, const StringStringList& labels );
	StringStringList variableLabels( int projectType ) const;
	
	void registerVariableIcons( int projectType, const StringStringList& icons );
	StringStringList variableIcons( int projectType ) const;
	
	void registerVariableSuffixes( int projectType, const StringStringListList& suffixes );
	StringStringListList variableSuffixes( int projectType ) const;
	
	QString projectsFilter();
	int projectTypeForFileName( const QString& fileName ) const;
	// return true if variable name is variable wich values are files
	bool isFileBased( int projectType, const QString& variableName ) const;
	// return true if variable name is variable wich values are paths
	bool isPathBased( int projectType, const QString& variableName ) const;
	
	/*
	QStringList variablesList() const
	{
		QStringList list;
		foreach ( const QString& s, QStringList( FilteredVariables ) << FileVariables << PathVariables << VariableLabels.keys() << VariableIcons.keys() << VariableSuffixes.keys() )
			if ( !list.contains( s ) )
				list << s;
		return list;
	}
	*/
protected:
	QList<int> mRegistered; // registered project type
	QMap<int, QStringList> mOperators; // project type, operators
	QMap<int, QStringList> mFilteredVariables; // project type, filtered variable name
	QMap<int, QStringList> mFileVariables; // project type, filename based variable name
	QMap<int, QStringList> mPathVariables; // project type, pathname based variable name
	QMap<int, StringStringListList> mSuffixes; // project type, suffixe label, suffixes
	QMap<int, StringStringList> mVariableLabels; // project type, variable name, label
	QMap<int, StringStringList> mVariableIcons; // project type, variable name, icon
	QMap<int, StringStringListList> mVariableSuffixes; // project type, file based variable name, suffixes
};

#endif // XUPPROJECTITEMINFOS_H
