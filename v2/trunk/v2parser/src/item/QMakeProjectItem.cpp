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
	return d ? d->mFlags : QMakeProjectModel::defaultFlags();
}
//
void QMakeProjectItem::setFlags( Qt::ItemFlags f )
{
	if ( d )
		d->mFlags = f;
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
int QMakeProjectItem::row() const
{
	return parent() && parent()->d ? parent()->d->mChilds.indexOf( const_cast<QMakeProjectItem*>( this ) ) : 0;
}
//
int QMakeProjectItem::column() const
{
	return 1;
}
//
void QMakeProjectItem::setPrivateData( const QVariant& v, int r )
{
	if ( d )
		d->mDatas[r] = v;
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
QMakeProjectItem* QMakeProjectItem::row( int i )
{
	return rowCount() > i ? d->mChilds.at( i ) : 0;
}
//
QList<QMakeProjectItem*> QMakeProjectItem::rows() const
{
	return d ? d->mChilds : QList<QMakeProjectItem*>();
}
//
QMakeProjectItem* QMakeProjectItem::takeRow( int i )
{
	return takeRow( row( i ) );
}
//
QMakeProjectItem* QMakeProjectItem::takeRow( QMakeProjectItem* i )
{
	return model() ? model()->takeRow( i, this ) : 0;
}
//
void QMakeProjectItem::removeRow( int i )
{
	removeRow( row( i ) );
}
//
void QMakeProjectItem::removeRow( QMakeProjectItem* i )
{
	if ( model() )
		model()->removeRow( i, this );
}
//
void QMakeProjectItem::appendRow( QMakeProjectItem* i )
{
	insertRow( rowCount(), i );
}
//
void QMakeProjectItem::insertRow( int i, QMakeProjectItem* it )
{
	if ( model() )
		model()->insertRow( i, it, this );
}
//
bool QMakeProjectItem::swapRow( int i, int j )
{
	QMakeProjectItem* ii;
	QMakeProjectItem* ij;
	if ( ( ii = takeRow( i ) ) && ( ij = takeRow( j ) ) )
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
	if ( i > 0 )
		insertRow( i -1, takeRow( i ) );
	return i > 0;
}
//
bool QMakeProjectItem::moveRowDown( int i )
{
	if ( rowCount() -1 > i )
		insertRow( i +1, takeRow( i ) );
	return rowCount() -1 > i;
}
//
bool QMakeProjectItem::moveUp()
{
	return parent() ? parent()->moveRowUp( row() ) : false;
}
//
bool QMakeProjectItem::moveDown()
{
	return parent()  ? parent()->moveRowDown( row() ) : false;
}
