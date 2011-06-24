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
	
	virtual QStringList autoActivatePlugins() const;
	
	virtual void installCommands();
	
	virtual QStringList sourceFiles() const;
	void addFiles( const QStringList& files, XUPItem* scope = NULL );
	virtual void removeItem( XUPItem* item );
	
	virtual bool edit();
	
protected:
	CLIToolPlugin* interpreter() const;
	virtual Pair_String_StringList_List sourceFileNamePatterns() const;
	virtual QStringList filteredVariables() const;
	virtual QString iconsPath() const;
	virtual QString variableDisplayText( const QString& variableName ) const;
	virtual QString variableDisplayIcon( const QString& variableName ) const;
};

#endif // PYQTPROJECTITEM_H
