#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"
#include "XUPProjectItemInfos.h"

class XUPProjectItem : public QObject, public XUPItem
{
	Q_OBJECT
	
public:
	// project type id
	enum ProjectType { InvalidProject = -1, XUPProject = 0 };
	
	// ctor
	XUPProjectItem();
	// dtor
	virtual ~XUPProjectItem();
	
	// return the global static proejcts types informations
	static XUPProjectItemInfos* projectInfos();
	
	// set last encounter error
	void setLastError( const QString& error );
	// return the last encounter error
	QString lastError() const;
	
	// return the project absolute filename
	QString fileName() const;
	// return the project absolute path
	QString path() const;
	// return an absolute file name according to project path
	QString filePath( const QString& fileName ) const;
	
	// return the most toplevel project ( ie: the model root project )
	XUPProjectItem* topLevelProject() const;
	// return the parent project for include project ( recursive parent project for include project, else return project itself )
	XUPProjectItem* rootIncludeProject() const;
	
	// return true if item is a variable wich values are files
	bool isFileBased( XUPItem* item ) const;
	// return true if item is a variable wich values are paths
	bool isPathBased( XUPItem* item ) const;
	
	// return icon filename for item
	QString iconFileName( XUPItem* item ) const;
	// return the project icons path
	QString iconsPath() const;
	
	// return the display text of a project variable name
	QString variableDisplayText( const QString& variableName ) const;
	// return the display text of a project value
	QString valueDisplayText( XUPItem* valueItem ) const;
	
	// return the display text for the project item
	QString itemDisplayText( XUPItem* item );
	// return the display icon for the project item
	QIcon itemDisplayIcon( XUPItem* item );
	
	// return all variable items named variableName until caller is found ( if define ) or until the the complete tree is scanned
	virtual QList<XUPItem*> getVariables( const XUPItem* root, const QString& variableName, const XUPItem* callerItem = 0 ) const;
	// return the result of interpreting the variable variableName until caller item if defined, or until complete tree is scaned or default value if variable is empty
	virtual QString interpretVariable( const QString& variableName, const XUPItem* callerItem = 0, const QString& defaultValue = QString::null ) const;
	virtual QString interpretValue( XUPItem* callerItem, const QString& attribute ) const;
	
	// return the project type id
	inline virtual int projectType() const { return XUPProjectItem::XUPProject; }
	// register the project type
	virtual void registerProjectType() const;
	// return a new instance of this kind of projecttype
	// FIXME AK in future I think XUPProject will be abstract class
	inline virtual XUPProjectItem* newProject() const { return new XUPProjectItem(); }
	// if the given item is a include function, try handling it if needed
	virtual void handleIncludeItem( XUPItem* function ) const;
	// reimplement this member to allow custom row count, by example to open subproject
	virtual void customRowCount( XUPItem* item ) const;
	// open a project with encoding
	virtual bool open( const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	// close the proejct
	virtual void close();

protected:
	QDomDocument mDocument;
	static XUPProjectItemInfos* mXUPProjectInfos;
	static bool mFoundCallerItem;
};

#endif // XUPPROJECTITEM_H
