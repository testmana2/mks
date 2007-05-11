#ifndef QMAKEPROJECTPROXY_H
#define QMAKEPROJECTPROXY_H
//
#include "AbstractProjectProxy.h"
//
class QMakeProjectModel;
//
class QMakeProjectProxy : public AbstractProjectProxy
{
	Q_OBJECT
	//
public:
	QMakeProjectProxy( QMakeProjectModel*, bool = true );
	//
protected:
	bool filterAcceptsRow( int, const QModelIndex& ) const;
	bool mProjectViewMode;
	//
public slots:
	virtual void setFiltering( bool );
	virtual void doubleClicked( const QModelIndex& );
	virtual void customContextMenuRequested( const QPoint& );
	virtual void projectSettings( const QModelIndex& = QModelIndex() );
	//
};
//
#endif // QMAKEPROJECTPROXY_H
