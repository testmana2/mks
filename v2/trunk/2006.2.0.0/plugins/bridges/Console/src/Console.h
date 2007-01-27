#ifndef CONSOLE_H
#define CONSOLE_H
//
#include "BasePlugin.h"
#include "ConsoleCommand.h"
#include "ConsoleCommandParser.h"
//
#include <QProcess>
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
protected:
	QProcess* mProcess;
	ConsoleCommandParser* mParser;
	ConsoleCommandsList mConsoleCommandsList;
	bool mStop;
	//
public slots:
	void setEnvironment( const QStringList& );
	void getEnvironment( QStringList& );
	void addCommands( const ConsoleCommands& );
	void removeCommands( const ConsoleCommands& );
	void getCommandsList( ConsoleCommandsList& );
	void run();
	void isRunning( bool& );
	void runConsoleCommands( const ConsoleCommands& );
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
	void consoleCommandFinished( const ConsoleCommand& );
	// messagebox slots
	void messageBox( const QString& );
	void clearMessageBox();
	void dataAvailable( const QString& );
	void showListBox();
	void showConsole();
	// parser signal
	void newErrorAvailable( const ConsoleCommandParser::ErrorInfos& );
	//
};
//
#endif // CONSOLE_H
