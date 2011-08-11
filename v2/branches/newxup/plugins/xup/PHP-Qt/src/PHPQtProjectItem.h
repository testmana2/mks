#ifndef PHPQTPROJECTITEM_H
#define PHPQTPROJECTITEM_H

#include "XUPProjectItem.h"

class CLIToolPlugin;

class PHPQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	Q_INVOKABLE PHPQtProjectItem();
	
	virtual QString projectType() const;
	virtual void installCommands();
	virtual bool edit();
	virtual bool editProjectFiles();
	
protected:
	CLIToolPlugin* interpreter() const;
};

#endif // PHPQTPROJECTITEM_H
