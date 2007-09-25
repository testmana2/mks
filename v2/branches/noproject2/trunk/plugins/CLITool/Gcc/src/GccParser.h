#ifndef GCCPARSER_H
#define GCCPARSER_H

#include <QRegExp>

#include "pCommandParser.h"

class GccParser : public pCommandParser
{
Q_OBJECT
private:
	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;

public:
	GccParser();
	~GccParser();

    bool parse(const QByteArray*);

    QString name() const;
};

#endif
