#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"

class CLIToolPlugin;

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	
public:
	Q_INVOKABLE QMakeProjectItem();
	virtual ~QMakeProjectItem();
	
	virtual QString toNativeString() const;
	virtual QString projectType() const;
	virtual QString getVariableContent( const QString& variableName );
	bool analyze( XUPItem* item );
	void rebuildCache();
	// interpret the content, ie, replace variables by their content
	QString interpretContent( const QString& content );
	// handle the inclusion of include files
	bool handleIncludeFile( XUPItem* function );
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool save();
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget);
	
	virtual QStringList sourceFiles() const;
	virtual void addFiles( const QStringList& files, XUPItem* scope = NULL );
	virtual void removeItem( XUPItem* item );
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	// return the project settings scope, creating it if needed
	XUPItem* projectSettingsScope( bool create ) const;
	// return a project settings value as stringlist or string.
	virtual QStringList projectSettingsValues( const QString& variable, const QStringList& defaultValues = QStringList() ) const;
	virtual QString projectSettingsValue( const QString& variable, const QString& defaultValue = QString() ) const;
	// set a project setting value
	virtual void setProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void setProjectSettingsValue( const QString& variable, const QString& value );
	// add project setting value
	virtual void addProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void addProjectSettingsValue( const QString& variable, const QString& value );
	
	// split a multi line value into QStringList
	static QStringList splitMultiLineValue( const QString& value );
	
	virtual bool edit();
protected:
	QHash <QString, QString> mVariableCache;
	void initHashes();
	bool handleSubdirs( XUPItem* subdirs );
	CLIToolPlugin* builder() const; // init mVariableLabels, mVariableIcons
	virtual Pair_String_StringList_List sourceFileNamePatterns() const;
	virtual QString iconsPath() const;
	virtual QString variableDisplayText( const QString& variableName ) const;
	virtual QString variableDisplayIcon( const QString& variableName ) const;
	
	static QHash<QString, QString> mVariableLabels;
	static QHash<QString, QString> mVariableIcons;
	
	static QStringList mFileVariables;
	static QStringList mPathVariables;
	static QStringList mFilteredVariables;
public:
	static QStringList fileVariables();
	static QStringList pathVariables();
	virtual QStringList filteredVariables() const;
};

#endif // QMAKEPROJECTITEM_H
