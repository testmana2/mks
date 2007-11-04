/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : GccParser.cpp
** Date      : 2007-11-04T22:53:26
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "GccParser.h"

#include <QTextCodec>

GccParser::GccParser()
{
	Pattern ps[] = 
	{
		{
			QRegExp("^(.*/)?([^:]+):(\\d+):(\\d+:)?\\serror:\\s(.+)$"), //reg exp
			"Error in the file/line", //desctiption
			"%2", //file name
			"%4", //column
			"%3", //row
			pConsoleManager::stError, //type
			"%2:%3: %5", //text
			"%0", //full text
		},
		{
			QRegExp("^(.*/)?([^:]+):(\\d+):(\\d+:)?\\swarning:\\s(.+)$"), //reg exp
			"Warning in the file/line", //desctiption
			"%2", //file name
			"%4", //column
			"%3", //row
			pConsoleManager::stWarning, //type
			"%2:%3: %5", //text
			"%0" //full text
		},
		{
			QRegExp("^g\\+\\+\\s+\\-c.+([^\\s]+\\.cpp)"), //reg exp
			"Building file", //desctiption
			"%1", //file name
			"0", //column
			"0", //row
			pConsoleManager::stCompiling, //type
			"Compiling %1...", //text
			"%0" //full text
		},
		{
			QRegExp("^g\\+\\+.+\\-o\\s+([^\\s]+).+"), //reg exp
			"Linking file", //desctiption
			"0", //file name
			"0", //column
			"0", //row
			pConsoleManager::stLinking, //type
			"Linking %1...", //text
			"%0" //full text
		},
		{
			QRegExp("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$"), //reg exp
			"Undedined reference", //desctiption
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1%2", //text
			"%0" //full text
		},
		{QRegExp(), "", "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
	};
	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
		patterns.append (ps[i]);
}

GccParser::~GccParser()
{
}

bool GccParser::processParsing(const QByteArray& arr)
{
	QStringList l = QTextCodec::codecForLocale()->toUnicode( arr ).split( '\n' );
	foreach (QString s, l)
	{
		foreach ( Pattern p, patterns)
		{
			if (p.regExp.indexIn(s) != -1)
			{
				pConsoleManager::Step m;
				m.mFileName = replaceWithMatch(p.regExp,p.FileName);
				m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(), replaceWithMatch(p.regExp,p.col).toInt());
				m.mType = p.Type;
				m.mText = replaceWithMatch(p.regExp,p.Text);
				m.mFullText = replaceWithMatch(p.regExp,p.FullText);
				// emit signal
				emit newStepAvailable( m );
				return true;
			}
			
		}
	}
return false;
}

QString GccParser::name() const
{
	return PLUGIN_NAME;
}

QString GccParser::replaceWithMatch(QRegExp rex, QString s)
{
	int i = 0;
	while ( (i = s.indexOf("%")) != -1)
		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
	return s;
}
