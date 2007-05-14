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
static int this_prof = 0; // profondeur pr�c�dent
// regex !!!
static QVector<QString> content;
static QRegExp function_call("^((?:[a-zA-Z0-9]+(?:\\((?:.*)\\))?:)+)?([a-zA-Z]+)\\((.*)\\)[ \\t]*(#.*)?");
static QRegExp bloc("^(\\})?[ \\t]*((?:[-a-zA-Z0-9*|_!+]+:)*[-a-zA-Z0-9*|_!+]+)[ \\t]*(?:\\((.*)\\))?[ \\t]*(\\{)[ \\t]*(#.*)?");
static QRegExp variable("^(?:((?:[-a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?(?:[\\t]+)?:(?:[\\t]+)?)+)?([a-zA-Z0-9*!_]+))[ \\t]*([*+-]?=)[ \\t]*([^\\\\#]*)[ \\t]*(\\\\)?[ \t]*(#.*)?");
static QRegExp varLine("^[ \\t]*(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
static QRegExp explodeVars( "(\"?([][\\\\/\\w\\s]+\\.\\w+)\"?)" );
static QRegExp explodeNested( "([^:\\|]+)+(:|\\|)" );
static QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
static QRegExp comments("^#(.*)");
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
QStringList QMakeProjectParser::explodeValue(const QString& s)
{
	QStringList explode = s.split(QChar(' '));
	QStringList returnValue;
	QString temp;
	bool inString = false;

	foreach(QString v,explode)
	{
		if(inString == true)
		{
			if(v.right(1) == "\"")
			{
				temp += " ";
				temp += v.left(v.length()-1);
				inString = false;
				returnValue += temp;
			}
			else
			{
				temp += " ";
				temp += v;
			}
		}
		else
		{
			if(v.left(1) == "\"")
			{
				temp = v.right(v.length()-1);
				inString = true;
			}
			else
			{
				if(v != "")
					returnValue += v.trimmed();
			}
		}
	}
 
        return returnValue;
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
		if(line.size() > 0)
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
		// c'est une fonction du style :
		// func_name(params)
		if( function_call.exactMatch( content[ligne] ) )
		{
			// ==================        1        ==================
			QStringList liste = function_call.capturedTexts();
			addFunction( liste[1], liste[2], liste[3], liste[4], it );
		}
		// scope (nested compris)
		// "truc(params) {" ou "xml:truc(params) {" ("{" facultatif)
		else if(bloc.exactMatch(content[ligne]))
		{
			QStringList liste = bloc.capturedTexts();
			// ==================        2         ==================
			qWarning() << "ligne : " << ligne << " scope found !" << liste[1] << liste[2] << liste[3] << liste[4] << liste[5] << "end scope !";
			
			// si c'est la fin d'un bloc tout en tant le d�but d'un autre
			if(content[ligne].left(1) == "}")
			{
				// on �vite les r�cursions infinies ...
				if(this_prof != prof)
				{
					// ... car on renvoie sur la m�me ligne
					qWarning() << "ligne : " << ligne << " DEBUG 1 ::: " << content[ligne];
					return ligne-1;
				}
			}
			
			prof++;
			// parse le bloc en le transmettant en parent
			ligne = parseBuffer(ligne+1,it);
			prof--;
		}
		// fin de bloc seul
		else if(end_bloc.exactMatch(content[ligne]))
		{
			// ==================        3         ==================
			QStringList liste = end_bloc.capturedTexts();
			qWarning() << "ligne : " << ligne << " end scope found !" << liste[1] << liste[2] << "end end scope !";
			
			return ligne;
		}
		// variable (nested compris)
		// "HEADERS = ***" ou "win32:HEADERS = ***" ("+=" et "-=" sont aussi g�r�s)
		else if( variable.exactMatch( content[ligne] ) )
		{
			// ==================        4         ==================
			QStringList liste = variable.capturedTexts();
			qWarning( "line: %d", ligne );
			qDebug() << liste;
			QMakeProjectItem * v = addVariable( liste[1], liste[2], it );
			addValues( explodeValue( liste[3] ), liste[5], v );
			// si il y a un "\" � la fin de la ligne, lire la ligne suivante
			if ( liste[4].endsWith( "\\" ) )
			{
				ligne++;
				// lire la ligne suivante tant que il y a un "\" � la fin de celle si (attention aux commentaires)
				while( varLine.exactMatch( content[ligne] ) )
				{
					// ==================        5         ==================
					liste = varLine.capturedTexts();
					qWarning( "line: %d", ligne );
					qDebug() << liste;
					addValues( explodeValue( liste[1] ), liste[2], v );
					ligne++;
				}
				//
				liste = content[ligne].split( "#" );
				qWarning( "line: %d", ligne );
				qDebug() << liste;
				addValues( explodeValue( liste[0] ), liste.count() > 1 ? liste[1] : QString::null, v );
			}
		}
		// ici les commentaires seuls sur la ligne
		else if ( comments.exactMatch( content[ligne] ) )
			addComment( content[ligne], it );
		// ici les lignes vides
		else if ( content[ligne].trimmed() == "" )
			addEmpty( it );
		// une erreur ? c'est horrible
		else
			qWarning( "Error on line %d: %s", ligne +1, qPrintable( content[ligne] ) );
		ligne++;
	}
	return 1;
}
//
QMakeProjectItem* QMakeProjectParser::processNested( QString& s, QMakeProjectItem* i )
{
	QStringList l;
	int p = 0;
	while ( ( p = explodeNested.indexIn( s, p ) ) != -1 )
	{
		l = explodeNested.capturedTexts();
		i = addScope( l.at( 1 ).trimmed(), l.at( 2 ).trimmed(), QString::null, true, i );
		p += explodeNested.matchedLength();
	}
	if ( !s.isEmpty() )
		s = s.split( ':', QString::SkipEmptyParts ).last();
	return i;
}
//
QMakeProjectItem* QMakeProjectParser::addScope( const QString& v, const QString& o, const QString& c, bool b, QMakeProjectItem* i )
{
	QMakeProjectItem* s = b ? new QMakeProjectItem( AbstractProjectModel::NestedScopeType, i ) : new QMakeProjectItem( AbstractProjectModel::ScopeType, i );
	s->setData( v.trimmed(), AbstractProjectModel::ValueRole );
	s->setData( o.trimmed(), AbstractProjectModel::NestedOperatorRole );
	s->setData( c.trimmed(), AbstractProjectModel::CommentRole );
	(void) new QMakeProjectItem( AbstractProjectModel::ScopeEndType, s );
	return s;
}
//
QMakeProjectItem* QMakeProjectParser::addVariable( const QString& s, const QString& o, QMakeProjectItem* i )
{
	QString n = s;
	QMakeProjectItem* v = new QMakeProjectItem( AbstractProjectModel::VariableType, processNested( n, i ) );
	v->setData( n.trimmed(), AbstractProjectModel::ValueRole );
	v->setData( o.trimmed(), AbstractProjectModel::OperatorRole );
	return v;
}
//
//
QMakeProjectItem* QMakeProjectParser::addFunction( const QString& n, const QString& s, const QString& v, const QString& c, QMakeProjectItem* i )
{
	//qDebug() << n << s << v << c;
	QMakeProjectItem* f = new QMakeProjectItem( AbstractProjectModel::FunctionType, i );
	f->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	f->setData( c.trimmed(), AbstractProjectModel::CommentRole );
	//
	addValues( explodeValue( v ), QString::null, f );
	//
	return f;
}
//
void QMakeProjectParser::addValues( const QStringList& l, const QString& s, QMakeProjectItem* i )
{
	QMakeProjectItem* value = 0;
	foreach ( QString v, l )
	{
		value = new QMakeProjectItem( AbstractProjectModel::ValueType, i );
		value->setData( v.trimmed(), AbstractProjectModel::ValueRole );
	}
	if ( value )
		value->setData( s.trimmed(), AbstractProjectModel::CommentRole );
}
//
void QMakeProjectParser::addComment( const QString& s, QMakeProjectItem* i )
{
	QMakeProjectItem* c = new QMakeProjectItem( AbstractProjectModel::CommentType, i );
	c->setData( s.trimmed(), AbstractProjectModel::ValueRole );
}
//
void QMakeProjectParser::addEmpty( QMakeProjectItem* i )
{
	QMakeProjectItem* e = new QMakeProjectItem( AbstractProjectModel::EmptyType, i );
	e->setData( QString(), AbstractProjectModel::ValueRole );
}
