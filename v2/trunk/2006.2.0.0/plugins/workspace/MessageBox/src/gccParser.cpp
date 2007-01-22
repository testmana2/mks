#include "gccParser.h"
#include <QObject>
#include <QWidget>
#include <QRegExp>
#include <QString>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QBrush>
#include<QColor>

gccParser::gccParser () :  QObject()
{
//^([^:]+):(\d+):(\d+:)?\s((warning)|(error)):\s(.+)$
regExp = "^([^:]+):(\\d+):(\\d+:)?\\s((warning)|(error)):\\s(.+)$";
}

QVector<errStruct*>* gccParser::parseString(QString gccOutput)
{
	buffer.append (gccOutput);
	if (!buffer.endsWith('\n'))
		return NULL;//we must recieve all string, before parse
		
	errPointers.clear();
	if (regExp == QString::null) return NULL;
	QRegExp regExpObj (regExp);
	QStringList output = buffer.split('\n');
	for (int i = 0; i < output.count(); i++)
	{	int pos = 0;
		 while ((pos = regExpObj.indexIn(output[i], pos)) != -1) 
		{	errStruct* errStructure = new errStruct;
			errStructure->fullText = regExpObj.cap(0);
			errStructure->fileName = regExpObj.cap(1); 
			errStructure->line = regExpObj.cap(2).toInt();
			if (	regExpObj.cap(4) =="error")
				errStructure->type = error;
			else if (	regExpObj.cap(4) =="warning")
				errStructure->type = warning;	
			errStructure->text = regExpObj.cap(7);
			errPointers.append(errStructure);
			pos += regExpObj.matchedLength();
		}
	}
	buffer.clear();
return &errPointers;	
}

void gccParser::setRegExp (QString newstr)
{
	regExp = newstr;
}
