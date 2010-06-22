#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#include <QAction>

#include "XUPProjectItem.h"

class PyQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	enum ProjectType { PyQtProject = 3 };

	virtual int projectType() const;
	virtual void registerProjectType() const;
	virtual XUPProjectItem* newProject() const;
	
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;
	virtual void installCommands();
	virtual void uninstallCommands();
	
	virtual QStringList sourceFiles() const;
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	
	virtual XUPPlugin* editorPlugin();
	
protected slots:
	void on_interpret_clicked();
	
protected:
	QAction * mInterpretAction;
};

#endif // PYQTPROJECTITEM_H
