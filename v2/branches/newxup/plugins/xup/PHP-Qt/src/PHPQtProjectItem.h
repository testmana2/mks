#ifndef PHPQTPROJECTITEM_H
#define PHPQTPROJECTITEM_H

#include "XUPProjectItem.h"

#define PROJECT_TYPE_STRING "PHPQt"

class CLIToolPlugin;

class PHPQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	virtual QString projectType() const;
	virtual XUPProjectItem* newProject() const;

	virtual void installCommands();
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual QStringList sourceFiles() const;
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	virtual void removeItem( XUPItem* item );
	
	virtual bool edit();
	
	
protected:
	CLIToolPlugin* interpreter() const;
	virtual StringStringListList sourceFileNamePatterns() const;
	virtual QStringList filteredVariables() const;
	virtual QString iconsPath() const;
	virtual QString variableDisplayText( const QString& variableName ) const;
	virtual QString variableDisplayIcon( const QString& variableName ) const;
};

#endif // PHPQTPROJECTITEM_H
