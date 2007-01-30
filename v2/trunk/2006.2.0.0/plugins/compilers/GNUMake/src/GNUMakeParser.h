#ifndef GNUMAKEPARSER_H
#define GNUMAKEPARSER_H
//
#include "ConsoleCommandParser.h"
//
class GNUMakeParser : public ConsoleCommandParser
{
	Q_OBJECT
	//
public:
	GNUMakeParser( const QString& = "^([^:]+):(\\d+):(\\d+:)?\\s((warning)|(error)):\\s(.+)$", const QString& = "^g\\+\\+.+([^\\s]+\\.cpp)" );
	//
	virtual QString resultString();
	//
	virtual void appendToBuffer( const QString& );
	//
private:
	void parse();
	//
};
//
#endif // GNUMAKEPARSER_H
