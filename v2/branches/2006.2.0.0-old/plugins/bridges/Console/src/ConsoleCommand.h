#ifndef CONSOLECOMMAND_H
#define CONSOLECOMMAND_H
//
#include <QString>
#include <QList>
//
class ConsoleCommandParser;
//
class ConsoleCommand
{
public:
	ConsoleCommand( const QString& c, const QString& w, ConsoleCommandParser* p = 0 )
		: mCommand( c ), mWorkingFolder( w ), mParser( p ) {}
	//
	bool isNull() const { return mCommand.trimmed().isEmpty(); }
	QString command() const { return mCommand; }
	void setCommand( const QString& s ) { mCommand = s; }
	QString workingFolder() const { return mWorkingFolder; }
	void setWorkingFolder( const QString& s ) { mWorkingFolder = s; }
	ConsoleCommandParser* parser() const { return mParser; }
	void setParser( ConsoleCommandParser* p ) { mParser = p; }
	//
	bool operator == ( const ConsoleCommand& c ) const
	{ return command() == c.command() && workingFolder() == c.workingFolder() && parser() == c.parser(); }
	//
private:
	QString mCommand; // the command line to execute
	QString mWorkingFolder; // the working directory
	ConsoleCommandParser* mParser; // the output parser of this command
	//
};
//
typedef QList<ConsoleCommand> ConsoleCommands; // a list of ConsoleCommand
typedef QList<ConsoleCommands> ConsoleCommandsList; // a type for list of ConsoleCommands
//
#endif // CONSOLECOMMAND_H
