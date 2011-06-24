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
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	virtual void removeItem( XUPItem* item );
	
	virtual bool open( const QString& fileName, const QString& codec );
	
	virtual bool edit();
	
	virtual Pair_String_StringList_List sourceFileNamePatterns() const;
protected:
	virtual QString iconsPath() const;
};

#endif // MakefileProjectItem_H
