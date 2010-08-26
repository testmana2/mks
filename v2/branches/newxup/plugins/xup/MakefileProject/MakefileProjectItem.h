#ifndef MakefileProjectItem_H
#define MakefileProjectItem_H

#define PROJECT_TYPE_STRING "MakefileProject"

#include "XUPProjectItem.h"

class CLIToolPlugin;

class MakefileProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	MakefileProjectItem();
	virtual QString projectType() const;
	virtual XUPProjectItem* newProject() const;
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	virtual void removeItem( XUPItem* item );
	
	virtual bool open( const QString& fileName, const QString& codec );
	
	virtual bool edit();
	
	virtual StringStringListList sourceFileNamePatterns() const;
protected:
	virtual QString iconsPath() const;
};

#endif // MakefileProjectItem_H
