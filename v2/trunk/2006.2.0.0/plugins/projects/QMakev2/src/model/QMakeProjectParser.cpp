#include "QMakeProjectParser.h"
#include "QMakeProjectItem.h"
//
#include <QFileInfo>
#include <QBuffer>
// pj
#include <QVector>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
// pj
static int prof = 0; // profondeur courrante
static int this_prof = 0; // profondeur precedent
// regex !!!
static QVector<QString> content;
static bool mParseCommands = false;
static QRegExp function_call("^((?:[a-zA-Z0-9]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[|:](?:[ \\t]+)?)+)?([a-zA-Z]+)\\((.*)\\)[ \\t]*(#.*)?");
static QRegExp bloc("^(\\})?[ \\t]*(?:([-a-zA-Z0-9*|_!+]+:)*([-a-zA-Z0-9*|_!+]+))[ \\t]*(?:\\((.*)\\))?[ \\t]*(\\{)[ \\t]*(#.*)?");
static QRegExp variable("^(?:((?:[-a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[:|](?:[ \\t]+)?)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*([^\\\\#]*)[ \\t]*(\\\\)?[ \t]*(#.*)?");
static QRegExp varLine("^[ \\t]*(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
static QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
static QRegExp comments("^\\s*#(.*)");
static QRegExp explodeNested( "\\s*([^:|()]+|\\w+\\(.*\\))\\s*(:|\\|)" );
static QRegExp splitValues( "([^ \\\"]+)|\\\"(.*)\\\"" );
static QRegExp splitCommands( "(\\([^;]*\\));?|(\\$\\(\\w+\\)[^;]*);?" );
static QRegExp splitNested( "[:|\\|]" );
//
QMakeProjectParser::QMakeProjectParser( const QString& s, QMakeProjectItem* i )
	: QObject( i ), mIsOpen( false ), mRoot( i )
{
	loadFile( s, mRoot );
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
bool QMakeProjectParser::loadFile( const QString& s, QMakeProjectItem* it )
{
	// clear vectors
	content.clear();
	// open file
	QFile f( s );
	if ( !f.exists() || !f.open( QFile::ReadOnly | QFile::Text ) )
		return false;
	// trim file
	while (!f.atEnd()) {
		QByteArray line = f.readLine();
		line = line.trimmed();
		//if(line.size() > 0)
			content += line;
	}
	// set project data
	if ( it != mRoot )
		it = new QMakeProjectItem( AbstractProjectModel::ProjectType, it );
	it->setType( AbstractProjectModel::ProjectType );
	it->setData( QFileInfo( s ).completeBaseName() );
	it->setData( s, AbstractProjectModel::AbsoluteFilePathRole );
	// parse buffer
	mIsOpen = parseBuffer( 0, it );
	return mIsOpen;
}
// parser file
int QMakeProjectParser::parseBuffer( int ligne, QMakeProjectItem* it )
{
	while(ligne < content.size())
	{
		// function like :
		// func_name(params), nested optionnally
		if ( function_call.exactMatch( content[ligne] ) )
		{
			// ==================        1        ==================
			QStringList liste = function_call.capturedTexts();
			QMakeProjectItem* i = processNested( liste[1], it );
			i = addFunction( QString( "%1( %2 )" ).arg( liste[2].trimmed(), liste[3].trimmed() ), "", i );
			if ( i )
				i->setData( liste[4].trimmed(), AbstractProjectModel::CommentRole );
		}
		// scope (nested compris)
		// "truc(params) {" ou "xml:truc(params) {" ("{" facultatif)
		else if ( bloc.exactMatch( content[ligne] ) )
		{
			// if end block and start new one
			if( content[ligne].left(1) == "}" )
			{
				// drop infinite loop
				if(this_prof != prof)
				{
					this_prof = prof-1;
					return ligne-1;
				}
			}
			// ==================        2         ==================
			QStringList liste = bloc.capturedTexts();
			QMakeProjectItem* i = processNested( liste[2], it );
			QString s;
			if ( liste[4].trimmed().isEmpty() )
				s = liste[3].trimmed();
			else
				s = QString( "%1( %2 )" ).arg( liste[3].trimmed(), liste[4].trimmed() );
			i = addScope( s, "", false, i );
			if ( i )
				i->setData( liste[6].trimmed(), AbstractProjectModel::CommentRole );
			//
			prof++;
			// parse block giving parent scope
			ligne = parseBuffer( ligne+1, i );
			prof--;
		}
		// end of block
		else if ( end_bloc.exactMatch( content[ligne] ) )
			return ligne;
		// variable ( nested optionnaly)
		// "HEADERS = ***" or "win32:HEADERS = ***" ("+=" and "-=" managed)
		else if ( variable.exactMatch( content[ligne] ) )
		{
			// ==================        4         ==================
			QStringList liste = variable.capturedTexts();
			QMakeProjectItem *i, *v = processNested( liste[1], it );
			v = addVariable( liste[2], liste[3], v );
			i = processValues( liste[4], v );
			if ( i )
				i->setData( liste[6].trimmed(), AbstractProjectModel::CommentRole );
			// if last char is \ read next lines
			if ( liste[5] == "\\" )
			{
				ligne++;
				// while last car of line is \ continue to read, be carrefull on comment
				while( varLine.exactMatch( content[ligne] ) )
				{
					// ==================        5         ==================
					liste = varLine.capturedTexts();
					i = processValues( liste[1], v );
					if ( i )
						i->setData( liste[2].trimmed(), AbstractProjectModel::CommentRole );
					ligne++;
				}
				// last line , optionnally with comment
				liste = content[ligne].split( "#" );
				i = processValues( liste[0], v );
				if ( i && liste.count() > 1 )
					i->setData( liste[1], AbstractProjectModel::CommentRole );
			}
		}
		// single line comment
		else if ( comments.exactMatch( content[ligne] ) )
			addComment( content[ligne], it );
		// empty lines
		else if ( content[ligne].trimmed() == "" )
			addEmpty( it );
		// error
		else
			qWarning( "Error on line %d: %s", (ligne+1), qPrintable( content[ligne] ) );
		ligne++;
	}
	return 1;
}
// 100%
QMakeProjectItem* QMakeProjectParser::processNested( const QString& s, QMakeProjectItem* i )
{
	QStringList l;
	int p = 0;
	while ( ( p = explodeNested.indexIn( s, p ) ) != -1 )
	{
		l = explodeNested.capturedTexts();
		i = addScope( l.at( 1 ).trimmed(), l.at( 2 ).trimmed(), true, i );
		p += explodeNested.matchedLength();
	}
	return i;
}
// 100%
QMakeProjectItem* QMakeProjectParser::processValues( const QString& s, QMakeProjectItem* i )
{
	QStringList l;
	int p = 0;
	QMakeProjectItem* v = 0;
	QRegExp* r = mParseCommands ? &splitCommands : &splitValues;
	while ( ( p = r->indexIn( s, p ) ) != -1 )
	{
		l = r->capturedTexts();
		qDebug() << l;
		l.removeAll( "" );
		v = addValue( l.at( 1 ).trimmed(), i );
		p += r->matchedLength();
	}
	return v;
}
// 100%
QMakeProjectItem* QMakeProjectParser::addScope( const QString& v, const QString& o, bool b, QMakeProjectItem* i )
{
	QMakeProjectItem* s = new QMakeProjectItem( b ? AbstractProjectModel::NestedScopeType : AbstractProjectModel::ScopeType, i );
	s->setData( v.trimmed(), AbstractProjectModel::ValueRole );
	s->setData( o.trimmed(), AbstractProjectModel::NestedOperatorRole );
	(void) new QMakeProjectItem( AbstractProjectModel::ScopeEndType, s );
	return s;
}
// 100%
QMakeProjectItem* QMakeProjectParser::addFunction( const QString& s, const QString& o, QMakeProjectItem* i )
{
	QMakeProjectItem* f = new QMakeProjectItem( AbstractProjectModel::FunctionType, i );
	f->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	f->setData( o.trimmed(), AbstractProjectModel::NestedOperatorRole );
	return f;
}
// 100%
QMakeProjectItem* QMakeProjectParser::addVariable( const QString& s, const QString& o, QMakeProjectItem* i )
{
	QMakeProjectItem* v = new QMakeProjectItem( AbstractProjectModel::VariableType, i );
	v->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	v->setData( o.trimmed(), AbstractProjectModel::OperatorRole );
	mParseCommands = s.trimmed().endsWith( ".commands", Qt::CaseInsensitive );
	return v;
}
// 100%
QMakeProjectItem* QMakeProjectParser::addValue( const QString& s, QMakeProjectItem* i )
{
	QMakeProjectItem* v = new QMakeProjectItem( AbstractProjectModel::ValueType, i );
	v->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	return v;
}
// 100%
QMakeProjectItem* QMakeProjectParser::addComment( const QString& s, QMakeProjectItem* i )
{
	QMakeProjectItem* c = new QMakeProjectItem( AbstractProjectModel::CommentType, i );
	c->setData( s, AbstractProjectModel::ValueRole );
	return c;
}
// 100%
QMakeProjectItem* QMakeProjectParser::addEmpty( QMakeProjectItem* i )
{
	QMakeProjectItem* e = new QMakeProjectItem( AbstractProjectModel::EmptyType, i );
	e->setData( "", AbstractProjectModel::ValueRole );
	return e;
}
