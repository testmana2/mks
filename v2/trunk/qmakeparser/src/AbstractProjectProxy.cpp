#include "AbstractProjectProxy.h"
#include "AbstractProjectItemModel.h"
#include "QMakeProjectItemModel.h"
#include "QMakeProjectItem.h"
//
#include <QStringList>
//
AbstractProjectProxy::AbstractProjectProxy( QMakeProjectItemModel* s )
	: QSortFilterProxyModel( s ), mSource( s ), mComplexModel( false )
{
	setSourceModel( mSource );
}
//
AbstractProjectProxy::~AbstractProjectProxy()
{
}
//
QMakeProjectItemModel* AbstractProjectProxy::project() const
{
	return mSource;
}
//
bool AbstractProjectProxy::isComplexModel() const
{
	return mComplexModel;
}
//
bool AbstractProjectProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( mComplexModel || !i.isValid() )
		return true;
	static QStringList SimpleModelVariables = QStringList() << "FORMS"
		<< "HEADERS" << "SOURCES" << "TRANSLATIONS" << "RESOURCES" << "OPENEDFILES";
	// got the needed index
	QModelIndex index = i;
	if ( mSource->hasChildren( i ) )
		index = i.child( r, i.column() );
	// apply filtering
	switch ( index.data( QMakeProjectItem::TypeRole ).toInt() )
	{
	case QMakeProjectItem::Project:
	case QMakeProjectItem::Value:
		return true;
		break;
	case QMakeProjectItem::Variable:
		return SimpleModelVariables.contains( index.data().toString(), Qt::CaseInsensitive );
		break;
	case QMakeProjectItem::Scope:
	{
		for ( int j = 0; j < mSource->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;
		return false;
		break;
	}
	default:
		return false;
		break;
	}
	return false;
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
