#ifndef GCCPARSER_H
#define GCCPARSER_H

#include <QProcess>

#include "pCommandParser.h"

class GccParser : public pCommandParser
{
Q_OBJECT
private:
	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;

public:
	GccParser();
	~GccParser();

	QString name() const;
	bool parse(const QByteArray*);

};

#endif
