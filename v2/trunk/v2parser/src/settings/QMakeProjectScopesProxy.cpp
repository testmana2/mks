#include "QMakeProjectScopesProxy.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
//
QMakeProjectScopesProxy::QMakeProjectScopesProxy( QMakeProjectModel* m )
	: QSortFilterProxyModel( m )
{
	setSourceModel( m );
}
//
bool QMakeProjectScopesProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
return true;
	QModelIndex index;
	index = sourceModel()->index( r, 0, i );
	return index.data( QMakeProjectItem::TypeRole ).toInt() != QMakeProjectItem::ValueType;
}
