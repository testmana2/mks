#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
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
		mProjectFilePath = s;
		QFile f( mProjectFilePath );
		if ( f.open( QFile::ReadOnly ) )
		{
			setupModelData( f.readAll(), mRootItem );
			return true;
		}
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
	QMakeProjectItem* pItem = mRootItem;
	if ( p.isValid() )
		pItem = static_cast<QMakeProjectItem*>( p.internalPointer() );
	QMakeProjectItem* cItem = pItem->row( r );
	if ( cItem )
		return createIndex( r, c, cItem );
	return QModelIndex();
}
//
QModelIndex QMakeProjectModel::parent( const QModelIndex& i ) const
{
	if ( !i.isValid() )
		return QModelIndex();
	//
	QMakeProjectItem* cItem = static_cast<QMakeProjectItem*>( i.internalPointer() );
	QMakeProjectItem* pItem = 0;
	if ( cItem )
		pItem = cItem->parent();
	if ( !pItem || pItem == mRootItem )
		return QModelIndex();
	//
	return createIndex( pItem->row(), 0, pItem );
}
//
int QMakeProjectModel::rowCount( const QModelIndex& p ) const
{
	QMakeProjectItem* pItem = mRootItem;
	if ( p.isValid() )
		pItem = static_cast<QMakeProjectItem*>( p.internalPointer() );
	if ( pItem )
		return pItem->rowCount();
	return 0;
}
//
int QMakeProjectModel::columnCount( const QModelIndex& p ) const
{
	QMakeProjectItem* pItem = mRootItem;
	if ( p.isValid() )
		pItem = static_cast<QMakeProjectItem*>( p.internalPointer() );
	if ( pItem )
		return pItem->columnCount();
	return 0;
}
//
QVariant QMakeProjectModel::data( const QModelIndex& i, int r ) const
{
	if ( !i.isValid() )
		return QVariant();
	QMakeProjectItem* mItem = static_cast<QMakeProjectItem*>( i.internalPointer() );
	if ( mItem )
		return mItem->data( r );
	return QVariant();
}
//
bool QMakeProjectModel::setData( const QModelIndex& i, const QVariant& v, int r )
{
	if ( !i.isValid() )
		return false;
	QMakeProjectItem* mItem = static_cast<QMakeProjectItem*>( i.internalPointer() );
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
		//
		emit dataChanged( i, i );
	}
	return mItem;
}
//
Qt::ItemFlags QMakeProjectModel::flags( const QModelIndex& i ) const
{
	if ( !i.isValid() )
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	QMakeProjectItem* mItem = static_cast<QMakeProjectItem*>( i.internalPointer() );
	if ( mItem )
		return mItem->flags();
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//
QMakeProjectItem* QMakeProjectModel::itemFromIndex( const QModelIndex& i ) const
{
	if ( i.isValid() )
		return static_cast<QMakeProjectItem*>( i.internalPointer() );
	return 0;
}
//
QModelIndex QMakeProjectModel::indexFromItem( QMakeProjectItem* i ) const
{
	if ( i )
		return createIndex( i->row(), i->column(), i );
	return QModelIndex();
}
//
void QMakeProjectModel::insertRow( int r, QMakeProjectItem* i )
{
	mRootItem->insertRow( r, i );
}
//
QMakeProjectItem* QMakeProjectModel::row( int i ) const
{
	if ( i < mRootItem->rowCount() )
		return static_cast<QMakeProjectItem*>( mRootItem->row( i ) );
	return 0;
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
