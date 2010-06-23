#ifndef PHPQTPROJECTITEM_H
#define PHPQTPROJECTITEM_H

#include "XUPProjectItem.h"

class PHPQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	enum ProjectType { PHPQtProject = 2 };

	virtual int projectType() const;
	virtual void registerProjectType() const;
	virtual XUPProjectItem* newProject() const;

	virtual void installCommands();
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual QStringList sourceFiles() const;
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	
	virtual XUPPlugin* editorPlugin();
	
	
protected:
	InterpreterPlugin* interpreter() const;
};

#endif // PHPQTPROJECTITEM_H
