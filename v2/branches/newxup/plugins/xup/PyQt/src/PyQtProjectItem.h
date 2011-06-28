#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#include "XUPProjectItem.h"

class CLIToolPlugin;

class PyQtProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	Q_INVOKABLE PyQtProjectItem();
	
	virtual QString projectType() const;
	virtual void installCommands();
	virtual bool edit();
	
protected:
	virtual DocumentFilterMap sourceFileNamePatterns() const;
	
	CLIToolPlugin* interpreter() const;
};

#endif // PYQTPROJECTITEM_H
