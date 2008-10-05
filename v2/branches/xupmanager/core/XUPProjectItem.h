#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"
#include "XUPProjectItemInfos.h"

class XUPProjectItem : public XUPItem
{
public:
	enum ProjectType { XUPProject = 0 };
	XUPProjectItem();
	virtual ~XUPProjectItem();
	
	void setLastError( const QString& error );
	QString lastError() const;
	
	QString fileName() const;
	QString filePath( const QString& fileName ) const;
	static XUPProjectItemInfos* projectInfos();
	// return true if item is a variable wich values are files
	bool isFileBased( XUPItem* item ) const;
	// return true if item is a variable wich values are paths
	bool isPathBased( XUPItem* item ) const;
	// return icon filename for item
	QString iconFileName( XUPItem* item ) const;
	// return the icons path
	QString iconsPath() const;
	// return the display text of a variable name
	QString variableDisplayText( const QString& variableName ) const;
	
	inline virtual int projectType() const { return XUPProjectItem::XUPProject; }
	virtual void registerProjectType() const;
	inline virtual XUPProjectItem* newItem() const { return new XUPProjectItem(); }
	virtual bool open( const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	virtual void close();
	virtual void customRowCount( XUPItem* item );

protected:
	QDomDocument mDocument;
	static XUPProjectItemInfos* mXUPProjectInfos;
};

#endif // XUPPROJECTITEM_H
