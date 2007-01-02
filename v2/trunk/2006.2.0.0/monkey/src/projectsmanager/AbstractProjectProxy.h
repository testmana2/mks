#ifndef ABSTRACTPROJECTPROXY_H
#define ABSTRACTPROJECTPROXY_H
//
#include <QSortFilterProxyModel>
#include <QHash>
//
#include "AbstractProjectItemModel.h"
//
class AbstractProjectProxy;
typedef QHash<int, AbstractProjectProxy*> QHashProxys; // id, proxy
//
class Q_MONKEY_EXPORT AbstractProjectProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	AbstractProjectProxy( AbstractProjectItemModel* );
	virtual ~AbstractProjectProxy();
	//
	int id() const;
	static AbstractProjectProxy* byId( int );
	static QHashProxys all();
	static AbstractProjectProxy* getProxyByProject( AbstractProjectItemModel* );
	virtual	bool isComplexModel() const;
	//
	virtual AbstractProjectItemModel* project() const = 0;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const = 0;
	//
	static int mUniqueId;
	int mId;
	static QHashProxys mProxysList;
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
