#include <QDebug>

#include "GNUMakeParser.h"

GNUMakeParser::GNUMakeParser()
{
	Pattern ps[] = 
	{
		{
			QRegExp("^make: \\*\\*\\* (No rule to make target `.+', needed by `.+')\\.  Stop\\.$"), //reg exp
			"No rule for make target", //desctiption
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::Error, //type
			"%1", //text
			"%0" //full text
		},
        {
            QRegExp("make\\[\\d\\]: Entering directory\\s`([^\\n]*)'"), //reg exp
            "Entering dirrectory", //desctiption
            "", //file name
            "0", //column
            "0", //row
            pConsoleManager::Compiling, //type
            "make: Building %1", //text
            "%0" //full text
        },
		{QRegExp(), "", "", "", "", pConsoleManager::Unknown,"",""} //this item must be last
	};
	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
		patterns.append (ps[i]);

}

GNUMakeParser::~GNUMakeParser()
{
}

bool GNUMakeParser::parse(const QByteArray* arr)
{
	QStringList l = QString(*arr).split( '\n' );
	foreach (QString s, l)
	{
		foreach ( Pattern p, patterns)
		{
			if (p.regExp.indexIn(s) != -1)
			{
				pConsoleManager::Message m;
				m.mFileName = replaceWithMatch(p.regExp,p.FileName);
				m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(),replaceWithMatch(p.regExp,p.col).toInt());
				m.mType = p.Type;
				m.mText = replaceWithMatch(p.regExp,p.Text);
				m.mFullText = replaceWithMatch(p.regExp,p.FullText);
				// emit signal
				emit newMessageAvailible( m );
				return true;
			}
			
		}
	}
return false;
}

QString GNUMakeParser::name() const
{
	return QString ("GNUMake");
}

QString GNUMakeParser::replaceWithMatch(QRegExp rex, QString s)
{
	int i = 0;
	while ( (i = s.indexOf("%")) != -1)
		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
	return s;
}