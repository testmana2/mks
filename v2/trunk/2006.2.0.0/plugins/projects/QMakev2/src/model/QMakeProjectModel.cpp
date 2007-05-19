#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectParser.h"
///
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
//
QMakeProjectModel::QMakeProjectModel( const QString& s, QObject* p )
	: AbstractProjectModel( s, p ), mRootItem( new QMakeProjectItem( AbstractProjectModel::ProjectType ) )
{
	mRootItem->setPrivateModel( this );
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
		// icon update
		if ( mItem->data( Qt::DecorationRole ).value<QPixmap>().isNull() || r == AbstractProjectModel::TypeRole )
		{
			switch ( mItem->type() )
			{
				case AbstractProjectModel::EmptyType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/line.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::CommentType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/comment.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::NestedScopeType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/scope.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::ScopeType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/scope.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::ScopeEndType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/scope_end.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::VariableType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/variable.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::ValueType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/value.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::FunctionType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/function.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::IncludeType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/function.png" ), Qt::DecorationRole );
					break;
				case AbstractProjectModel::ProjectType:
					mItem->setPrivateData( QPixmap( ":/Icons/icons/project.png" ), Qt::DecorationRole );
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
		if ( ( p->type() == AbstractProjectModel::ScopeType || p->type() == AbstractProjectModel::NestedScopeType || p->type() == AbstractProjectModel::FunctionType ) && i->type() != AbstractProjectModel::ScopeEndType )
			j--;
		beginInsertRows( p->index(), j, j );
		i->setParent( p );
		p->insertPrivateRow( j, i );
		i->setData( true, AbstractProjectModel::DeleteRole );
		endInsertRows();
		if ( p == mRootItem ) // TODO: fix this fucking hack for proxy !
			reset();
	}
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
	takeRow( i, p ? p : mRootItem );
}
//
bool QMakeProjectModel::removeRows( int r, int c, const QModelIndex& p )
{
	QMakeProjectItem* pItem = p.isValid() ? static_cast<QMakeProjectItem*>( p.internalPointer() ) : mRootItem;
	bool b = false;
	if ( pItem )
	{
		beginRemoveRows( p, r, r +c -1 );
		for ( int i = r; i < r +c; i++ )
		{
			QMakeProjectItem* cItem = pItem->row( r );
			if ( cItem && pItem->removePrivateRow( r ) )
			{
				if ( cItem->data( AbstractProjectModel::DeleteRole ).toBool() )
					delete cItem;
				b = true;
			}
			else
			{
				b = false;
				break;
			}
		}
		endRemoveRows();
	}
	return b;
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
		i->setData( false, AbstractProjectModel::DeleteRole );
		removeRows( i->row(), 1, p->index() );
		i->setParent( 0 );
		return i;
	}
	return 0;
}
//
QVariant QMakeProjectModel::headerData( int i, Qt::Orientation o, int r ) const
{
	return o == Qt::Horizontal && i == 0 ? mRootItem->data( r ) : QVariant();
}
// 100%
QModelIndexList QMakeProjectModel::getIndexList( const QModelIndex& i, AbstractProjectModel::NodeType t, const QString& v, const QString& o, const QString& s ) const
{
	QModelIndexList l;
	QModelIndex p = project( i );
	foreach ( QModelIndex index, match( p.child( 0, 0 ), AbstractProjectModel::ValueRole, v, -1, Qt::MatchFixedString | Qt::MatchRecursive ) )
		if ( project( index ) == p )
			if ( isEqualScope( scopeOf( index ), s ) )
				if ( ( !o.isEmpty() && index.data( AbstractProjectModel::OperatorRole ).toString() == o ) || o.isEmpty() )
					if ( index.data( AbstractProjectModel::TypeRole ).toInt() == t )
						l << index;
	return l;
}
// TODO : fix me
QModelIndex QMakeProjectModel::getScope( const QModelIndex& i, const QString& s, bool c ) const
{
	// get project item
	QModelIndex p = project( i );
	QMakeProjectItem* sItem = itemFromIndex( p );
	// it there is scope
	if ( !s.trimmed().isEmpty() )
	{
		static QRegExp r( "([^:|]+)(\\:|\\|)?" );
		int pos = 0;
		QString cScope = checkScope( s );
		// create scope is there are not existing
		while ( ( pos = r.indexIn( cScope, pos ) ) != -1 )
		{
			QStringList l = r.capturedTexts();
			QString s1 = l.at( 1 );
			QString s2 = l.at( 2 );
			QString sScope = cScope.left( pos ).append( s1 );
			// search existing scope
			QModelIndex fItem = ( getIndexList( p, AbstractProjectModel::ScopeType, s1, s2, sScope ) << getIndexList( p, AbstractProjectModel::NestedScopeType, s1, s2, sScope ) ).value( 0 );
			// create scope
			if ( ( !fItem.isValid() || project( fItem ) != p ) )
			{
				if ( c )
				{
					sItem = new QMakeProjectItem( l.at( 2 ).trimmed().isEmpty() ? AbstractProjectModel::ScopeType : AbstractProjectModel::NestedScopeType, sItem );
					sItem->setData( s1, AbstractProjectModel::ValueRole );
					sItem->setData( s2, AbstractProjectModel::OperatorRole );
					(void) new QMakeProjectItem( AbstractProjectModel::ScopeEndType, sItem );
				}
				else
					return indexFromItem( sItem );
			}
			else
				sItem = itemFromIndex( fItem );
			pos += r.matchedLength();
		}
		// if scope is nested, made it ScopeType
		if ( sItem && sItem->type() == AbstractProjectModel::NestedScopeType )
		{
			sItem->setType( AbstractProjectModel::ScopeType );
			sItem->setData( "", AbstractProjectModel::OperatorRole );
		}
	}
	return indexFromItem( sItem );
}
// 100%
QString QMakeProjectModel::scopeOf( const QModelIndex& i ) const
{
	QString s;
	int t;
	QModelIndex j = i;
	QModelIndex p = project( j );
	while ( j.isValid() && j != p )
	{
		t = j.data( AbstractProjectModel::TypeRole ).toInt();
		QString v = j.data( AbstractProjectModel::ValueRole ).toString();
		QString o = j.data( AbstractProjectModel::OperatorRole ).toString();
		if ( t == AbstractProjectModel::ScopeType || t == AbstractProjectModel::NestedScopeType )
			s.prepend( QString( "%1%2" ).arg( v, o.isEmpty() ? ":" : o ) );
		j = j.parent();
	}
	return s ;
}
// 100%
QString QMakeProjectModel::checkScope( const QString& s ) const
{
	// remove trailing : as it s the default operator
	QString ss = s.trimmed();
	if ( !ss.isEmpty() && ss.endsWith( ':' ) )
		ss.chop( 1 );
	return ss;
}
// 100%
bool QMakeProjectModel::isEqualScope( const QString& s1, const QString& s2 ) const
{
	QString ss1( s1.toLower() ), ss2( s2.toLower() );
	ss1.replace( '/', ':' );
	ss2.replace( '/', ':' );
	return checkScope( ss1 ) == checkScope( ss2 );
}
// 100%
QModelIndexList QMakeProjectModel::getIndexListValues( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	QModelIndexList l = getIndexList( i, AbstractProjectModel::VariableType, v, o, s );
	QModelIndexList ll;
	foreach ( QModelIndex index, l )
		for ( int j = 0; j < rowCount( index ); j++ )
			if ( index.child( j, 0 ).data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ValueType )
				ll << index.child( j, 0 );
	return ll;
}
// 100%
QModelIndex QMakeProjectModel::getIndexVariable( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	return getIndexList( i, AbstractProjectModel::VariableType, v, o, s ).value ( 0 );
}
// 100%
QStringList QMakeProjectModel::getListValues( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	QStringList l;
	foreach ( QModelIndex index, getIndexList( i, AbstractProjectModel::VariableType, v, o, s ) )
		for ( int j = 0; j < rowCount( index ); j++ )
			if ( index.child( j, 0 ).data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ValueType )
				l << index.child( j, 0 ).data( AbstractProjectModel::ValueRole ).toString();
	return l;
}
// 100%
QString QMakeProjectModel::getStringValues( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	return getListValues( v, i, o, s ).join( " " );
}
// 100%
void QMakeProjectModel::setListValues( const QStringList& vl, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	QModelIndex p = project( i );
	QModelIndex it = getIndexVariable( v, p, o, s );
	// create index if it not exists
	if ( !it.isValid() )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		QMakeProjectItem* vItem = new QMakeProjectItem( AbstractProjectModel::VariableType, itemFromIndex( getScope( p, s, true ) ) );
		vItem->setData( v, AbstractProjectModel::ValueRole );
		vItem->setData( o, AbstractProjectModel::OperatorRole );
		it = vItem->index();
	}
	// set values
	QStringList l = vl;
	// check all child of variable and remove from model/list unneeded index/value
	for ( int j = rowCount( it ) -1; j > -1; j-- )
	{
		QModelIndex c = it.child( j, 0 );
		if ( c.data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ValueType )
		{
			QString d = c.data().toString();
			if ( l.contains( d ) )
				l.removeAll( d );
			else
				QAbstractItemModel::removeRow( c.row(), c.parent() );
		}
	}
	// add require index
	QMakeProjectItem* pItem = static_cast<QMakeProjectItem*>( it.internalPointer() );
	if ( pItem )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				QMakeProjectItem* cItem = new QMakeProjectItem( AbstractProjectModel::ValueType, pItem );
				cItem->setData( e, AbstractProjectModel::ValueRole );
			}
		}
		// if variable is empty, remove it
		if ( !pItem->rowCount() )
			QAbstractItemModel::removeRow( it.row(), it.parent() );
	}
}
// 100%
void QMakeProjectModel::setStringValues( const QString& val, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	setListValues( val.isEmpty() ? QStringList() : QStringList( val ), v, i, o, s );
}
// 100%
void QMakeProjectModel::addListValues( const QStringList& vl, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	QModelIndex p = project( i );
	QModelIndex it = getIndexVariable( v, p, o, s );
	// create index if it not exists
	if ( !it.isValid() )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		QMakeProjectItem* vItem = new QMakeProjectItem( AbstractProjectModel::VariableType, itemFromIndex( getScope( p, s, true ) ) );
		vItem->setData( v, AbstractProjectModel::ValueRole );
		vItem->setData( o, AbstractProjectModel::OperatorRole );
		it = vItem->index();
	}
	// add values
	QStringList l = vl;
	// check all values of variable and remove from list already existing values
	for ( int j = rowCount( it ) -1; j > -1; j-- )
	{
		QModelIndex c = it.child( j, 0 );
		if ( c.data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ValueType )
		{
			QString d = c.data().toString();
			if ( l.contains( d ) )
				l.removeAll( d );
		}
	}
	// add require values
	QMakeProjectItem* pItem = static_cast<QMakeProjectItem*>( it.internalPointer() );
	if ( pItem )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				QMakeProjectItem* cItem = new QMakeProjectItem( AbstractProjectModel::ValueType, pItem );
				cItem->setData( e, AbstractProjectModel::ValueRole );
			}
		}
	}
}
//
void QMakeProjectModel::addStringValues( const QString& val, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	addListValues( QStringList( val ), v, i, o, s );
}
//
bool QMakeProjectModel::open()
{
	QMakeProjectParser p( mRootFilePath, mRootItem );
	return p.isOpen();
}
//
QModelIndex QMakeProjectModel::rootProject() const
{
	return mRootItem->index();
}
//
void QMakeProjectModel::close( const QModelIndex& i )
{
	QModelIndex j = project( i );
	foreach ( QModelIndex p, subProjects( j ) )
		close( p );
	save( j );
	emit aboutToClose( j );
	if ( i != rootProject() )
		QAbstractItemModel::removeRow( i.row(), i.parent() );
	else
		deleteLater();
}
//
void QMakeProjectModel::save( const QModelIndex& i )
{
	QModelIndex j = project( i );
	if ( isModified( j ) )
	{
		// save j
		setModified( j, false );
	}
}
//
void QMakeProjectModel::saveAll()
{
	foreach ( QModelIndex i, subProjects( rootProject(), true ) )
		save( i );
}
//
void QMakeProjectModel::prepareCompletion()
{
	qWarning( "QMakeProjectModel::prepareCompletion() : Not yet available" );
}
//
/*
***********************************************************
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
QMakeProjectItem* QMakeProjectModel::project( const QModelIndex& i )
{
	if ( !i.isValid() )
		return isOpen() ? mRootItem : 0;
	QMakeProjectItem* it = static_cast<QMakeProjectItem*>( i.internalPointer() );
	while ( it && it->data( QMakeAbstractProjectModel::TypeRole ).toInt() != QMakeAbstractProjectModel::ProjectType )
		it = it->parent();
	return it;
}
***********************************************************
*/
