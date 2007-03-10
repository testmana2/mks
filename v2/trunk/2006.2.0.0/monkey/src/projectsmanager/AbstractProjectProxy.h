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
	virtual bool isComplexModel() const;
	virtual bool isSettingsView() const;
	//
	virtual AbstractProjectItemModel* project() const = 0;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const = 0;
	//
	static int mUniqueId;
	static QHashProxys mProxysList;
	int mId;
	bool mComplexModel;
	bool mSettingsView;
	//
public slots:
	virtual void setComplexModel( bool );
	virtual void setSettingsView( bool );
	virtual void doubleClicked( const QModelIndex& ) = 0;
	virtual void customContextMenuRequested( const QPoint& ) = 0;
	virtual void projectSettings() = 0;
	//
signals:
	void complexModelChanged( bool );
	void settingsViewChanged( bool );
	// emit when a file request to be open
	void fileOpenRequested( const QString&, AbstractProjectProxy* );
	//
};
//
#endif // ABSTRACTPROJECTPROXY_H
