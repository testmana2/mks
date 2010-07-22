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
	virtual bool analyze( XUPItem* item );
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool save();
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget);
	
	virtual QStringList sourceFiles() const;
	virtual void addFiles( const QStringList& files, XUPItem* scope = NULL );
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	// split a multi line value into QStringList
	static QStringList splitMultiLineValue( const QString& value );
	
	virtual XUPPlugin* editorPlugin();
protected:
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
