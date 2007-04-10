#ifndef QMAKEPROJECTPROXYMODEL_H
#define QMAKEPROJECTPROXYMODEL_H
//
#include <QSortFilterProxyModel>
//
class QMakeProjectModel;
//
class QMakeProjectProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	QMakeProjectProxyModel( QMakeProjectModel* );
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
};
//
#endif // QMAKEPROJECTPROXYMODEL_H
