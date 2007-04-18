#include "QMakeProjectProxyModel.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
//
QMakeProjectProxyModel::QMakeProjectProxyModel( QMakeProjectModel* m )
	: QSortFilterProxyModel( m )
{
	setSourceModel( m );
}
//
bool QMakeProjectProxyModel::filterAcceptsRow( int r, const QModelIndex& i ) const
{
return true;
	QModelIndex index;
	index = sourceModel()->index( r, 0, i );
	return index.data( QMakeProjectItem::TypeRole ).toInt() != QMakeProjectItem::ValueType;
}
