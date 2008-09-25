#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"

class XUPProjectItem : public XUPItem
{
public:
	XUPProjectItem();
	virtual ~XUPProjectItem();
	
	virtual bool open( const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	virtual bool close();
	
	void setLastError( const QString& error );
	QString lastError() const;
	
	inline QString fileName() const { return mFileName; }

protected:
	QString mFileName;
	QDomDocument mDocument;
	QString mLastError;
	QList<XUPProjectItem*> mSubProjects;
};

#endif // XUPPROJECTITEM_H
