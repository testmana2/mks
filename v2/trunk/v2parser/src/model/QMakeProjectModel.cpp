#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectParser.h"
///
#include <QPixmap>
//
QMakeProjectModel::QMakeProjectModel( const QString& s, QObject* p )
	: QAbstractItemModel( p ), mOpen( false ), mRootItem( new QMakeProjectItem( QMakeProjectItem::RootType ) )
{
	mRootItem->setPrivateModel( this );
	if ( !openProject( s ) )
		qWarning( "Can't open project: %s", qPrintable( s ) );
}
//
QMakeProjectModel::~QMakeProjectModel()
{
	delete mRootItem;
}
//
bool QMakeProjectModel::openProject( const QString& s )
{
	QMakeProjectParser parser( s, mRootItem );
	mOpen = parser.isOpen();
	return mOpen;
}
//
bool QMakeProjectModel::isOpen() const
{
	return mOpen;
}
//
QModelIndex QMakeProjectModel::index( int r, int c, const QModelIndex& p ) const
{
	QMakeProjectItem* pItem = p.isValid() ? static_cast<QMakeProjectItem*>( p.internalPointer() ) : mRootItem;
	QMakeProjectItem* cItem = pItem->row( r );
	return cItem && c == 0 ? createIndex( r, 0, cItem ) : QModelIndex();
}
//
QModelIndex QMakeProjectModel::parent( const QModelIndex& i ) const
{
	QMakeProjectItem* cItem = static_cast<QMakeProjectItem*>( i.internalPointer() );
	if ( !i.isValid() || !cItem || cItem->parent() == mRootItem )
		return QModelIndex();
	QMakeProjectItem* pItem = cItem->parent();
	return pItem ? createIndex( pItem->row(), 0, pItem ) : QModelIndex() ;
}
//
int QMakeProjectModel::rowCount( const QModelIndex& p ) const
{
	QMakeProjectItem* pItem = p.isValid() ? static_cast<QMakeProjectItem*>( p.internalPointer() ) : mRootItem;
	return pItem ? pItem->rowCount() : 0;
}
//
int QMakeProjectModel::columnCount( const QModelIndex& p ) const
{
	QMakeProjectItem* pItem = p.isValid() ? static_cast<QMakeProjectItem*>( p.internalPointer() ) : mRootItem;
	return pItem ? pItem->columnCount() : 0;
}
//
QVariant QMakeProjectModel::data( const QModelIndex& i, int r ) const
{
	QMakeProjectItem* mItem = i.isValid() ? static_cast<QMakeProjectItem*>( i.internalPointer() ) : 0;
	return mItem ? mItem->data( r ) : QVariant();	
}
//
bool QMakeProjectModel::setData( const QModelIndex& i, const QVariant& v, int r )
{
	QMakeProjectItem* mItem = i.isValid() ? static_cast<QMakeProjectItem*>( i.internalPointer() ) : 0;
	if ( mItem )
	{
		mItem->setPrivateData( v, r );
		if ( r == QMakeProjectItem::ValueRole )
			mItem->setPrivateData( v );
		// icon update
		if ( mItem->data( Qt::DecorationRole ).value<QPixmap>().isNull() || r == QMakeProjectItem::TypeRole )
		{
			switch ( mItem->type() )
			{
				case QMakeProjectItem::EmptyType:
					mItem->setPrivateData( QPixmap( ":/Images/images/line.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::CommentType:
					mItem->setPrivateData( QPixmap( ":/Images/images/line.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::NestedScopeType:
					mItem->setPrivateData( QPixmap( ":/Images/images/scope.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::ScopeType:
					mItem->setPrivateData( QPixmap( ":/Images/images/scope.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::ScopeEndType:
					mItem->setPrivateData( QPixmap( ":/Images/images/scope_end.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::VariableType:
					mItem->setPrivateData( QPixmap( ":/Images/images/variable.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::ValueType:
					mItem->setPrivateData( QPixmap( ":/Images/images/value.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::FunctionType:
					mItem->setPrivateData( QPixmap( ":/Images/images/function.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::IncludeType:
					mItem->setPrivateData( QPixmap( ":/Images/images/function.png" ), Qt::DecorationRole );
					break;
				case QMakeProjectItem::ProjectType:
					mItem->setPrivateData( QPixmap( ":/Images/images/project.png" ), Qt::DecorationRole );
					break;
				default:
					break;
			}
		}
		emit dataChanged( i, i );
	}
	return mItem;
}
//
Qt::ItemFlags QMakeProjectModel::defaultFlags()
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//
Qt::ItemFlags QMakeProjectModel::flags( const QModelIndex& i ) const
{
	QMakeProjectItem* mItem = i.isValid() ? static_cast<QMakeProjectItem*>( i.internalPointer() ) : 0;
	return mItem ? mItem->flags() : defaultFlags();
}
//
QMakeProjectItem* QMakeProjectModel::itemFromIndex( const QModelIndex& i ) const
{
	return i.isValid() ? static_cast<QMakeProjectItem*>( i.internalPointer() ) : 0;
}
//
QModelIndex QMakeProjectModel::indexFromItem( const QMakeProjectItem* i ) const
{
	return i ? createIndex( i->row(), i->column(), const_cast<QMakeProjectItem*>( i ) ) : QModelIndex();
}
//
QMakeProjectItem* QMakeProjectModel::row( int i, QMakeProjectItem* p ) const
{
	return p ? p->row( i ) : mRootItem->row( i );
}
//
void QMakeProjectModel::appendRow( QMakeProjectItem* i, QMakeProjectItem* p )
{
	p = p ? p : mRootItem;
	insertRow( p->rowCount(), i, p );
}
//
void QMakeProjectModel::insertRow( int j, QMakeProjectItem* i, QMakeProjectItem* p )
{
	if ( i && ( p = p ? p : mRootItem ) && -1 < j && p->rowCount() +1 > j )
	{
		beginInsertRows( p->index(), j, j );
		p->insertPrivateRow( j, i );
		endInsertRows();
	}
}
//
bool QMakeProjectModel::insertRows( int, int, const QModelIndex& ) 
{
	return false;
}
//
void QMakeProjectModel::removeRow( int i, QMakeProjectItem* p )
{
	p = p ? p : mRootItem;
	removeRow( row( i, p ), p );
}
//
void QMakeProjectModel::removeRow( QMakeProjectItem* i, QMakeProjectItem* p )
{
	delete takeRow( i, p ? p : mRootItem );
}
//
bool QMakeProjectModel::removeRows( int r, int c, const QModelIndex& p )
{
	QMakeProjectItem* pItem = p.isValid() ? static_cast<QMakeProjectItem*>( p.internalPointer() ) : mRootItem;
	if ( pItem )
	{
		beginRemoveRows( p, r, r +c -1 );
		for ( int i = r; i < r +c; i++ )
			if ( r < rowCount( p ) )
				if ( !pItem->removePrivateRow( i ) )
					return false;
		endRemoveRows();
		return true;
	}
	return false;
}
//
QMakeProjectItem* QMakeProjectModel::takeRow( int i, QMakeProjectItem* p )
{
	return takeRow( ( p ? p : mRootItem )->row( i ) );
}
//
QMakeProjectItem* QMakeProjectModel::takeRow( QMakeProjectItem* i, QMakeProjectItem* p )
{
	if ( ( p = p ? p : mRootItem ) && p->rows().contains( i ) )
	{
		int j = i->row();
		beginRemoveRows( indexFromItem( p ), j, j );
		p->removePrivateRow( j );
		endRemoveRows();
		return i;
	}
	return 0;
}
//
QVariant QMakeProjectModel::headerData( int i, Qt::Orientation o, int r ) const
{
	return o == Qt::Horizontal && i == 0 ? mRootItem->data( r ) : QVariant();
}
//
QStringList QMakeProjectModel::getListValues( const QString& v, const QString& o, const QString& s )
{
	// TODO: Fix this member to allow scope to be like path, for imbriqued scopes, ie : win32/!debug/
	QModelIndexList indexes = match( index( 0, 0 ), Qt::DisplayRole, v, -1, Qt::MatchFixedString | Qt::MatchRecursive );
	QStringList l;
	foreach ( QModelIndex i, indexes )
		if ( ( s.isEmpty() && i.parent().data( QMakeProjectItem::TypeRole ).toInt() != QMakeProjectItem::NestedScopeType && i.parent().data( QMakeProjectItem::TypeRole ).toInt() != QMakeProjectItem::ScopeType ) ||
			( i.parent().parent() == QModelIndex() && ( i.parent().data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::NestedScopeType || i.parent().data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::ScopeType ) && i.parent().data( QMakeProjectItem::ValueRole ).toString().toLower() == s.toLower() ) )
			if ( i.data( QMakeProjectItem::OperatorRole ).toString() == o )
				for ( int j = 0; j < rowCount( i ); j++ )
					if ( i.child( j, 0 ).data( QMakeProjectItem::TypeRole ).toInt() == QMakeProjectItem::ValueType )
						l << i.child( j, 0 ).data( QMakeProjectItem::ValueRole ).toString();
	return l;
}
//
QString QMakeProjectModel::getStringValues( const QString& v, const QString& o, const QString& s )
{
	return getListValues( v, o, s ).join( " " );
}
