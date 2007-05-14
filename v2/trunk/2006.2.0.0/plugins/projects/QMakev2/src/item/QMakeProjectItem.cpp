#include "QMakeProjectItem.h"
#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectModel.h"
//
#include <QPixmap>
//
QMakeProjectItem::QMakeProjectItem( AbstractProjectModel::NodeType t, QMakeProjectItem* p, QMakeProjectItemPrivate* pp )
	: mParent( 0 ), mModel( 0 ), d( 0 )
{
	// set data object
	if ( pp )
		setData( pp );
	else
		d = new QMakeProjectItemPrivate;
	// set data
	setFlags( QMakeProjectModel::defaultFlags() );
	setData( t, AbstractProjectModel::TypeRole );
	setData( true, AbstractProjectModel::DeleteRole );
	// append to parent
	if ( p )
		p->appendRow( this );
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
AbstractProjectModel::NodeType QMakeProjectItem::type() const
{
	return ( AbstractProjectModel::NodeType )data( AbstractProjectModel::TypeRole ).toInt();
}
//
void QMakeProjectItem::setType( AbstractProjectModel::NodeType t )
{
	setData( t, AbstractProjectModel::TypeRole );
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
		model()->setData( index(), v, r );
	else
		setPrivateData( v, r );
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
	return i >= 0 && rowCount() > i ? rows().value( i ) : 0;
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
	// we can't move up the scope end item
	QMakeProjectItem* it = row( i );
	if ( !it || ( it && it->data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ScopeEndType ) )
		return false;
	// if valid, move it
	if ( i > 0 )
		insertRow( i -1, takeRow( i ) );
	return i > 0;
}
//
bool QMakeProjectItem::moveRowDown( int i )
{
	// if in a scope we can t go to very last item as it s the scope end
	QMakeProjectItem* it = row( i +1 );
	if ( !it || ( it && it->data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ScopeEndType ) )
		return false;
	// if valid, move it
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
