#include "QMakeProjectItem.h"
#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectModel.h"
//
#include <QPixmap>
//
QMakeProjectItem::QMakeProjectItem( QMakeProjectItem::NodeType t, QMakeProjectItem* p, QMakeProjectItemPrivate* pp )
	: mParent( 0 ), mModel( 0 ), d( 0 )
{
	setParent( p );
	if ( pp )
		setData( pp );
	else
		d = new QMakeProjectItemPrivate;
	if ( parent() )
		parent()->appendRow( this );
	setData( t, QMakeProjectItem::TypeRole );
	setFlags( QMakeProjectModel::defaultFlags() );
}
//
QMakeProjectItem::~QMakeProjectItem()
{
	if ( d )
		d->detach();
}
//
QModelIndex QMakeProjectItem::index() const
{
	return model() ? model()->indexFromItem( this ) : QModelIndex();
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
#ifdef QT_NO_DEBUG
	return d ? d->mDatas.value( r ) : QVariant();
#else
	QVariant v = d ? d->mDatas.value( r ) : QVariant();
	if ( !v.isValid() && r == Qt::DisplayRole )
		v = QString();
	return v;
#endif
}
//
void QMakeProjectItem::setData( const QVariant& v, int r )
{
	if ( model() )
		model()->setData( index(), v, r );
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
}
//
QMakeProjectModel* QMakeProjectItem::model() const
{
	return parent() ? parent()->model() : mModel;
}
//
int QMakeProjectItem::row() const
{
	return parent() ? parent()->rows().indexOf( const_cast<QMakeProjectItem*>( this ) ) : 0;
}
//
int QMakeProjectItem::column() const
{
	return 0;
}
//
int QMakeProjectItem::rowCount() const
{
	return rows().count();
}
//
int QMakeProjectItem::columnCount() const
{
	return 1;
}
//
QMakeProjectItem* QMakeProjectItem::row( int i )
{
	return i >= 0 && rowCount() > i ? rows().at( i ) : 0;
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
	QMakeProjectItem* ii = 0;
	QMakeProjectItem* ij = 0;
	if ( ( ii = takeRow( i ) ) && ( ij = takeRow( j ) ) )
	{
		insertRow( i, ij );
		insertRow( j, ii );
	}
	return ii && ij;
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
	if ( i < rowCount() -1 )
		insertRow( i +1, takeRow( i ) );
	return i < rowCount() -1;
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
//
void QMakeProjectItem::setPrivateModel( QMakeProjectModel* m )
{
	mModel = m;
}
//
void QMakeProjectItem::setPrivateData( const QVariant& v, int r )
{
	if ( d )
		d->mDatas[r] = v;
}
//
bool QMakeProjectItem::insertPrivateRow( int i, QMakeProjectItem* it )
{
	if ( d && i >= 0 && i <= rowCount() )
	{
		d->mChilds.insert( i, it );
		it->setParent( this );
		return true;
	}
	return false;
}
//
bool QMakeProjectItem::removePrivateRow( int i )
{
	if ( d && i >= 0 && rowCount() > i  )
	{
		d->mChilds.removeAt( i );
		return true;
	}
	return false;
}
