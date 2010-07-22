#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#define PROJECT_TYPE_STRING "PyQt"

#include "XUPProjectItem.h"

class InterpreterPlugin;

class PyQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	virtual QString projectType() const;
	virtual XUPProjectItem* newProject() const;
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	virtual QStringList sourceFiles() const;
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	
	virtual XUPPlugin* editorPlugin();
	
protected:
	InterpreterPlugin* interpreter() const;
	virtual StringStringListList sourceFileNamePatterns() const;
	virtual QStringList filteredVariables() const;
	virtual QString iconsPath() const;
	virtual QString variableDisplayText( const QString& variableName ) const;
	virtual QString variableDisplayIcon( const QString& variableName ) const;
};

#endif // PYQTPROJECTITEM_H
