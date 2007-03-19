#ifndef QMAKEPROJECTSETTINGSPROXY_H
#define QMAKEPROJECTSETTINGSPROXY_H
//
#include <QAbstractProxyModel>
//
class QMakeProjectItemModel;
//
class QMakeProjectProxy : public QAbstractProxyModel
{
	Q_OBJECT
	//
public:
	QMakeProjectSettingsProxy( QMakeProjectItemModel* );
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
};
//
#endif // QMAKEPROJECTSETTINGSPROXY_H
