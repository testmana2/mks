#include "QMakeProjectProxy.h"
#include "QMakeProjectItemModel.h"
#include "QMakeProjectItem.h"
//
QMakeProjectProxy::QMakeProjectProxy( QMakeProjectItemModel* s )
	: AbstractProjectProxy( s ), mSource( s )
{
	setSourceModel( mSource );
}
//
AbstractProjectItemModel* QMakeProjectProxy::project() const
{
	return mSource;
}
//
bool QMakeProjectProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( mComplexModel || !i.isValid() )
		return true;
	// got the needed index
	QModelIndex index = i;
	if ( mSource->hasChildren( i ) )
		index = i.child( r, i.column() );
	// apply filtering
	switch ( index.data( QMakeProjectItem::TypeRole ).toInt() )
	{
	case QMakeProjectItem::Project:
	case QMakeProjectItem::Folder:
	case QMakeProjectItem::Value:
	case QMakeProjectItem::File:
		return true;
		break;
	case QMakeProjectItem::Scope:
	{
		for ( int j = 0; j < mSource->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;
		return false;
		break;
	}
	case QMakeProjectItem::Variable:
		return QMakeProjectItemModel::simpleModelVariables().contains( index.data().toString(), Qt::CaseInsensitive );
		break;
	}
	return false;
}
//
void QMakeProjectProxy::doubleClicked( const QModelIndex& i )
{
	QModelIndex t = mapToSource( i );
	QMakeProjectItem* it = (QMakeProjectItem*)mSource->itemFromIndex( t );
	if ( it && it->type() == QMakeProjectItem::File )
		emit fileOpenRequested( it->data( QMakeProjectItem::AbsoluteFilePathRole ).toString() , this );
}
//
void QMakeProjectProxy::customContextMenuRequested( const QPoint& )
{
	qWarning( "context menu" );
}
