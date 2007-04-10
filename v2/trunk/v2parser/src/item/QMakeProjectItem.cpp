#include "QMakeProjectItem.h"
#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectModel.h"
//
#include <QPixmap>
//
QMakeProjectItem::QMakeProjectItem( QMakeProjectItem::NodeType t, QMakeProjectItem* p, QMakeProjectItemPrivate* pp )
{
	if ( pp )
		setData( pp );
	else
		d = new QMakeProjectItemPrivate;
	setParent( p );
	if ( parent() )
		parent()->appendRow( this );
	setData( t, QMakeProjectItem::TypeRole );
}
//
QMakeProjectItem::~QMakeProjectItem()
{
	if ( d )
		d->detach();
}
//
void QMakeProjectItem::clear()
{
	setData( 0 );
	setData( new QMakeProjectItemPrivate );
}
//
QMakeProjectItem::NodeType QMakeProjectItem::type() const
{
	return ( QMakeProjectItem::NodeType )data( QMakeProjectItem::TypeRole ).toInt();
}
//
void QMakeProjectItem::setType( QMakeProjectItem::NodeType t )
{
	setData( t, QMakeProjectItem::TypeRole );
}
//
Qt::ItemFlags QMakeProjectItem::flags() const
{
	return d ? d->mFlags : Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//
void QMakeProjectItem::setFlags( Qt::ItemFlags f )
{
	if ( d )
		d->mFlags = f;
}
//
QMakeProjectItem* QMakeProjectItem::row( int i )
{
	return d ? d->mChilds.value( i ) : 0;
}
//
void QMakeProjectItem::appendRow( QMakeProjectItem* i )
{
	insertRow( rowCount(), i );
}
//
void QMakeProjectItem::insertRow( int i, QMakeProjectItem* it )
{
	if ( d && it && model() && i > -1 && i < rowCount() +1 )
	{
		model()->beginInsertRows( model()->indexFromItem( this ), i, i );
		d->mChilds.insert( i, it );
		it->setParent( this );
		model()->endInsertRows();
	}
	Q_UNUSED( i ); // shut up gcc warning
}
//
void QMakeProjectItem::removeRow( int i )
{
	removeRow( row( i ) );
}
//
void QMakeProjectItem::removeRow( QMakeProjectItem* i )
{
	delete takeRow( i );
}
//
QMakeProjectItem* QMakeProjectItem::takeRow( int i )
{
	return takeRow( row( i ) );
}
//
QMakeProjectItem* QMakeProjectItem::takeRow( QMakeProjectItem* it )
{
	if ( d && it && model() )
	{
		int i = it->row();
		model()->beginRemoveRows( model()->indexFromItem( this ), i, i );
		d->mChilds.removeAll( it );
		it->setParent( 0 );
		model()->endRemoveRows();
		return it;
	}
	return 0;
}
//
bool QMakeProjectItem::swapRow( int i, int j )
{
	QMakeProjectItem* ii;
	QMakeProjectItem* ij;
	if ( d && ( ii = takeRow( i ) ) && ( ij = takeRow( j ) ) )
	{
		insertRow( i, ij );
		insertRow( j, ii );
		return true;
	}
	return false;
}
//
bool QMakeProjectItem::moveRowUp( int i )
{
	if ( d && i > 0 )
	{
		insertRow( i -1, takeRow( i ) );
		return true;
	}
	return false;
}
//
bool QMakeProjectItem::moveRowDown( int i )
{
	if ( d && i < d->mChilds.count() -1 )
	{
		insertRow( i +1, takeRow( i ) );
		return true;
	}
	return false;
}
//
bool QMakeProjectItem::moveUp()
{
	if ( parent() )
		return parent()->moveRowUp( row() );
	return false;
}
//
bool QMakeProjectItem::moveDown()
{
	if ( parent() )
		return parent()->moveRowDown( row() );
	return false;
}
//
QMakeProjectItem* QMakeProjectItem::parent() const
{
	return mParent;
}
//
void QMakeProjectItem::setParent( QMakeProjectItem* i )
{
	mParent = i;
	setModel( i ? i->model() : 0 );
}
//
QMakeProjectModel* QMakeProjectItem::model() const
{
	return mModel;
}
//
void QMakeProjectItem::setModel( QMakeProjectModel* m )
{
	mModel = m;
}
//
int QMakeProjectItem::rowCount() const
{
	return d ? d->mChilds.count() : 0;
}
//
int QMakeProjectItem::columnCount() const
{
	return 1;
}
//
int QMakeProjectItem::row() const
{
	if ( d && parent() )
		return parent()->d->mChilds.indexOf( const_cast<QMakeProjectItem*>( this ) );
	return 0;
}
//
int QMakeProjectItem::column() const
{
	return 1;
}
//
QVariant QMakeProjectItem::data( int r ) const
{
	return d ? d->mDatas.value( r ) : QVariant();
}
//
void QMakeProjectItem::setData( const QVariant& v, int r )
{
	if ( model() )
		model()->setData( model()->indexFromItem( this ), v, r );
}
//
void QMakeProjectItem::setData( QMakeProjectItemPrivate* p )
{
	if ( d )
		d->detach();
	if ( p )
		p->attach();
	d = p;
}
//
void QMakeProjectItem::setPrivateData( const QVariant& v, int r )
{
	if ( d )
		d->mDatas[r] = v;
}
