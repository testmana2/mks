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
bool QMakeProjectParser::parse( const QString& s, QMakeProjectItem* it )
{
	QFile f( s );
	if ( !it || !f.exists() || !f.open( QFile::ReadOnly ) )
		return false;
	QBuffer buf( this );
	buf.setData( f.readAll() );
	if ( !buf.open( QBuffer::ReadOnly | QBuffer::Text ) )
		return false;
	// set project data
	it->setType( QMakeProjectItem::ProjectType );
	it->setData( QFileInfo( s ).completeBaseName() );
	it->setData( s, QMakeProjectItem::AbsoluteFilePathRole );
	//
	fakeProject( it );
	//
	mIsOpen = true;
	return true;
}
//
void QMakeProjectParser::fakeProject( QMakeProjectItem* it )
{
	return;
	QMakeProjectItem* p, * i, * s;
	// set app name
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "APP_TITLE", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "Monkey Studio", QMakeProjectItem::ValueRole );
	// set app icon
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "APP_ICON", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "/path/to/app/icon.png", QMakeProjectItem::ValueRole );
	// set app help file
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "APP_HELP_FILE", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "/path/to/app/help/file.html", QMakeProjectItem::ValueRole );
	// set app author
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "APP_AUTHOR", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "Nox P@sNox (pasnox@hotmail.com)", QMakeProjectItem::ValueRole );
	// set app version
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "VERSION", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "1.0.0.1", QMakeProjectItem::ValueRole );
	// set build autoincrement
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "APP_AUTO_INCREMENT", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "1", QMakeProjectItem::ValueRole );
	// set app template
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "TEMPLATE", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "app", QMakeProjectItem::ValueRole );
	// set app version
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "LANGUAGE", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "Qt4/C++", QMakeProjectItem::ValueRole );
	// set app config
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "CONFIG", QMakeProjectItem::ValueRole );
	p->setData( "+=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "debug_and_release", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "warn_off", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "build_all", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "qt", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "thread", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "designer", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "my_perso_config", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "coveragemeter", QMakeProjectItem::ValueRole );
	// set qt config
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "QT", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "core", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "gui", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "xml", QMakeProjectItem::ValueRole );
	// set destdir
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "DESTDIR", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "/path/for/defaut", QMakeProjectItem::ValueRole );
	// set destdir win32
	s = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	s->setData( "win32", QMakeProjectItem::ValueRole );
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, s );
	p->setData( "DESTDIR", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "/path/for/win32", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, s );
	// set destdir mac
	s = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	s->setData( "mac", QMakeProjectItem::ValueRole );
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, s );
	p->setData( "DESTDIR", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "/path/for/mac", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, s );
	// set target
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "TARGET", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "my_app_name", QMakeProjectItem::ValueRole );
	// set libs
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "LIBS", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "-ldefaut", QMakeProjectItem::ValueRole );
	// set libs win32
	s = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	s->setData( "win32", QMakeProjectItem::ValueRole );
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, s );
	p->setData( "LIBS", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "-lwin32", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, s );
	// set libs mac
	s = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	s->setData( "mac", QMakeProjectItem::ValueRole );
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, s );
	p->setData( "LIBS", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "-lmac", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, s );
	// set vpath
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "VPATH", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "vpath", QMakeProjectItem::ValueRole );
	// set vpath win32
	s = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	s->setData( "win32", QMakeProjectItem::ValueRole );
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, s );
	p->setData( "VPATH", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "vpath win32", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, s );
	// set vpath mac
	s = new QMakeProjectItem( QMakeProjectItem::NestedScopeType, it );
	s->setData( "mac", QMakeProjectItem::ValueRole );
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, s );
	p->setData( "VPATH", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "vpath mac", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ScopeEndType, s );
	// set translations
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "TRANSLATIONS", QMakeProjectItem::ValueRole );
	p->setData( "=", QMakeProjectItem::OperatorRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "translations/demos_french.ts", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "translations/demos_german.ts", QMakeProjectItem::ValueRole );
	i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
	i->setData( "translations/demos_italian.ts", QMakeProjectItem::ValueRole );
	// header
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "HEADERS", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.h" ).arg( j ), QMakeProjectItem::ValueRole );
	}
	// source
	p = new QMakeProjectItem( QMakeProjectItem::VariableType, it );
	p->setData( "SOURCES", QMakeProjectItem::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( QMakeProjectItem::ValueType, p );
		i->setData( QString( "pouet%1.cpp" ).arg( j ), QMakeProjectItem::ValueRole );
	}
}
