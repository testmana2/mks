#ifndef GCCPARSER_H
#define GCCPARSER_H

#include <QProcess>

#include "pCommandParser.h"

class GccParser : public pCommandParser
{
Q_OBJECT
private:
	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;
    QProcess proc;

public:
	GccParser();
	~GccParser();

	QString name() const;
	bool parse(const QByteArray*);
public slots:
    void read();
};

#endif
