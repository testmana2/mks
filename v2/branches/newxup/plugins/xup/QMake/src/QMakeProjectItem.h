#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	
public:
	enum ProjectType { QMakeProject = 1 };
	
	QMakeProjectItem();
	virtual ~QMakeProjectItem();
	
	virtual QString toString() const;
	inline virtual int projectType() const { return QMakeProjectItem::QMakeProject; }
	virtual void registerProjectType() const;
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
	bool handleSubdirs( XUPItem* subdirs );
	BuilderPlugin* builder() const;
};

#endif // QMAKEPROJECTITEM_H