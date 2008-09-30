#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"
#include "XUPProjectItemInfos.h"

#include <QApplication>

class XUPProjectItem : public XUPItem
{
public:
	enum ProjectType { XUPProject = 0 };
	XUPProjectItem();
	virtual ~XUPProjectItem();
	
	void setLastError( const QString& error );
	QString lastError() const;
	
	inline QString fileName() const { return mFileName; }
	inline static XUPProjectItemInfos* projectInfos() { return mXUPProjectInfos; }
	// return true if item is a variable wich values are files
	bool isFileBased( XUPItem* item ) const;
	// return true if item is a variable wich values are paths
	bool isPathBased( XUPItem* item ) const;
	// return icon filename for item
	QString iconFileName( XUPItem* item ) const;
	// return the icons path
	QString iconsPath() const;
	// return the display text of a variable name
	QString displayText( const QString& variableName ) const;
	
	inline virtual int projectType() const { return XUPProjectItem::XUPProject; }
	inline virtual QString tr( const char* text ) const { return qApp->translate( "XUPProjectItem", text ); }
	virtual void registerProjectType() const;
	inline virtual XUPProjectItem* newItem() const { return new XUPProjectItem(); }
	virtual bool open( const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	virtual void close();

protected:
	QString mFileName;
	QDomDocument mDocument;
	QString mLastError;
	static XUPProjectItemInfos* mXUPProjectInfos;
};

#endif // XUPPROJECTITEM_H
