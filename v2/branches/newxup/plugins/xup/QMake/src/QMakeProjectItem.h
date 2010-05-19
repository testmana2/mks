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
	
	virtual BuilderPlugin* builder( const QString& plugin = QString() ) const;
	virtual DebuggerPlugin* debugger( const QString& plugin = QString() ) const;
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;
	virtual void installCommands();

protected:
	bool handleSubdirs( XUPItem* subdirs );
	// return the textual representation key for platform type
	QString platformTypeString( XUPProjectItem::PlatformType type ) const;

};

#endif // QMAKEPROJECTITEM_H
