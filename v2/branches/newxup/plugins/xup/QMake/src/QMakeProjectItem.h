#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "XUPProjectItem.h"

class CLIToolPlugin;

class QMakeProjectItem : public XUPProjectItem
{
	Q_OBJECT
	
public:
	Q_INVOKABLE QMakeProjectItem();
	virtual ~QMakeProjectItem();
	
    virtual void addFiles( const QStringList& files, XUPItem* scope = 0 );
	virtual void removeValue( XUPItem* item );
    
	virtual QString toNativeString() const;
	virtual QString projectType() const;
	
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool save();
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget);
	
	virtual void installCommands();
    
    virtual bool edit();
	virtual bool editProjectFiles();
	
protected:
	QHash <QString, QString> mVariableCache;
    
    // return the variable content
    QString getVariableContent( const QString& variableName );
	// analyze the project for handling subdirs and includes
	bool analyze( XUPItem* item );
	// interpret the content, ie, replace variables by their content
	QString interpretContent( const QString& content );
	// handle the inclusion of include files
	bool handleIncludeFile( XUPItem* function );
	// handle inclusion of subdirs projects
	bool handleSubdirs( XUPItem* subdirs );
	//
	void rebuildCache();
	CLIToolPlugin* builder() const;
};

#endif // QMAKEPROJECTITEM_H
