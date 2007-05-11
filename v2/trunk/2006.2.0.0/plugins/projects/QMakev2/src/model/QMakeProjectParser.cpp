#include "QMakeProjectParser.h"
#include "QMakeProjectItem.h"
//
#include <QFileInfo>
#include <QBuffer>
// pj
#include <QVector>
#include <QRegExp>
#include <QStringList>
// pj
static int prof = 0; // profondeur courrante
static int this_prof = 0; // profondeur pr�c�dent
// regex !!!
static QVector<QString> content;
static QRegExp function_call("^((?:[a-zA-Z]+)\\((.*)\\):)?([a-zA-Z]+)\\((.*)\\)[ \\t]*(#.*)?");
static QRegExp bloc("^(\\})?[ \\t]*((?:[-a-zA-Z0-9*|_!+]+:)*[-a-zA-Z0-9*|_!+]+)[ \\t]*(?:\\((.*)\\))?[ \\t]*(\\{)[ \\t]*(#.*)?");
static QRegExp variable("^((?:[-a-zA-Z0-9*!_|+]+:)*[a-zA-Z0-9*!_]+)[ \\t]*([*+-]?=)[ \\t]*([^\\\\#]*)[ \\t]*(\\\\)?[ \t]*(#.*)?"); 
static QRegExp varLine("^[ \\t]*(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
static QRegExp explodeVars("^[ \\t]*([^ ]+|\"[^\"]+\")(?:[ \\t]*([^ ]+|\"[^\"]+\"))*[ \\t]*");
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
	return QStringList( s );
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
				temp += v;
				inString = false;
				returnValue += temp;
			}
			else
			{
				temp += v;
			}
		}
		else
		{
			if(v.left(1) == "\"")
			{
				temp = v;
				inString = true;
			}
			else
			{
				if(v != "")
					returnValue += v;
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

	/*
	QFile f( s );
	if ( !it || !f.exists() || !f.open( QFile::ReadOnly ) )
		return false;
	QBuffer buf( this );
	buf.setData( f.readAll() );
	if ( !buf.open( QBuffer::ReadOnly | QBuffer::Text ) )
		return false;
	// set project data
	it->setType( AbstractProjectModel::ProjectType );
	it->setData( QFileInfo( s ).completeBaseName() );
	it->setData( s, AbstractProjectModel::AbsoluteFilePathRole );
	//
	fakeProject( it );
	*/
}
// parser file
#include <QDebug>
int QMakeProjectParser::parseBuffer( int ligne, QMakeProjectItem* it )
{
	while(ligne < content.size())
	{
		// c'est une fonction du style :
		// func_name(params)
		if(function_call.exactMatch(content[ligne]))
		{
			QStringList liste = function_call.capturedTexts();
			// ==================        1        ==================
			qWarning() << "ligne : " << ligne << " function found !" << liste[1] << liste[2] << liste[3] << liste[4] << "end function !";
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
			QStringList liste = end_bloc.capturedTexts();
			// ==================        3         ==================
			qWarning() << "ligne : " << ligne << " end scope found !" << liste[1] << liste[2] << "end end scope !";
			
			return ligne;
		}
		// variable (nested compris)
		// "HEADERS = ***" ou "win32:HEADERS = ***" ("+=" et "-=" sont aussi g�r�s)
		else if(variable.exactMatch(content[ligne]))
		{
			QStringList liste = variable.capturedTexts();
			// ==================        4         ==================
			QMakeProjectItem *value = NULL, *variable = new QMakeProjectItem( AbstractProjectModel::VariableType, it );
			variable->setData( liste[1], AbstractProjectModel::ValueRole );
			variable->setData( liste[2], AbstractProjectModel::OperatorRole );
			
			/*
			// récup des différentes valeurs
			QStringList liste2 = explodeValue(liste[3]);
			
			foreach ( QString s, liste2 )
			{
				if ( !s.isEmpty() )
				{
					value = new QMakeProjectItem( AbstractProjectModel::ValueType, variable );
					value->setData( s, AbstractProjectModel::ValueRole );
				}
			}
			if ( liste2.count() )
				value->setData( liste[5], AbstractProjectModel::CommentRole );
			liste2.clear();
			*/

			qWarning() << "ligne : " << ligne << " variable found !" << liste[1] << liste[2] << liste[3] << liste[4] << "end variable !";
			// si il y a un "\" � la fin de la ligne, lire la ligne suivante
			if(liste[4].right(1) == "\\")
			{
				ligne++;
				// lire la ligne suivante tant que il y a un "\" � la fin de celle si (attention aux commentaires)
				while(varLine.exactMatch(content[ligne]))
				{
					liste = varLine.capturedTexts();
					// ==================        5         ==================
					/*
					// récup des différentes valeurs
					liste2 = explodeValue(liste[1]);
					
					foreach ( QString s, liste2 )
					{
						if ( !s.isEmpty() )
						{
							value = new QMakeProjectItem( AbstractProjectModel::ValueType, variable );
							value->setData( s, AbstractProjectModel::ValueRole );
						}
					}
					if ( liste2.count() ) // TODO : faudrait peut être modifier ceci par " if (liste[2] != "") " ? et les autres bien entendu
						value->setData( liste[2], AbstractProjectModel::CommentRole );
					liste2.clear();
					*/
					qWarning() << "ligne : " << ligne << " multiligne found !" << liste[1] << liste[2] << "end multiligne !";
					
					ligne++;
				}
				liste = content[ligne].split( "#" );
				
				//liste2 = explodeValue(liste[1]);
				// TODO : liste2 contient la liste des valeurs, reste à faire le foreach
				
				// ici c'est la derni�re ligne qui ne comporte pas de "\"
				value = new QMakeProjectItem( AbstractProjectModel::ValueType, variable );
				value->setData( liste[0], AbstractProjectModel::ValueRole );
				if ( liste.count() > 1 )
					value->setData( liste[1], AbstractProjectModel::CommentRole );
					
				qWarning() << "ligne : " << ligne << " last multiligne found !" << content[ligne] << "end last multiligne !";
			}
		}
		// ici les commentaires seuls sur la ligne
		else if(comments.exactMatch(content[ligne]))
		{
			QStringList liste = function_call.capturedTexts();
			// ==================        6         ==================
			qWarning() << "ligne : " << ligne << " comment found !" << content[ligne] << "end comment !";
		}
		// ici les lignes vides
		else if(content[ligne] == "")
		{
		}
		// une erreur ? c'est horrible
		else
		{
			qWarning("Error on line %d",(ligne+1));
		}
		ligne++;
	}
	
	return 1;
}
//
void QMakeProjectParser::fakeProject( QMakeProjectItem* it )
{
	//return;
	QMakeProjectItem* p, * i, * s;
	// set app template
	p = new QMakeProjectItem( AbstractProjectModel::VariableType, it );
	p->setData( "TEMPLATE", AbstractProjectModel::ValueRole );
	p->setData( "=", AbstractProjectModel::OperatorRole );
	i = new QMakeProjectItem( AbstractProjectModel::ValueType, p );
	i->setData( "app", AbstractProjectModel::ValueRole );
	// set app version
	p = new QMakeProjectItem( AbstractProjectModel::VariableType, it );
	p->setData( "LANGUAGE", AbstractProjectModel::ValueRole );
	p->setData( "=", AbstractProjectModel::OperatorRole );
	i = new QMakeProjectItem( AbstractProjectModel::ValueType, p );
	i->setData( "Qt4/C++", AbstractProjectModel::ValueRole );
	// header
	p = new QMakeProjectItem( AbstractProjectModel::VariableType, it );
	p->setData( "HEADERS", AbstractProjectModel::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( AbstractProjectModel::ValueType, p );
		i->setData( QString( "pouet%1.h" ).arg( j ), AbstractProjectModel::ValueRole );
	}
	// source
	p = new QMakeProjectItem( AbstractProjectModel::VariableType, it );
	p->setData( "SOURCES", AbstractProjectModel::ValueRole );
	//
	for ( int j = 0; j < 5; j++ )
	{
		i = new QMakeProjectItem( AbstractProjectModel::ValueType, p );
		i->setData( QString( "pouet%1.cpp" ).arg( j ), AbstractProjectModel::ValueRole );
	}
}
