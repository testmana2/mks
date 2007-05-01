#ifndef QMAKEPROJECTPROXY_H
#define QMAKEPROJECTPROXY_H
//
#include <QSortFilterProxyModel>
//
class QMakeProjectModel;
//
class QMakeProjectProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	QMakeProjectProxy( QMakeProjectModel*, QObject* = 0 );
	bool isFiltering() const;
	bool isNegateFilter() const;
	QList<int> filterRoles() const;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
	bool mFiltering;
	bool mNegateFilter;
	QList<int> mFilterRoles;
	//
public slots:
	void setFiltering( bool );
	void setNegateFilter( bool );
	void addFilterRole( int );
	void setFilterRoles( const QList<int>& );
	//
signals:
	void filteringChanged( bool );
	void negateFilterChanged( bool );
	//
};
//
#endif // QMAKEPROJECTPROXY_H
