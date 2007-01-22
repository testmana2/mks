#ifndef CONSOLE_H
#define CONSOLE_H
//
#include "BasePlugin.h"
//
#include <QProcess>
//
class ConsoleCommand
{
public:
	ConsoleCommand( const QString& c, const QString& w )
		: mCommand( c ), mWorkingFolder( w ) {}
	//
	bool isNull() const { return mCommand.isNull(); }
	QString command() const { return mCommand; }
	void setCommand( const QString& s ) { mCommand = s; }
	QString workingFolder() const { return mWorkingFolder; }
	void setWorkingFolder( const QString& s ) { mWorkingFolder = s; }
	//
	bool operator == ( const ConsoleCommand& c ) const
	{ return mCommand == c.command() && mWorkingFolder == c.workingFolder(); }
	//
private:
	QString mCommand; // the command line to execute
	QString mWorkingFolder; // the working directory
	//
};
//
typedef QList<ConsoleCommand> ConsoleCommands; // a list of ConsoleCommand
typedef QList<ConsoleCommands> ConsoleCommandsList; // a type for list of ConsoleCommands
//
class Console : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	//
public:
	virtual ~Console();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	//
	bool isRunning();
	QStringList systemEnvironment() const;
	ConsoleCommandsList commandsList() const;
	//
private:
	QProcess* mProcess;
	ConsoleCommandsList mConsoleCommandsList;
	bool mStop;
	//
public slots:
	void setSystemEnvironment( const QStringList& );
	void addCommands( const ConsoleCommands& );
	void removeCommands( const ConsoleCommands& );
	void run();
	void runConsoleCommands( ConsoleCommands* );
	void stopConsole();
	//
private slots:
	void error( QProcess::ProcessError );
	void finished( int, QProcess::ExitStatus );
	void readyRead();
	void started();
	void stateChanged( QProcess::ProcessState );
	//
signals:
	void messageBox( const QString& );
	void clearMessageBox();
	void dataAvailable( const QString& );
	//
};
//
#endif // CONSOLE_H
