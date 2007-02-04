#include "AbstractProjectProxy.h"
//
#include <QHash>
//
int AbstractProjectProxy::mUniqueId = 0;
QHashProxys AbstractProjectProxy::mProxysList = QHashProxys();
//
AbstractProjectProxy::AbstractProjectProxy( AbstractProjectItemModel* s )
	: QSortFilterProxyModel( s ), mComplexModel( false )
{
	mId = mUniqueId;
	mUniqueId++;
	mProxysList[ mId ] = this;
}
//
AbstractProjectProxy::~AbstractProjectProxy()
{
	if ( mProxysList.contains( mId ) )
		mProxysList.remove( mId );
	qWarning( "Proxy deleted" );
}
//
int AbstractProjectProxy::id() const
{
	return mId;
}
//
AbstractProjectProxy* AbstractProjectProxy::byId( int i )
{
	if ( !mProxysList.contains( i ) )
		return 0;
	return mProxysList.value( i );
}
//
QHashProxys AbstractProjectProxy::all()
{
	return mProxysList;
}
//
AbstractProjectProxy* AbstractProjectProxy::getProxyByProject( AbstractProjectItemModel* p )
{
	foreach ( AbstractProjectProxy* py, all() )
		if ( py->project() == p )
			return py;
	return 0;
}
//
bool AbstractProjectProxy::isComplexModel() const
{
	return mComplexModel;
}
//
void AbstractProjectProxy::setComplexModel( bool b )
{
	if ( mComplexModel == b )
		return;
	mComplexModel = b;
	filterChanged();
	emit complexModelChanged( mComplexModel );
}
