#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "../../core/XUPProjectItem.h"

class QMakeProjectItem : public XUPProjectItem
{	
public:
	enum ProjectType { QMakeProject = 1 };
	
	QMakeProjectItem();
	virtual ~QMakeProjectItem();
	
	virtual inline QString tr( const char* text ) const { return qApp->translate( "QMakeProjectItem", text ); }
	
	inline virtual int projectType() const { return QMakeProjectItem::QMakeProject; }
	virtual void registerProjectType() const;
	inline virtual XUPProjectItem* newProject() const { return new QMakeProjectItem(); }
	virtual void customRowCount( XUPItem* item ) const;
	virtual bool open( const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	virtual void close();
	
	virtual QString interpretVariable( const QString& variableName, const XUPItem* callerItem = 0, const QString& defaultValue = QString::null ) const;
	virtual QString interpretValue( XUPItem* callerItem, const QString& attribute ) const;
};

#endif // QMAKEPROJECTITEM_H
