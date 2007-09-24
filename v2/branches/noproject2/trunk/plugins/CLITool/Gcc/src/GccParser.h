#ifndef GCCPARSER_H
#define GCCPARSER_H

#include "pCommandParser.h"

class GccParser : public pCommandParser
{
public:
    GccParser();
    virtual ~GccParser();

    bool parse(QByteArray*);

    QString name();

};

#endif
