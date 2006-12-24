#include "AbstractProjectProxy.h"
//
AbstractProjectProxy::AbstractProjectProxy( AbstractProjectItemModel* s )
	: QSortFilterProxyModel( s ), mComplexModel( false )
{
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
