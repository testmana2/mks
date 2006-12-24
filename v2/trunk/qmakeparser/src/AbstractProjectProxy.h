#ifndef ABSTRACTPROJECTPROXY_H
#define ABSTRACTPROJECTPROXY_H
//
#include <QSortFilterProxyModel>
//
#include "AbstractProjectItemModel.h"
//
class AbstractProjectProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	AbstractProjectProxy( AbstractProjectItemModel* );
	//
	virtual	bool isComplexModel() const;
	//
	virtual AbstractProjectItemModel* project() const = 0;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const = 0;
	//
	bool mComplexModel;
	//
public slots:
	virtual void setComplexModel( bool );
	//
signals:
	void complexModelChanged( bool );
	//
};
//
#endif // ABSTRACTPROJECTPROXY_H
