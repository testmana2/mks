#include "QMake2XUP.h"
#include "XUPManager.h"

/**************************
WARNING :
si "operator" n'existe pas, il vaux "="
si "multiline" n'existe pas, il vaux "false"
si "nested" n'existe pas, il vaux "false"
**************************/

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtXml>

QString tabsString( int i )
{ return QString().fill( '\t', i ); }

QByteArray QMake2XUP::convertFromPro( const QString& s, const QString& version )
{
	// check if file exists
	if ( !QFile::exists( s ) )
		return QByteArray();
	
	QFile f( s );
	if( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return QByteArray();
	QByteArray data = f.readAll();
	
	QVector<QByteArray> temp_v = data.split( '\n' ).toVector();
	QVector<QString> v;
	foreach(QByteArray b,temp_v)
	{// used to trim the vector's data
		v.append(QString(b).trimmed());
	}
	QStack<bool> isNested;
	QStack<QString> pile;
	QString file;
	QString inVarComment;
	int nbEmptyLine = 0;
	
	QRegExp Variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*((?:\\\\\\\\\\\\\\\"|\\\\\\\"|[^\\\\#])+)?[ \\t]*(\\\\)?[ \t]*(#.*)?");
	//QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[ \\t]*[:|][ \\t]*)+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[ \\t]*(\\{)[ \\t]*(#.*)?");
	QRegExp bloc("^(\\})?[ \\t]*((?:(?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[ \\t]*[:|][ \\t]*)+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[:]*[ \\t]*(\\{)[ \\t]*(#.*)?");
	QRegExp function_call("^((?:[a-zA-Z0-9\\.]+(?:\\((?:.*)\\))?[ \\t]*[|:][ \\t]*)+)?([a-zA-Z]+\\((.*)\\))[ \\t]*(#.*)?");
	QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
	QRegExp end_bloc_continuing("^(\\})[ \\t]*(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?[ \\t]*[:|][ \\t]*)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*((?:\\\\\\\\\\\\\\\"|\\\\\\\"|[^\\\\#])+)?[ \\t]*(\\\\)?[ \t]*(#.*)?");
	QRegExp comments("^#(.*)");
	QRegExp varLine("^(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
	
	file.append( QString( "<!DOCTYPE XUPProject>\n<project version=\"%1\" name=\"%2\" expanded=\"false\">\n" ).arg( version ).arg( QFileInfo( s ).fileName() ) );
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
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
				// pop the last scope of the bloc
				file.append(pile.pop().toUtf8());
				isNested.pop();
				// pop the nested scope of the bloc
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
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
					file.append("<scope name=\""+s.left(s.length()-1).trimmed().toUtf8()+"\""+(liste[5].trimmed().toUtf8() != "" ? " comment=\""+liste[5].trimmed().toUtf8()+"\"" : "")+">\n");
				}
				else
				{
					file.append("<scope name=\""+s.trimmed().toUtf8()+"\" nested=\"true\">\n");
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
		else if(Variable.exactMatch(v[i]))
		{
			QString tmp_end;
			QStringList liste = Variable.capturedTexts();
			QStringList liste2 = liste[1].trimmed().split(QChar(':'),QString::SkipEmptyParts);
			if(liste[1] == "else" || (liste2.size() > 0 && liste2[0] == "else"))
			{
				// pop the last scope of the bloc
				file.append(pile.pop().toUtf8());
				isNested.pop();
				// pop the nested scope of the bloc
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
			}
			else
			{
				while(!isNested.isEmpty() && isNested.top())
				{
					file.append(pile.pop().toUtf8());
					isNested.pop();
				}
			}
			if(liste2.size() > 0)
			{
				foreach(QString s,liste2)
				{
					file.append("<scope name=\""+s.trimmed().toUtf8()+"\" nested=\"true\">\n");
					pile += "</scope>\n";
					isNested.push(true);
				}
			}
			QByteArray isMulti = (liste[5].trimmed() == "\\" ? " multiline=\"true\"" : "");
			QByteArray theOp = (liste[3].trimmed().toUtf8() == "=" ? "" : " operator=\""+liste[3].trimmed().toUtf8()+"\"");
			file.append("<variable name=\""+liste[2].trimmed().toUtf8()+"\""+theOp+isMulti+">\n");
			
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
				inVarComment = multivalues.value(ku).toUtf8().trimmed();
				if ( inVarComment.startsWith( "#" ) )
				{
					if ( inVarComment == "#" && ku < multivalues.size() )
					{
						ku++;
						inVarComment = "# " +multivalues.value(ku).trimmed();
					}
					file.append( QString( "<comment>%1 \\</comment>" ).arg( QString( inVarComment ) ) );
				}
				else
					file.append("<value"+(liste[6].trimmed().toUtf8() != "" && ku+1 == multivalues.size() ? " comment=\""+liste[6].trimmed().toUtf8()+"\"" : "")+">"+multivalues.value(ku).toUtf8()+"</value>\n");
			}
			
			if(isMulti == " multiline=\"true\"")
			{
				i++;
				while(varLine.exactMatch(v[i]))
				{
					liste = varLine.capturedTexts();
					tmpValues = liste[1].trimmed().split(" ");
					multivalues = QStringList();
					ajout = "";
					inStr = false;
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
						inVarComment = multivalues.value(ku).toUtf8().trimmed();
						if ( inVarComment.startsWith( "#" ) )
						{
							if ( inVarComment == "#" && ku < multivalues.size() )
							{
								ku++;
								inVarComment = "# " +multivalues.value(ku).trimmed();
							}
							file.append( QString( "<comment>%1 \\</comment>" ).arg( QString( inVarComment ) ) );
						}
						else
							file.append("<value"+(liste[2].trimmed().toUtf8() != "" && ku+1 == multivalues.size() ? " comment=\""+liste[2].trimmed().toUtf8()+"\"" : "")+">"+multivalues.value(ku).toUtf8()+"</value>\n");
					}
					i++;
				}
				liste = v[i].split( "#" );
				QString comment;
				if(liste.size() == 2)
					comment = "#"+liste[1];
				tmpValues = liste[0].trimmed().split(" ");
				multivalues = QStringList();
				ajout = "";
				inStr = false;
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
					inVarComment = multivalues.value(ku).toUtf8().trimmed();
					if ( inVarComment.startsWith( "#" ) )
					{
						if ( inVarComment == "#" && ku < multivalues.size() )
						{
							ku++;
							inVarComment = "# " +multivalues.value(ku).trimmed();
						}
						file.append( QString( "<comment>%1 \\</comment>" ).arg( QString( inVarComment ) ) );
					}
					else
						file.append("<value"+(comment.trimmed().toUtf8() != "" && ku+1 == multivalues.size() ? " comment=\""+comment.trimmed().toUtf8()+"\"" : "")+">"+multivalues.value(ku).toUtf8()+"</value>\n");
				}
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
				file.append("<scope name=\""+s.trimmed().toUtf8()+"\" nested=\"true\">\n");
				tmp_end += "</scope>\n";
			}
			file.append("<function"+(liste[4].trimmed() != "" ? " comment=\""+liste[4].trimmed().toUtf8()+"\"" : "")+">"+liste[2].trimmed().toUtf8()+"</function>\n");
			file.append(tmp_end.toUtf8());
		}
		else if(end_bloc_continuing.exactMatch(v[i]))
		{
			QStringList liste = end_bloc_continuing.capturedTexts();
			while(!isNested.isEmpty() && isNested.top())
			{
				file.append(pile.pop().toUtf8());
				isNested.pop();
			}
			// pop the last scope of the bloc
			file.append(pile.pop().toUtf8());
			isNested.pop();
			// pop the nested scope of the bloc
			while(!isNested.isEmpty() && isNested.top())
			{
				file.append(pile.pop().toUtf8());
				isNested.pop();
			}
			
			QStringList liste2 = liste[2].split(QChar(':'),QString::SkipEmptyParts);
			foreach(QString s,liste2)
			{
				file.append("<scope name=\""+s.trimmed().toUtf8()+"\" nested=\"true\">\n");
				pile += "</scope>\n";
				isNested.push(true);
			}
			QByteArray theOp = (liste[4].trimmed().toUtf8() == "=" ? "" : " operator=\""+liste[4].trimmed().toUtf8()+"\"");
			file.append("<variable name=\""+liste[3].trimmed().toUtf8()+"\""+theOp+">\n");
			if ( liste[7].trimmed().toUtf8().startsWith( "#" ) )
				file.append( QString( "<comment>%1</comment>" ).arg( QString( liste[7].trimmed().toUtf8() ) ) );
			else
				file.append("<value"+(liste[7].trimmed().toUtf8() != "" ? " comment=\""+liste[7].trimmed().toUtf8()+"\"" : "")+">"+liste[5].trimmed().toUtf8()+"</value>\n");
			file.append("</variable>\n");
		}
		else if(end_bloc.exactMatch(v[i]))
		{
			while(!isNested.isEmpty() && isNested.top())
			{
				file.append(pile.pop().toUtf8());
				isNested.pop();
			}
			file.append(pile.pop().toUtf8());
			isNested.pop();
			if(!isNested.isEmpty() && isNested.top())
			{
				file.append(pile.pop().toUtf8());
				isNested.pop();
			}
		}
		else if(comments.exactMatch(v[i]))
		{
			QStringList liste = comments.capturedTexts();
			file.append("<comment>#"+liste[1].toUtf8()+"</comment>\n");
		}
		else if(v[i] == "")
		{
			nbEmptyLine++;
			while(!isNested.isEmpty() && isNested.top())
			{
				file.append(pile.pop().toUtf8());
				isNested.pop();
			}
			if(i+1 < v.size())
			{
				if(v[i+1] != "")
				{
					file.append("<emptyline>"+QString::number(nbEmptyLine)+"</emptyline>\n");
					nbEmptyLine = 0;
				}
			}
			else
				file.append("<emptyline>"+QString::number(nbEmptyLine)+"</emptyline>\n");
		}
		else
		{
			qWarning("%s didn't match", qPrintable(v[i]));
		}
	}
	file.append("</project>\n");
	
	return file.toUtf8();
}

QByteArray convertNodeToPro( const QDomElement& e, const QString& v )
{
	static int tabs = 0; // tabs indentation
	static bool isMultiline = false; // tell if last variable is multiline or not
	bool isNested = false; // tell if scope is nested or not
	QString comment; // comment of item if available
	QString data; // the data to return
	const QString tn = e.tagName(); // current node tag name
	const QString EOL = "\r\n"; // end of lines for generated file

	if ( tn != "project" )
	{
		if ( tn == "function" )
		{
			data.append( tabsString( tabs ) +e.firstChild().toText().data() );
			comment = e.attribute( "comment" );
			if ( !comment.isEmpty() )
				data.append( ' ' +comment );
			data.append( EOL );
		}
		else if ( tn == "emptyline" )
		{
			for ( int i = 0; i < e.firstChild().toText().data().toInt(); i++ )
				data.append( EOL );
		}
		else if ( tn == "variable" )
		{
			int vtabs = tabs;
			QDomElement n = e.parentNode().toElement();
			if ( n.tagName() == "scope" && QVariant( n.attribute( "nested", "false" ) ).toBool() )
				vtabs--;
			data.append( tabsString( vtabs ) +QString( "%1\t%2 " ).arg( e.attribute( "name" ) ).arg( e.attribute( "operator", "=" ) ) );
			isMultiline = QVariant( e.attribute( "multiline", "false" ) ).toBool();
		}
		else if ( tn == "value" )
		{
			int vtabs = tabs;
			if ( !e.previousSibling().isNull() && isMultiline )
				vtabs++;
			else if ( e.previousSibling().isNull() || !isMultiline )
				vtabs = 0;
			data.append( tabsString( vtabs ) +e.firstChild().toText().data() );
			comment = e.attribute( "comment" );
			if ( isMultiline )
			{
				if ( !e.nextSibling().isNull() )
					data.append( " \\" );
				if ( !comment.isEmpty() )
					data.append( ' ' +comment );
				data.append( EOL );
			}
			else if ( e.nextSibling().isNull() )
			{
				if ( !comment.isEmpty() )
					data.append( ' ' +comment );
				data.append( EOL );
			}
			else
				data.append( ' ' );
		}
		else if ( tn == "scope" )
		{
			int vtabs = tabs;
			if ( e.attribute( "name" ) == "else" && !QVariant( e.previousSibling().toElement().attribute( "nested", "false" ) ).toBool() )
				vtabs = 0;
			isNested = QVariant( e.attribute( "nested", "false" ) ).toBool();
			comment = e.attribute( "comment" );
			data.append( tabsString( vtabs ) +e.attribute( "name" ) );
			if ( !isNested )
			{
				data.append( " {" );
				if ( !comment.isEmpty() )
					data.append( ' ' +comment );
				data.append( EOL );
				tabs++;
			}
			else
				data.append( ':' );
		}
		else if ( tn == "comment" )
		{
			int vtabs = tabs;
			if ( e.parentNode().toElement().tagName() == "variable" && isMultiline )
				vtabs++;
			data.append( tabsString( vtabs ) +e.firstChild().toText().data() +EOL );
		}
	}
	else
		tabs = 0;
	
	const QStringList tv = QStringList() << "function" << "emptyline" << "value" << "comment";
	if ( e.hasChildNodes() && !tv.contains( tn ) )
	{
		QDomNodeList l = e.childNodes();
		for ( int i = 0; i < l.count(); i++ )
			data.append( convertNodeToPro( l.at( i ).toElement(), v ) );
		
		if ( tn == "scope" && !isNested )
		{
			tabs--;
			data.append( tabsString( tabs ) +"}" );
			QDomElement n = e.nextSibling().toElement();
			if ( !( n.tagName() == "scope" && n.attribute( "name" ) == "else" ) )
				data.append( EOL );
			else
				data.append( ' ' );
		}
	}

	return data.toAscii();
}

QByteArray QMake2XUP::convertToPro( const QDomDocument& d, const QString& v )
{
	// get project node
	QDomElement e  = d.firstChildElement( "project" ).toElement();
	// check project available
	if ( e.isNull() )
		return QByteArray();
	// parse project scope
	return convertNodeToPro( e, v );
}
