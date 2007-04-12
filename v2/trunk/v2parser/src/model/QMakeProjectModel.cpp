#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectParser.h"
//
#include <QFileInfo>
#include <QBuffer>
#include <QPixmap>
//
QMakeProjectModel::QMakeProjectModel( const QString& s, QObject* p )
	: QAbstractItemModel( p ), mOpen( false ), mProjectFilePath( s )
{
	mRootItem = new QMakeProjectItem( QMakeProjectItem::ProjectType );
}
//
bool QMakeProjectModel::openProject( const QString& s )
{
	if ( QFile::exists( s ) && ( s != mProjectFilePath || !mOpen ) )
	{
		QFile f( ( mProjectFilePath = s ) );
		if ( f.open( QFile::ReadOnly ) )
			return setupModelData( f.readAll(), mRootItem );
	}
	return false;
}
//
bool QMakeProjectModel::openProject()
{
	return openProject( mProjectFilePath );
}
//
QMakeProjectModel::~QMakeProjectModel()
{
	delete mRootItem;
}
//
QModelIndex QMakeProjectModel::index( int r, int c, const QModelIndex& p ) const
{
	QMakeProjectItem* pItem = p.isValid() ? static_cast<QMakeProjectItem*>( p.internalPointer() ) : mRootItem;
	QMakeProjectItem* cItem = pItem->row( r );
	return cItem ? createIndex( r, c, cItem ) : QModelIndex();
}
//
QModelIndex QMakeProjectModel::parent( const QModelIndex& i ) const
{
	QMakeProjectItem* cItem = static_cast<QMakeProjectItem*>( i.internalPointer() );
	QMakeProjectItem* pItem = 0;
	if ( !i.isValid() || !cItem || ( pItem = cItem->parent() ) == mRootItem )
		return QModelIndex();
	return createIndex( pItem->row(), pItem->column(), pItem );
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
int QMakeProjectModel::rowCount( const QMakeProjectItem* p ) const
{
	return p ? p->rowCount() : mRootItem->rowCount();
}
//
int QMakeProjectModel::columnCount( const QMakeProjectItem* p ) const
{
	return p ? p->rowCount() : mRootItem->columnCount();
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
Qt::ItemFlags QMakeProjectModel::flags( const QModelIndex& i ) const
{
	QMakeProjectItem* mItem = i.isValid() ? static_cast<QMakeProjectItem*>( i.internalPointer() ) : 0;
	return mItem ? mItem->flags() : Qt::ItemIsEnabled | Qt::ItemIsSelectable;
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
	return i < rowCount( p ) ? static_cast<QMakeProjectItem*>( ( p ? p : mRootItem )->row( i ) ) : 0;
}

//
void QMakeProjectModel::appendRow( QMakeProjectItem* i, QMakeProjectItem* p )
{
	insertRow( rowCount( indexFromItem( p ) ), i, p );
}
//
void QMakeProjectModel::insertRow( int j, QMakeProjectItem* i, QMakeProjectItem* p )
{
	if ( !p )
		p = mRootItem;
	if ( i && i->d && j > -1 && j < p->rowCount() +1 )
	{
		beginInsertRows( indexFromItem( p ), j, j );
		p->d->mChilds.insert( j, i );
		i->setParent( p );
		endInsertRows();
	}
	Q_UNUSED( j ); // shut up gcc warning
}
//
void QMakeProjectModel::removeRow( int i, QMakeProjectItem* p )
{
	if ( !p )
		p = mRootItem;
	removeRow( p->row( i ), p );
}
//
void QMakeProjectModel::removeRow( QMakeProjectItem* i, QMakeProjectItem* p )
{
	delete takeRow( i, p );
}
//
QMakeProjectItem* QMakeProjectModel::takeRow( int i, QMakeProjectItem* p )
{
	if ( !p )
		p = mRootItem;
	return takeRow( p->row( i ) );
}
//
QMakeProjectItem* QMakeProjectModel::takeRow( QMakeProjectItem* i, QMakeProjectItem* p )
{
	if ( !p )
		p = mRootItem;
	if ( i && i->d )
	{
		int j = i->row();
		beginRemoveRows( indexFromItem( p ), j, j );
		i->d->mChilds.removeAll( i );
		i->setParent( 0 );
		endRemoveRows();
		return i;
	}
	return 0;
}

bool QMakeProjectModel::removeRows( int r, int c, const QModelIndex& p )
{
	if ( !p.isValid() )
		return false;
	QMakeProjectItem* mItem = static_cast<QMakeProjectItem*>( p.internalPointer() );
	bool b = false;
	if ( mItem )
	{
		for ( int i = 0; i < c; i++ )
		{
			if ( r < mItem->rowCount() )
			{
				mItem->removeRow( r );
				b = true;
			}
		}
	}
	return b;
}
//
QVariant QMakeProjectModel::headerData( int i, Qt::Orientation o, int r ) const
{
	if ( o == Qt::Horizontal && i == 0 )
		return mRootItem->data( r );
    return QVariant();
}
//
void QMakeProjectModel::pReset()
{
	//reset();
}
//
bool QMakeProjectModel::setupModelData( const QByteArray& b, QMakeProjectItem* pi )
{
	QBuffer buf( this );
	buf.setData( b );
	if ( !buf.open( QFile::ReadOnly | QFile::Text ) )
		return false;
	//
	mRootItem->clear();
	mRootItem->setModel( this );
	mRootItem->setType( QMakeProjectItem::ProjectType );
	mRootItem->setData( QFileInfo( mProjectFilePath ).completeBaseName() );
	mRootItem->setData( mProjectFilePath, QMakeProjectItem::AbsoluteFilePathRole );
	//
	//QMakeProjectParser parser( buf, pi );
	//return true;
	//
	QMakeProjectItem* p;
	QMakeProjectItem* i;
	// pri
	QMakeProjectItem* pri = new QMakeProjectItem( QMakeProjectItem::IncludeType, pi );
	pri->setData( "pri", QMakeProjectItem::ValueRole );
	// header pri
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, pri );
	p->setData( "HEADERS", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.h" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// source pri
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, pri );
	p->setData( "SOURCES", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.cpp" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// header
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, pi );
	p->setData( "HEADERS", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.h" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// header
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, pi );
	p->setData( "SOURCES", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.cpp" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// win32
	p = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, pi );
	p->setData( "win32", QMakeProjectItem::ValueRole );
	// append clone
	//i = new QMakeProjectItem( QMakeProjectItem::EmptyType, p, pri->d );
	//
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, p );
	return true;
}
