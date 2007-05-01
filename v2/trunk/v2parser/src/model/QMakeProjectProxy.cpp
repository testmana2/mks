#include "QMakeProjectProxy.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
//
QMakeProjectProxy::QMakeProjectProxy( QMakeProjectModel* m, QObject* p )
	: QSortFilterProxyModel( p ), mFiltering( false ), mNegateFilter( true )
{
	setDynamicSortFilter( true );
	setSourceModel( m );
}
//
bool QMakeProjectProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !mFiltering )
		return true;
	//
	QModelIndex index;
	index = sourceModel()->index( r, 0, i );
	bool b = mNegateFilter ? !mFilterRoles.contains( index.data( QMakeProjectItem::TypeRole ).toInt() ) : mFilterRoles.contains( index.data( QMakeProjectItem::TypeRole ).toInt() );
	if ( !b )
		for ( int j = 0; j < sourceModel()->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;
	return b;
}
//
bool QMakeProjectProxy::isFiltering() const
{
	return mFiltering;
}
//
void QMakeProjectProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	emit filteringChanged( mFiltering );
	filterChanged();
}
//
bool QMakeProjectProxy::isNegateFilter() const
{
	return mNegateFilter;
}
//
void QMakeProjectProxy::setNegateFilter( bool b )
{
	if ( b == mNegateFilter )
		return;
	mNegateFilter = b;
	emit negateFilterChanged( mNegateFilter );
	if ( mFiltering )
		filterChanged();
}
//
void QMakeProjectProxy::addFilterRole( int r )
{
	if ( mFilterRoles.contains( r ) )
		return;
	mFilterRoles << r;
	if ( mFiltering )
		filterChanged();
}
//
QList<int> QMakeProjectProxy::filterRoles() const
{
	return mFilterRoles;
}
//
void QMakeProjectProxy::setFilterRoles( const QList<int>& r )
{
	if ( mFilterRoles == r )
		return;
	mFilterRoles = r;
	if ( mFiltering )
		filterChanged();
}
