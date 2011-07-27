#ifndef MakefileProjectItem_H
#define MakefileProjectItem_H

#include "XUPProjectItem.h"

class CLIToolPlugin;

class MakefileProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	Q_INVOKABLE MakefileProjectItem();
	
	virtual QString projectType() const;
	virtual void installCommands();
	virtual void addFiles( const QStringList& files, XUPItem* scope = 0 );
	virtual void removeValue( XUPItem* item );
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool edit();
};

#endif // MakefileProjectItem_H
