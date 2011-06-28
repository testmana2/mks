#include "QMake2XUP.h"
#include "QMakeProjectItem.h"

#include <MonkeyCore.h>

/**************************
WARNING :
if "operator" does not exists it imply "="
if "multiline" does not exists it imply "false"
if "nested" does not exists it imply "false"
**************************/

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtXml>
#include <QTextCodec>

#include <exception>

class MksException : public std::exception
{
public:
	MksException( QString p_s ) throw()
	{
		s = p_s;
	};
	virtual ~MksException() throw() {};
	virtual const char* what() const throw()
	{
		return s.toLocal8Bit().constData();
	};
private:
	QString s;
};

const QString GENERATED_XUP_VERSION = "1.1.0";

QMakeDocumentFilter::QMakeDocumentFilter()
	: DocumentFilter()
{
	type = Other;
	filtered = false;
}

QMakeDocumentFilter::Map QMake2XUP::qmakeFilters()
{
	static QMakeDocumentFilter::Map filters;
	
	if ( filters.isEmpty() ) {
		const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
		const DocumentFilterMap projectFilters = MonkeyCore::projectTypesIndex()->typeFilters( PLUGIN_NAME );
		QStringList hf; // HEADERS filters
		QStringList sf; // SOURCES filters
		QStringList yf; // YACC filters
		QStringList lf; // LEX filters
		int weight = 0;
		
		foreach ( const QString& f, cf ) {
			if ( f.startsWith( "*.h", Qt::CaseInsensitive ) ) {
				hf << f;
			}
			
			if ( f.startsWith( "*.c", Qt::CaseInsensitive ) ) {
				sf << f;
			}
		}
		
		foreach ( const QString& s, sf ) {
			const QString y = QString( s ).replace( "c", "y", Qt::CaseInsensitive );
			const QString l = QString( s ).replace( "c", "l", Qt::CaseInsensitive );
			
			if ( !yf.contains( y ) ) {
				yf << s;
			}
			
			if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( l ) ) {
				lf << s;
			}
		}
		
		filters[ "FORMS3" ].weight = weight++;
		filters[ "FORMS3" ].label = QMakeProjectItem::tr( "Qt3 Forms" );
		filters[ "FORMS3" ].icon = "forms3.png";
		filters[ "FORMS3" ].filters << "*.ui";
		filters[ "FORMS3" ].type = QMakeDocumentFilter::File;
		filters[ "FORMS3" ].filtered = true;
		
		filters[ "FORMS" ].weight = weight++;
		filters[ "FORMS" ].label = QMakeProjectItem::tr( "Qt Forms" );
		filters[ "FORMS" ].icon = "forms.png";
		filters[ "FORMS" ].filters << "*.ui";
		filters[ "FORMS" ].type = QMakeDocumentFilter::File;
		filters[ "FORMS" ].filtered = true;
		
		filters[ "HEADERS" ].weight = weight++;
		filters[ "HEADERS" ].label = QMakeProjectItem::tr( "Headers" );
		filters[ "HEADERS" ].icon = "headers.png";
		filters[ "HEADERS" ].filters << hf;
		filters[ "HEADERS" ].type = QMakeDocumentFilter::File;
		filters[ "HEADERS" ].filtered = true;
		
		filters[ "SOURCES" ].weight = weight++;
		filters[ "SOURCES" ].label = QMakeProjectItem::tr( "Sources" );
		filters[ "SOURCES" ].icon = "sources.png";
		filters[ "SOURCES" ].filters << sf;
		filters[ "SOURCES" ].type = QMakeDocumentFilter::File;
		filters[ "SOURCES" ].filtered = true;
		
		filters[ "OBJECTIVE_SOURCES" ].weight = weight++;
		filters[ "OBJECTIVE_SOURCES" ].label = QMakeProjectItem::tr( "Objective Sources" );
		filters[ "OBJECTIVE_SOURCES" ].icon = "obj_sources.png";
		filters[ "OBJECTIVE_SOURCES" ].filters << "*.m" << "*.mm";
		filters[ "OBJECTIVE_SOURCES" ].type = QMakeDocumentFilter::File;
		filters[ "OBJECTIVE_SOURCES" ].filtered = true;
		
		filters[ "YACCSOURCES" ].weight = weight++;
		filters[ "YACCSOURCES" ].label = QMakeProjectItem::tr( "YACC Sources" );
		filters[ "YACCSOURCES" ].icon = "yacc_sources.png";
		filters[ "YACCSOURCES" ].filters << yf;
		filters[ "YACCSOURCES" ].type = QMakeDocumentFilter::File;
		filters[ "YACCSOURCES" ].filtered = true;
		
		filters[ "LEXSOURCES" ].weight = weight++;
		filters[ "LEXSOURCES" ].label = QMakeProjectItem::tr( "LEX Sources" );
		filters[ "LEXSOURCES" ].icon = "lex_sources.png";
		filters[ "LEXSOURCES" ].filters << lf;
		filters[ "LEXSOURCES" ].type = QMakeDocumentFilter::File;
		filters[ "LEXSOURCES" ].filtered = true;
		
		filters[ "TRANSLATIONS" ].weight = weight++;
		filters[ "TRANSLATIONS" ].label = QMakeProjectItem::tr( "Qt Translations" );
		filters[ "TRANSLATIONS" ].icon = "translations.png";
		filters[ "TRANSLATIONS" ].filters << "*.ts";
		filters[ "TRANSLATIONS" ].type = QMakeDocumentFilter::File;
		filters[ "TRANSLATIONS" ].filtered = true;
		
		filters[ "RESOURCES" ].weight = weight++;
		filters[ "RESOURCES" ].label = QMakeProjectItem::tr( "Qt Resources" );
		filters[ "RESOURCES" ].icon = "resources.png";
		filters[ "RESOURCES" ].filters << "*.qrc";
		filters[ "RESOURCES" ].type = QMakeDocumentFilter::File;
		filters[ "RESOURCES" ].filtered = true;
		
		filters[ "DEF_FILE" ].weight = weight++;
		filters[ "DEF_FILE" ].label = QMakeProjectItem::tr( "Def. Files" );
		filters[ "DEF_FILE" ].icon = "def_file.png";
		filters[ "DEF_FILE" ].filters << "*.def";
		filters[ "DEF_FILE" ].type = QMakeDocumentFilter::File;
		filters[ "DEF_FILE" ].filtered = true;
		
		filters[ "RC_FILE" ].weight = weight++;
		filters[ "RC_FILE" ].label = QMakeProjectItem::tr( "Rc. Files" );
		filters[ "RC_FILE" ].icon = "rc_file.png";
		filters[ "RC_FILE" ].filters << "*.rc";
		filters[ "RC_FILE" ].type = QMakeDocumentFilter::File;
		filters[ "RC_FILE" ].filtered = true;
		
		filters[ "RES_FILE" ].weight = weight++;
		filters[ "RES_FILE" ].label = QMakeProjectItem::tr( "Res. Files" );
		filters[ "RES_FILE" ].icon = "res_file.png";
		filters[ "RES_FILE" ].filters << "*.res";
		filters[ "RES_FILE" ].type = QMakeDocumentFilter::File;
		filters[ "RES_FILE" ].filtered = true;
		
		filters[ "SUBDIRS" ].weight = weight++;
		filters[ "SUBDIRS" ].label = QMakeProjectItem::tr( "Sub Projects" );
		filters[ "SUBDIRS" ].icon = "project.png";
		filters[ "SUBDIRS" ].filters
			<< projectFilters.value( "QMAKE_PROJECT" ).filters
			<< projectFilters.value( "QMAKE_INCLUDE_PROJECT" ).filters
			;
		filters[ "SUBDIRS" ].type = QMakeDocumentFilter::File;
		filters[ "SUBDIRS" ].filtered = false;
		
		filters[ "OTHER_FILES" ].weight = weight++;
		filters[ "OTHER_FILES" ].label = QMakeProjectItem::tr( "Other Files" );
		filters[ "OTHER_FILES" ].icon = "file.png";
		filters[ "OTHER_FILES" ].filters << "*.txt" << "*README*" << "*Change*";
		filters[ "OTHER_FILES" ].type = QMakeDocumentFilter::File;
		filters[ "OTHER_FILES" ].filtered = true;
		
		//
		filters[ "INCLUDEPATH" ].weight = weight++;
		filters[ "INCLUDEPATH" ].label = QMakeProjectItem::tr( "Includes Paths" );
		filters[ "INCLUDEPATH" ].icon = "includepath.png";
		filters[ "INCLUDEPATH" ].filters.clear();
		filters[ "INCLUDEPATH" ].type = QMakeDocumentFilter::Path;
		filters[ "INCLUDEPATH" ].filtered = true;
		
		filters[ "DEPENDPATH" ].weight = weight++;
		filters[ "DEPENDPATH" ].label = QMakeProjectItem::tr( "Depends Paths" );
		filters[ "DEPENDPATH" ].icon = "dependpath.png";
		filters[ "DEPENDPATH" ].filters.clear();
		filters[ "DEPENDPATH" ].type = QMakeDocumentFilter::Path;
		filters[ "DEPENDPATH" ].filtered = true;
		
		filters[ "VPATH" ].weight = weight++;
		filters[ "VPATH" ].label = QMakeProjectItem::tr( "Virtuals Paths" );
		filters[ "VPATH" ].icon = "vpath.png";
		filters[ "VPATH" ].filters.clear();
		filters[ "VPATH" ].type = QMakeDocumentFilter::Path;
		filters[ "VPATH" ].filtered = true;
		
		//
		filters[ "LIBS" ].weight = weight++;
		filters[ "LIBS" ].label = QMakeProjectItem::tr( "Libraries Files" );
		filters[ "LIBS" ].icon = "libs.png";
		filters[ "LIBS" ].filters.clear();
		filters[ "LIBS" ].type = QMakeDocumentFilter::Other;
		filters[ "LIBS" ].filtered = true;
		
		filters[ "DEFINES" ].weight = weight++;
		filters[ "DEFINES" ].label = QMakeProjectItem::tr( "Defines" );
		filters[ "DEFINES" ].icon = "defines.png";
		filters[ "DEFINES" ].filters.clear();
		filters[ "DEFINES" ].type = QMakeDocumentFilter::Other;
		filters[ "DEFINES" ].filtered = true;
	}
	
	return filters;
}

QString QMake2XUP::convertFromPro( const QString& s, const QString& codec )
{
	// check if file exists
	if ( !QFile::exists( s ) )
		return QString();
	
	QFile f( s );
	if( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return QString();
	QString data = QTextCodec::codecForName( codec.toUtf8() )->toUnicode( f.readAll() );
	
	QVector<QString> temp_v = data.split( '\n' ).toVector();
	QVector<QString> v;
	foreach(QString b,temp_v)
	{// used to trim the vector's data
		v.append(b.trimmed());
	}
	QStack<bool> isNested;
	QStack<QString> pile;
	QString file;
	QString inVarComment;
	int nbEmptyLine = 0;
	
	QRegExp Variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([*+-]?=)[ \\t]*((?:\\\\'|\\\\\\$|\\\\\\\\\\\\\\\"|\\\\\\\"|[^#])+)?[ \\t]*(#.*)?");
	QRegExp RegexVar("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*~=([^#]+)(#.*)?");
	//QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[ \\t]*[:|][ \\t]*)+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[ \\t]*(\\{)[ \\t]*(#.*)?");
	QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([-\\.a-zA-Z0-9*|_!+]+(?:\\((?:.*)\\))?))[:]*[ \\t]*(\\{)[ \\t]*(#.*)?");
	QRegExp function_call("^((?:!?[-\\.a-zA-Z0-9*!_|+]+(?:[ \\t]*\\((?:.*)\\))?[ \\t]*[|:][ \\t]*)+)?([a-zA-Z]+[ \\t]*\\((.*)\\))[ \\t]*(#.*)?");
	QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
	QRegExp end_bloc_continuing("^(\\})[ \\t]*(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*((?:\\\\'|\\\\\\$|\\\\\\\\\\\\\\\"|\\\\\\\"|[^\\\\#])+)?[ \\t]*(\\\\)?[ \t]*(#.*)?");
	QRegExp comments("^#(.*)");
	QRegExp varLine("^(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
	
	file.append( QString( "<!DOCTYPE XUPProject>\n<project name=\"%1\" version=\"%2\" expanded=\"false\">\n" ).arg( QFileInfo( s ).fileName() ).arg( GENERATED_XUP_VERSION ) );
	try
	{
		for(int i = 0;i < v.size();i++)
		{
			if(bloc.exactMatch(v[i]))
			{
				QString tmp_end;
				QStringList liste = bloc.capturedTexts();
				if(liste[1] == "}")
				{
					while(!isNested.isEmpty() && isNested.top())
					{
						file.append(pile.pop());
						isNested.pop();
					}
					// pop the last scope of the bloc
					file.append(pile.pop());
					isNested.pop();
					// pop the nested scope of the bloc
					while(!isNested.isEmpty() && isNested.top())
					{
						file.append(pile.pop());
						isNested.pop();
					}
				}
				if(liste[0].trimmed()[0] == '}')
					liste[0] = liste[0].trimmed().right(liste[0].trimmed().length()-2);
				liste[0] = liste[0].left(liste[0].indexOf(QChar('{'))+1);
				QStringList liste2 = liste[0].split(QChar(':'),QString::SkipEmptyParts);
				
				foreach(QString s,liste2)
				{
					if(s[s.length()-1] == '{')
					{
						file.append("<scope name=\""+escape(s.left(s.length()-1).trimmed())+"\""+(liste[5].trimmed() != "" ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">\n");
					}
					else
					{
						file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
						tmp_end += "</scope>\n";
					}
				}
				if(tmp_end != "")
				{
					pile += tmp_end;
					isNested.push(true);
				}
				pile += "</scope>\n";
				isNested.push(false);
			}
			else if(RegexVar.exactMatch(v[i]))
			{
				QStringList liste = RegexVar.capturedTexts();
				// liste[1] = scopes
				// liste[2] = la variable
				// liste[3] = la ligne (ne pas oublier trimmed())
				// liste[4] = le commentaire
				file.append("<comment value=\"#"+escape(liste[2])+"\" />\n");
			}
			else if(Variable.exactMatch(v[i]))
			{
				QString tmp_end;
				QStringList liste = Variable.capturedTexts();
				QStringList liste2 = liste[1].trimmed().split(QChar(':'), QString::SkipEmptyParts);
				if(liste[1] == "else" || (liste2.size() > 0 && liste2[0] == "else"))
				{
					if(isNested.size())
					{
						// pop the last scope of the bloc
						file.append(pile.pop());
						isNested.pop();
						// pop the nested scope of the bloc
						while(!isNested.isEmpty() && isNested.top())
						{
							file.append(pile.pop());
							isNested.pop();
						}
					}
				}
				else
				{
					while(!isNested.isEmpty() && isNested.top())
					{
						file.append(pile.pop());
						isNested.pop();
					}
				}
				if(liste2.size() > 0)
				{
					foreach(QString s,liste2)
					{
						file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
						pile += "</scope>\n";
						isNested.push(true);
					}
				}
				
				QString isMulti;
				if(liste[4].trimmed().endsWith("\\") || liste[4].trimmed() == "\\")
				{
					isMulti = " multiline=\"true\"";
					QString tmppp = liste[4].trimmed();
					tmppp.chop(1);
					liste[4] = tmppp;
				}
				QString theOp = (liste[3].trimmed() == "=" ? "" : " operator=\""+liste[3].trimmed()+"\"");
				file.append("<variable name=\""+escape(liste[2].trimmed())+"\""+theOp+isMulti+">\n");
				bool isFile = QMakeProjectItem::fileVariables().contains(liste[2].trimmed());
				bool isPath = QMakeProjectItem::pathVariables().contains(liste[2].trimmed());
				if ( isFile || isPath )
				{
					QStringList tmpValues = liste[4].trimmed().split(" ");
					bool inStr = false;
					QStringList multivalues;
					QString ajout;
					for(int ku = 0;ku < tmpValues.size();ku++)
					{
						if(tmpValues.value(ku).startsWith('"') )
							inStr = true;
						if(inStr)
						{
							if(ajout != "")
								ajout += " ";
							ajout += tmpValues.value(ku);
							if(tmpValues.value(ku).endsWith('"') )
							{
								multivalues += ajout;
								ajout = "";
								inStr = false;
							}
						}
						else
						{
							multivalues += tmpValues.value(ku);
						}
					}
					for(int ku = 0;ku < multivalues.size();ku++)
					{
						inVarComment = multivalues.value(ku).trimmed();
						if ( inVarComment.startsWith( "#" ) )
						{
							if ( inVarComment == "#" && ku < multivalues.size() )
							{
								ku++;
								inVarComment = "# " +multivalues.value(ku).trimmed();
							}
							file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
						}
						else
						{
							if ( isFile )
							{
								file.append("<file"+(liste[5].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
							}
							else if ( isPath )
							{
								file.append("<path"+(liste[5].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
							}
						}
					}
				}
				else
					file.append("<value"+(liste[5].trimmed() != "" ? " comment=\""+escape(liste[5].trimmed())+"\"" : "")+">"+escape(liste[4].trimmed())+"</value>\n");
				if(isMulti == " multiline=\"true\"")
				{
					i++;
					while(varLine.exactMatch(v[i]))
					{
						QStringList liste3 = varLine.capturedTexts();
						bool isFile = QMakeProjectItem::fileVariables().contains(liste[2].trimmed());
						bool isPath = QMakeProjectItem::pathVariables().contains(liste[2].trimmed());
						if ( isFile || isPath )
						{
							QStringList tmpValues = liste3[1].trimmed().split(" ");
							QStringList multivalues = QStringList();
							QString ajout = "";
							bool inStr = false;
							for(int ku = 0;ku < tmpValues.size();ku++)
							{
								if(tmpValues.value(ku).startsWith('"') )
									inStr = true;
								if(inStr)
								{
									if(ajout != "")
										ajout += " ";
									ajout += tmpValues.value(ku);
									if(tmpValues.value(ku).endsWith('"') )
									{
										multivalues += ajout;
										ajout = "";
										inStr = false;
									}
								}
								else
								{
									multivalues += tmpValues.value(ku);
								}
							}
							for(int ku = 0;ku < multivalues.size();ku++)
							{
								inVarComment = multivalues.value(ku).trimmed();
								if ( inVarComment.startsWith( "#" ) )
								{
									if ( inVarComment == "#" && ku < multivalues.size() )
									{
										ku++;
										inVarComment = "# " +multivalues.value(ku).trimmed();
									}
									file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
								}
								else
								{
									if ( isFile )
									{
										file.append("<file"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
									}
									else if ( isPath )
									{
										file.append("<path"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
									}
								}
							}
						}
						else
							file.append("<value"+(liste3[2].trimmed() != "" ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(liste3[1].trimmed())+"</value>\n");
						i++;
					}
					QStringList liste3 = v[i].split( "#" );
					QString comment;
					if(liste3.size() == 2)
						comment = "#"+liste3[1];
					bool isFile = QMakeProjectItem::fileVariables().contains(liste[2].trimmed());
					bool isPath = QMakeProjectItem::pathVariables().contains(liste[2].trimmed());
					if ( isFile || isPath )
					{
						QStringList tmpValues = liste3[0].trimmed().split(" ");
						QStringList multivalues = QStringList();
						QString ajout = "";
						bool inStr = false;
						for(int ku = 0;ku < tmpValues.size();ku++)
						{
							if(tmpValues.value(ku).startsWith('"') )
								inStr = true;
							if(inStr)
							{
								if(ajout != "")
									ajout += " ";
								ajout += tmpValues.value(ku);
								if(tmpValues.value(ku).endsWith('"') )
								{
									multivalues += ajout;
									ajout = "";
									inStr = false;
								}
							}
							else
							{
								multivalues += tmpValues.value(ku);
							}
						}
						for(int ku = 0;ku < multivalues.size();ku++)
						{
							inVarComment = multivalues.value(ku).trimmed();
							if ( inVarComment.startsWith( "#" ) )
							{
								if ( inVarComment == "#" && ku < multivalues.size() )
								{
									ku++;
									inVarComment = "# " +escape(multivalues.value(ku).trimmed());
								}
								file.append( QString( "<comment content=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
							}
							else
							{
								if ( isFile )
								{
									file.append("<file"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
								}
								else if ( isPath )
								{
									file.append("<path"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
								}
							}
						}
					}
					else
						file.append("<value"+(comment.trimmed() != "" ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(liste3[0].trimmed())+"</value>\n");
				}
				file.append("</variable>\n");
			}
			else if(function_call.exactMatch(v[i]))
			{
				QString tmp_end;
				QStringList liste = function_call.capturedTexts();
				QStringList liste2 = liste[1].split(QChar(':'),QString::SkipEmptyParts);
				foreach(QString s,liste2)
				{
					file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
					tmp_end += "</scope>\n";
				}
				QStringList explode_params = liste[2].split("(");
				QString func_name = explode_params[0].trimmed();
				QString params;
				if(explode_params.size() > 2)
					params = explode_params.join("(");
				else
					params = explode_params[1];
				params = params.trimmed(); // to be sure that the last char is the last ")"
				params.chop(1); // pop the last ")"
				params = params.trimmed(); // to pop off the ending spaces
				file.append("<function"+(liste[4].trimmed() != "" ? " comment=\""+escape(liste[4].trimmed())+"\"" : "")+" name=\""+escape(func_name)+"\" parameters=\""+escape(params)+"\" />\n");
				file.append(tmp_end);
			}
			else if(end_bloc_continuing.exactMatch(v[i]))
			{
				QStringList liste = end_bloc_continuing.capturedTexts();
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop());
					isNested.pop();
				}
				// pop the last scope of the bloc
				file.append(pile.pop());
				isNested.pop();
				// pop the nested scope of the bloc
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop());
					isNested.pop();
				}
				
				QStringList liste2 = liste[2].split(QChar(':'),QString::SkipEmptyParts);
				foreach(QString s,liste2)
				{
					file.append("<scope name=\""+escape(s.trimmed())+"\" nested=\"true\">\n");
					pile += "</scope>\n";
					isNested.push(true);
				}
				QString isMulti = (liste[6].trimmed() == "\\" ? " multiline=\"true\"" : "");
				QString theOp = (liste[4].trimmed() == "=" ? "" : " operator=\""+liste[4].trimmed()+"\"");
				file.append("<variable name=\""+escape(liste[3].trimmed())+"\""+theOp+">\n");
				if ( liste[7].trimmed().startsWith( "#" ) )
					file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( liste[7].trimmed() ) ) );
				else
					file.append("<value"+(liste[7].trimmed() != "" ? " comment=\""+escape(liste[7].trimmed())+"\"" : "")+">"+escape(liste[5].trimmed())+"</value>\n");
				if(isMulti == " multiline=\"true\"")
				{
					i++;
					while(varLine.exactMatch(v[i]))
					{
						QStringList liste3 = varLine.capturedTexts();
						bool isFile = QMakeProjectItem::fileVariables().contains(liste[2].trimmed());
						bool isPath = QMakeProjectItem::pathVariables().contains(liste[2].trimmed());
						if ( isFile || isPath )
						{
							QStringList tmpValues = liste3[1].trimmed().split(" ");
							QStringList multivalues = QStringList();
							QString ajout = "";
							bool inStr = false;
							for(int ku = 0;ku < tmpValues.size();ku++)
							{
								if(tmpValues.value(ku).startsWith('"') )
									inStr = true;
								if(inStr)
								{
									if(ajout != "")
										ajout += " ";
									ajout += tmpValues.value(ku);
									if(tmpValues.value(ku).endsWith('"') )
									{
										multivalues += ajout;
										ajout = "";
										inStr = false;
									}
								}
								else
								{
									multivalues += tmpValues.value(ku);
								}
							}
							for(int ku = 0;ku < multivalues.size();ku++)
							{
								inVarComment = multivalues.value(ku).trimmed();
								if ( inVarComment.startsWith( "#" ) )
								{
									if ( inVarComment == "#" && ku < multivalues.size() )
									{
										ku++;
										inVarComment = "# " +multivalues.value(ku).trimmed();
									}
									file.append( QString( "<comment value=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
								}
								else
								{
									if ( isFile )
									{
										file.append("<file"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
									}
									else if ( isPath )
									{
										file.append("<path"+(liste3[2].trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
									}
								}
							}
						}
						else
							file.append("<value"+(liste3[2].trimmed() != "" ? " comment=\""+escape(liste3[2].trimmed())+"\"" : "")+">"+escape(liste3[1].trimmed())+"</value>\n");
						i++;
					}
					QStringList liste3 = v[i].split( "#" );
					QString comment;
					if(liste3.size() == 2)
						comment = "#"+liste3[1];
					bool isFile = QMakeProjectItem::fileVariables().contains(liste[2].trimmed());
					bool isPath = QMakeProjectItem::pathVariables().contains(liste[2].trimmed());
					if ( isFile || isPath )
					{
						QStringList tmpValues = liste3[0].trimmed().split(" ");
						QStringList multivalues = QStringList();
						QString ajout = "";
						bool inStr = false;
						for(int ku = 0;ku < tmpValues.size();ku++)
						{
							if(tmpValues.value(ku).startsWith('"') )
								inStr = true;
							if(inStr)
							{
								if(ajout != "")
									ajout += " ";
								ajout += tmpValues.value(ku);
								if(tmpValues.value(ku).endsWith('"') )
								{
									multivalues += ajout;
									ajout = "";
									inStr = false;
								}
							}
							else
							{
								multivalues += tmpValues.value(ku);
							}
						}
						for(int ku = 0;ku < multivalues.size();ku++)
						{
							inVarComment = multivalues.value(ku).trimmed();
							if ( inVarComment.startsWith( "#" ) )
							{
								if ( inVarComment == "#" && ku < multivalues.size() )
								{
									ku++;
									inVarComment = "# " +escape(multivalues.value(ku).trimmed());
								}
								file.append( QString( "<comment content=\"%1\" />\n" ).arg( QString( inVarComment ) ) );
							}
							else
							{
								if ( isFile )
								{
									file.append("<file"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</file>\n");
								}
								else if ( isPath )
								{
									file.append("<path"+(comment.trimmed() != "" && ku+1 == multivalues.size() ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(multivalues.value(ku)).remove( '"' )+"</path>\n");
								}
							}
						}
					}
					else
						file.append("<value"+(comment.trimmed() != "" ? " comment=\""+escape(comment.trimmed())+"\"" : "")+">"+escape(liste3[0].trimmed())+"</value>\n");
				}
				file.append("</variable>\n");
			}
			else if(end_bloc.exactMatch(v[i]))
			{
				while(!isNested.isEmpty() && isNested.top())
				{
					isNested.top();
					file.append(pile.pop());
					isNested.pop();
				}
				isNested.top();
				file.append(pile.pop());
				isNested.pop();
				if(!isNested.isEmpty() && isNested.top())
				{
					isNested.top();
					file.append(pile.pop());
					isNested.pop();
				}
			}
			else if(comments.exactMatch(v[i]))
			{
				QStringList liste = comments.capturedTexts();
				file.append("<comment value=\"#"+escape(liste[1])+"\" />\n");
			}
			else if(v[i] == "")
			{
				nbEmptyLine++;
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop());
					isNested.pop();
				}
				if(i+1 < v.size())
				{
					if(v[i+1] != "")
					{
						file.append("<emptyline count=\""+QString::number(nbEmptyLine)+"\" />\n");
						nbEmptyLine = 0;
					}
				}
				else
					file.append("<emptyline count=\""+QString::number(nbEmptyLine)+"\" />\n");
			}
			else
			{
				qWarning( "%s didn't match", v[i].toLocal8Bit().constData());
				throw MksException( QString("Erreur parsing project file: %1").arg( s ) );
			}
		}
		while(!isNested.isEmpty() && isNested.top())
		{
			file.append(pile.pop());
			isNested.pop();
		}
	}
	catch(const std::exception & e)
	{
		// re-init the XML output
		file.append( QString( "<!DOCTYPE XUPProject>\n<project name=\"%1\" version=\"%2\" expanded=\"false\">\n" ).arg( QFileInfo( s ).fileName() ).arg( GENERATED_XUP_VERSION ) );
		// empty both stacks
		isNested.clear();
		pile.clear();
		qWarning("%s", e.what() );
	}
	
	file.append( "</project>\n" );
	// to output the xml in a file
	/*QFile apt(s+".xml");
	if( apt.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		apt.write(file.toAscii());
		apt.close();
	}*/
	
	return file;
}

QString QMake2XUP::convertToPro( const QDomDocument& document )
{
	// get project node
	QDomElement element  = document.firstChildElement( "project" ).toElement();
	
	// check project available
	if ( element.isNull() ) {
		return QString::null;
	}
	
	// parse project scope
	QString contents = convertNodeToPro( element );
	
	// remove last eol
	if ( contents.length() > 0 ) {
		contents.chop( 1 );
	}
	
	// return buffer
	return contents;
}

QString QMake2XUP::escape( const QString& string )
{
	return Qt::escape( string ).replace( "\"" , "&quot;" );
}

QString QMake2XUP::convertNodeToPro( const QDomNode& node, int weight, bool multiline, bool nested, const QString& EOL )
{
	const QStringList notContainers = QStringList()
		<< "comment"
		<< "emptyline"
		<< "value"
		<< "file"
		<< "path"
		;
	QString data; // the data to return
	
	if ( !isProject( node ) ) {
		if ( isComment( node ) ) {
			const QString cmt = nodeAttribute( node, "value" );
			int cweight = weight;
			
			if ( multiline ) {
				cweight++;
			}
			
			data.append( tabbedString( cweight, cmt, EOL ) );
		}
		else if ( isEmptyLine( node ) ) {
			const int count = node.attributes().namedItem( "count" ).nodeValue().toInt();
			for ( int i = 0; i < count; i++ ) {
				data.append( EOL );
			}
		}
		else if ( isVariable( node ) ) {
			const QString variable = QString( "%1\t%2 " )
				.arg( nodeAttribute( node, "name" ) )
				.arg( nodeAttribute( node, "operator", "=" ) )
				;
			multiline = isMultiline( node );
			int vweight = weight;
			
			if ( isBlock( node.parentNode() ) && nested ) {
				vweight = 0;
			}
			
			data.append( tabbedString( vweight, variable ) );
		}
		else if ( isValue( node ) ) {
			const QString value = node.toElement().text();
			const QString comment = nodeAttribute( node, "comment" );
			int vweight = weight;
			
			if ( node.previousSibling().isNull() || !multiline ) {
				vweight = 0;
			}
			else if ( multiline ) {
				vweight++;
			}
			
			data.append( tabbedString( vweight, value ) );
			
			if ( multiline ) {
				if ( !isLastValue( node ) ) {
					data.append( " \\" );
				}
			}
			
			if ( multiline || node.nextSibling().isNull() ) {
				if ( !comment.isEmpty() ) {
					data.append( ' ' +comment );
				}
				
				data.append( EOL );
			}
			else {
				data.append( ' ' );
			}
		}
		else if ( isBlock( node ) ) {
			const QString content = node.nodeName().compare( "function", Qt::CaseInsensitive ) == 0
				? QString( "%1( %2 )" ).arg( nodeAttribute( node, "name" ) ).arg( nodeAttribute( node, "parameters" ) )
				: nodeAttribute( node, "name" );
			const QString comment = nodeAttribute( node, "comment" );
			int bweight = weight;
			
			if ( isBlock( node.parentNode() ) && nested ) {
				bweight = 0;
			}
			
			nested = isNested( node );
			
			data.append( tabbedString( bweight, content ) );
			
			if ( nested ) {
				data.append( ':' );
			}
			else {
				if ( node.hasChildNodes() ) {
					data.append( " {" );
					weight++;
				}
				
				if ( !comment.isEmpty() ) {
					data.append( ' ' +comment );
				}
				
				data.append( EOL );
			}
		}
	}
	
	if ( node.hasChildNodes() && !notContainers.contains( node.nodeName() ) ) {
		QDomNodeList nodes = node.childNodes();
		
		for ( int i = 0; i < nodes.count(); i++ ) {
			data.append( convertNodeToPro( nodes.at( i ), weight, multiline, nested, EOL ) );
		}
	}
	
	if ( isBlock( node ) && !nested ) {
		const QString comment = nodeAttribute( node, "closing-comment" );
		const QDomNode sibling = node.nextSibling();
		
		if ( node.hasChildNodes() ) {
			weight--;
			data.append( tabbedString( weight, "}" ) );
		}
		
		if ( !( isBlock( sibling ) && ( nodeAttribute( sibling, "name" ).compare( "else", Qt::CaseInsensitive ) == 0 ) ) ) {
			if ( node.hasChildNodes() ) {
				if ( !comment.isEmpty() ) {
					data.append( ' ' +comment );
				}
				
				data.append( EOL );
			}
		}
		else {
			data.append( ' ' );
		}
	}

	return data;
}

QString QMake2XUP::tabbedString( int weight, const QString& string, const QString& eol )
{
	return QString( weight, '\t' ).append( string ).append( eol );
}

QString QMake2XUP::nodeAttribute( const QDomNode& node, const QString& attribute, const QString& defaultValue )
{
	QString value = node.attributes().namedItem( attribute ).nodeValue();
	
	if ( value.isEmpty() ) {
		value = defaultValue;
	}
	
	return value;
}

bool QMake2XUP::isMultiline( const QDomNode& node )
{
	QString string = node.attributes().namedItem( "multiline" ).nodeValue();
	
	if ( string.isEmpty() ) {
		string = "false";
	}
	
	return QVariant( string ).toBool();
}

bool QMake2XUP::isNested( const QDomNode& node )
{
	QString string = node.attributes().namedItem( "nested" ).nodeValue();
	
	if ( string.isEmpty() ) {
		string = "false";
	}
	
	bool nested = QVariant( string ).toBool();
	
	if ( nested && node.childNodes().count() > 1 ) {
		nested = false;
	}
	
	return nested;
}

bool QMake2XUP::isBlock( const QDomNode& node )
{
	const QString name = node.nodeName();
	
	return name.compare( "function", Qt::CaseInsensitive ) == 0
		|| name.compare( "scope", Qt::CaseInsensitive ) == 0
		;
}

bool QMake2XUP::isProject( const QDomNode& node )
{
	return node.nodeName().compare( "project", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isComment( const QDomNode& node )
{
	return node.nodeName().compare( "comment", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isEmptyLine( const QDomNode& node )
{
	return node.nodeName().compare( "emptyline", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isVariable( const QDomNode& node )
{
	return node.nodeName().compare( "variable", Qt::CaseInsensitive ) == 0;
}

bool QMake2XUP::isValue( const QDomNode& node )
{
	const QString name = node.nodeName();
	
	return name.compare( "value", Qt::CaseInsensitive ) == 0
		|| name.compare( "file", Qt::CaseInsensitive ) == 0
		|| name.compare( "path", Qt::CaseInsensitive ) == 0
		;
}

bool QMake2XUP::isLastValue( const QDomNode& node )
{
	QDomNode sibling = node;
	
	while ( !( sibling = sibling.nextSibling() ).isNull() ) {
		if ( isValue( sibling ) ) {
			return false;
		}
	}
	
	return true;
}
