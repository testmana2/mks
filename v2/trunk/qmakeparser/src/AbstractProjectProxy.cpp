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
