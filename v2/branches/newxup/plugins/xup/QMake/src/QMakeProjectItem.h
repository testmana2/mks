#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"

#define PROJECT_TYPE_STRING "QMake"

class BuilderPlugin;

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	
public:
	QMakeProjectItem();
	virtual ~QMakeProjectItem();
	
	virtual QString toString() const;
	virtual QString projectType() const;
	inline virtual XUPProjectItem* newProject() const { return new QMakeProjectItem(); }
	virtual QString getVariableContent( const QString& variableName );
	bool analyze( XUPItem* item );
	virtual void rebuildCache();
	// interpret the content, ie, replace variables by their content
	QString interpretContent( const QString& content );
	// handle the inclusion of include files
	bool handleIncludeFile( XUPItem* function );
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool save();
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget);
	
	virtual QStringList sourceFiles() const;
	virtual void addFiles( const QStringList& files, XUPItem* scope = NULL );
	
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
	QHash <QString, QString> variableCache();
protected:
	QHash <QString, QString> mVariableCache;
	void initHashes();
	bool handleSubdirs( XUPItem* subdirs );
	BuilderPlugin* builder() const; // init mVariableLabels, mVariableIcons
	virtual StringStringListList sourceFileNamePatterns() const;
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
