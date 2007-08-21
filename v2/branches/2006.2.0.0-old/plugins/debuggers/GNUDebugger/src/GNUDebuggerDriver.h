#include <QProcess>
#include <QFile>
#include <QTextStream>


class GNUDebuggerDriver : public QObject
{
	Q_OBJECT
public:
	GNUDebuggerDriver();
	~GNUDebuggerDriver();
	QProcess *mpGdbProcess;
	
	void setConfig()
		{ setCommand("set height 0\r\nset width 0\r\n set confirm off");}

	void setQuit()
		{ setCommand("quit");}

	// ouvre le fichier a debugger
	void setFile(QString progpath)
		{setCommand("file " + QString(progpath) );  }

	// controle
	void setRun(QString args)
		{setCommand("run " + args );}

	void setNextStepInto()
		{setCommand("step");}

	void setNextStepOver()
		{setCommand("next");}

	void setContinue()
		{setCommand("continue");}

	void setCommand(QString command);	

	void setBreakpoint(QString breakpoint_at)
		{setCommand(breakpoint_at);}

private:

	QString GdbName;
	// queue command
	QList <QString> mCommandList;
	// Gdb Idl ?
	bool gdbBusy;
	bool gdbStarted;
	QString st;

	// log file
	QFile *file_log_gdb;
	void openLogFile();
	void closeLogFile();
	void writeLogFile(QString);

private slots:
	void onReadAll();
	void slotGdbCommandFinish();
	void OnError(QProcess::ProcessError er);
	void gdbfinished ( int , QProcess::ExitStatus  );

signals:
	void signalGdbCommandFinish();
	void gdbDataAvailable(QString);
};

