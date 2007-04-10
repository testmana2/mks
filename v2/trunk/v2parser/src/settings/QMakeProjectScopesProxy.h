#ifndef QMAKEPROJECTSCOPESPROXY_H
#define QMAKEPROJECTSCOPESPROXY_H
//
#include <QSortFilterProxyModel>
//
class QMakeProjectModel;
//
class QMakeProjectScopesProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	QMakeProjectScopesProxy( QMakeProjectModel* );
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
};
//
#endif // QMAKEPROJECTSCOPESPROXY_H
