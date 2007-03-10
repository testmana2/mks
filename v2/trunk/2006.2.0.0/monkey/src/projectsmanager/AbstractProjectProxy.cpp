#include "AbstractProjectProxy.h"
//
#include <QHash>
//
int AbstractProjectProxy::mUniqueId = 0;
QHashProxys AbstractProjectProxy::mProxysList = QHashProxys();
//
AbstractProjectProxy::AbstractProjectProxy( AbstractProjectItemModel* s )
	: QSortFilterProxyModel( s ), mId(mUniqueId++ ), mComplexModel( false ),
	mSettingsView( false )
{
	mProxysList[ mId ] = this;
}
//
AbstractProjectProxy::~AbstractProjectProxy()
{
	if ( mProxysList.contains( mId ) )
		mProxysList.remove( mId );
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
bool AbstractProjectProxy::isSettingsView() const
{
	return mSettingsView;
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
//
void AbstractProjectProxy::setSettingsView( bool b )
{
	if ( mSettingsView == b )
		return;
	mSettingsView = b;
	filterChanged();
	emit settingsViewChanged( mSettingsView );
}
