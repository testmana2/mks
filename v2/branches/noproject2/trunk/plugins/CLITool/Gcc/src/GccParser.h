#ifndef GCCPARSER_H
#define GCCPARSER_H

#include <QProcess>
#include <QList>

#include "pCommandParser.h"

class GccParser : public pCommandParser
{
struct Pattern
{
	QString regExp;
	QString about;
	QString mFileName;
	QString col;
	QString row;
	pConsoleManager::MessageType mType;
	QString mText;
	QString mFullText;
};

Q_OBJECT
private:
	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;
	QList <Pattern> patterns;
public:
	GccParser();
	~GccParser();

	QString name() const;
	bool parse(const QByteArray*);

};

#endif
