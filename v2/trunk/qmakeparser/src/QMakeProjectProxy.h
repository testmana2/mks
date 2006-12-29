#ifndef QMAKEPROJECTPROXY_H
#define QMAKEPROJECTPROXY_H
//
#include "AbstractProjectProxy.h"
//
class QMakeProjectItemModel;
//
class QMakeProjectProxy : public AbstractProjectProxy
{
	Q_OBJECT
	//
public:
	QMakeProjectProxy( QMakeProjectItemModel* );
	//
	virtual AbstractProjectItemModel* project() const;
	//
private:
	QMakeProjectItemModel* mSource;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
};
//
#endif // QMAKEPROJECTPROXY_H
