#ifndef GCCPARSER_H
#define GCCPARSER_H

#include "pCommandParser.h"

class GccParser : public pCommandParser
{
public:
	GccParser();
	~GccParser();

    bool parse(const QByteArray*);

    QString name() const;
};

#endif
