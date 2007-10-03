/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : ConsoleManager
** FileName  : ConsoleManager.h
** Date      : mar. août 21 20:52:27 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PCONSOLEMANAGER_H
#define PCONSOLEMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pCommand.h"

#include <QApplication>
#include <QProcess>

class Q_MONKEY_EXPORT pConsoleManager : public QProcess, public QSingleton<pConsoleManager>
{
	Q_OBJECT
	friend class QSingleton<pConsoleManager>;
	
public:

	// FIXME DO NOT SURE IT'S THE BEST PLACE FOR Message and MessageType definition. hlamer
	enum MessageType { Unknown = -1, Error, Warning, Good, Bad, Compiling, State };
	struct Message
	{
		QString mFileName;
		QPoint mPosition;
		MessageType mType;
		QString mText;
		QString mFullText;
	};

	pCommand* currentCommand() const { return mCommands.value( 0 ); }

protected:
	int mTimerId;
	pCommandList mCommands;
	QHash<QString,pCommandParser*> parsers;//list of all availible parsers
	QList<pCommandParser*> currentParsers;//list of parsers, that used for current command. First elements will be applyed at first.
	QProcess parserProcess ;
	void timerEvent( QTimerEvent* );
	//Generate a list of parsers, that will be used for command, that executing now
	void setCurrentParsers (QStringList, bool);

private:
	pConsoleManager( QObject* = QApplication::instance() );
	~pConsoleManager();

public:
	//add parser to the list of active parsers. Will be used by CLIToolPlugin
	void addParser (pCommandParser*);
	//remove parser from the list of active parsers. Will be used by CLIToolPlugin
	void removeParser (QString);

public slots:
	void sendRawCommand( const QString& );
	void sendRawData( const QByteArray& );
	void stopCurrentCommand( bool = false );
	void addCommand( pCommand* );
	void addCommands( const pCommandList& );
	void removeCommand( pCommand* );
	void removeCommands( const pCommandList& );

private slots:
	void executeProcess();
	void error( QProcess::ProcessError );
	void finished( int, QProcess::ExitStatus );
	void readyRead();
	void started();
	void stateChanged( QProcess::ProcessState );
	void readyReadParser ();

signals:
	void commandError( pCommand*, QProcess::ProcessError );
	void commandFinished( pCommand*, int, QProcess::ExitStatus );
	void commandReadyRead( pCommand*, const QByteArray& );
	void commandStarted( pCommand* );
	void commandStateChanged( pCommand*, QProcess::ProcessState );
	void commandSkipped( pCommand* );
	void newMessageAvailible (const pConsoleManager::Message);
};

#endif // PCONSOLEMANAGER_H
