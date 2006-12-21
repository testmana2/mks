#ifndef ABSTRACTPROJECTPROXY_H
#define ABSTRACTPROJECTPROXY_H
//
#include <QSortFilterProxyModel>
//
class QMakeProjectItemModel;
//
class AbstractProjectProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	AbstractProjectProxy( QMakeProjectItemModel* );
	~AbstractProjectProxy();
	//
	QMakeProjectItemModel* project() const;
	//
	bool isComplexModel() const;
	//
private:
	QMakeProjectItemModel* mSource;
	bool mComplexModel;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
public slots:
	void setComplexModel( bool );
	//
signals:
	void complexModelChanged( bool );
	//
};
//
#endif // ABSTRACTPROJECTPROXY_H
