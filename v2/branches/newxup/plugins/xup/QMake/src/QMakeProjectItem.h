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
	
	virtual QString toNativeString() const;
	virtual QString projectType() const;
	virtual QString getVariableContent( const QString& variableName );
	
	virtual bool open( const QString& fileName, const QString& codec );
	virtual bool save();
	virtual QString targetFilePath( bool allowToAskUser = false, XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget);
	
	virtual void addFiles( const QStringList& files, XUPItem* scope = 0 );
	virtual void removeItem( XUPItem* item );
	
	virtual void installCommands();
	
	// return the project settings scope, creating it if needed
	XUPItem* projectSettingsScope( bool create ) const;
	// return a project settings value as stringlist or string.
	virtual QStringList projectSettingsValues( const QString& variable, const QStringList& defaultValues = QStringList() ) const;
	virtual QString projectSettingsValue( const QString& variable, const QString& defaultValue = QString() ) const;
	// set a project setting value
	virtual void setProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void setProjectSettingsValue( const QString& variable, const QString& value );
	// add project setting value
	virtual void addProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void addProjectSettingsValue( const QString& variable, const QString& value );
	
	virtual bool edit();
	
protected:
	QHash <QString, QString> mVariableCache;
	
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
