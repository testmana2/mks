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
	return cItem && c == 0 ? createIndex( r, 0, cItem ) : QModelIndex();
}
//
#include <QDebug>
QModelIndex QMakeProjectModel::parent( const QModelIndex& i ) const
{
/*
	qWarning( "index: %s", qPrintable( i.data().toString() ) );
	qDebug() << i;
*/
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
		beginInsertRows( indexFromItem( p ), j, j );
		p->d->mChilds.insert( j, i );
		i->setParent( p );
		endInsertRows();
	}
	Q_UNUSED( j ); // shut up gcc warning
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
//
void QMakeProjectModel::debugModel( QMakeProjectItem* p )
{
	qWarning( "item: %s", qPrintable( p ? p->data().toString() : "null" ) );
	foreach ( QMakeProjectItem* i, ( p ? p : mRootItem )->rows() )
		debugModel( i );
}
