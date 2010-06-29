#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#include "XUPProjectItem.h"

class InterpreterPlugin;

class PyQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	enum ProjectType { PyQtProject = 3 };

	virtual int projectType() const;
	virtual void registerProjectType() const;
	virtual XUPProjectItem* newProject() const;
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	virtual QStringList sourceFiles() const;
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	
	virtual XUPPlugin* editorPlugin();
	
protected:
	InterpreterPlugin* interpreter() const;
	virtual StringStringListList sourceFileNamePatterns() const;
	virtual QString variableDisplayText( const QString& variableName ) const;
};

#endif // PYQTPROJECTITEM_H
