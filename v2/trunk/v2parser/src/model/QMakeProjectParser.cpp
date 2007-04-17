#include "QMakeProjectParser.h"
#include "QMakeProjectItem.h"
//
#include <QFileInfo>
#include <QBuffer>
//
QMakeProjectParser::QMakeProjectParser( const QString& s, QMakeProjectItem* i )
	: QObject( i ), mIsOpen( false ), mRoot( i )
{
	if ( !parse( s, i ) )
		qWarning( "Can't open project file: %s", qPrintable( s ) );
}
//
QMakeProjectParser::~QMakeProjectParser()
{
}
//
bool QMakeProjectParser::isOpen() const
{
	return mIsOpen;
}
//
bool QMakeProjectParser::parse( const QString& s, QMakeProjectItem* ri )
{
	QFile f( s );
	if ( !ri || !f.exists() || !f.open( QFile::ReadOnly ) )
		return false;
	QBuffer buf( this );
	buf.setData( f.readAll() );
	if ( !buf.open( QBuffer::ReadOnly | QBuffer::Text ) )
		return false;
	//
	QMakeProjectItem* it = new QMakeProjectItem( QMakeProjectItem::ProjectType, ri );
	it->setPrivateModel( ri->model() );
	it->setType( QMakeProjectItem::ProjectType );
	it->setData( QFileInfo( s ).completeBaseName() );
	it->setData( s, QMakeProjectItem::AbsoluteFilePathRole );
	// HERE TO DROP
	//
	QMakeProjectItem* p;
	QMakeProjectItem* i;
	// pri
	QMakeProjectItem* pri = new QMakeProjectItem( QMakeProjectItem::IncludeType, it );
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
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "HEADERS", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.h" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// header
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "SOURCES", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.cpp" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// win32
	p = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	p->setData( "win32", QMakeProjectItem::ValueRole );
	// append clone
	//i = new QMakeProjectItem( QMakeProjectItem::EmptyType, p, pri->d );
	//
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, p );
	//
	mIsOpen = true;
	return true;
}
