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
	QMakeProjectProxy( QMakeProjectModel*, bool = true, const QModelIndex& = QModelIndex() );
	//
protected:
	bool filterAcceptsRow( int, const QModelIndex& ) const;
	bool mProjectViewMode;
	QModelIndex mIndex;
	QModelIndex mPIndex;
	//
public slots:
	virtual void setFiltering( bool );
	virtual void doubleClicked( const QModelIndex& );
 	virtual void clicked( const QModelIndex& );
	virtual void customContextMenuRequested( const QPoint& );
	virtual void projectSettings( const QModelIndex& = QModelIndex() );
	//
 signals:
    void fileSelected ( QString absPath );
};
//
#endif // QMAKEPROJECTPROXY_H
