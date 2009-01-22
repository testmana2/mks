#ifndef QGDBDRIVER_H
#define QGDBDRIVER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>
#include <QMetaType>

#include "mi_gdb.h"

class QGdbDriver : public QObject
{
	Q_OBJECT
	
public:
	enum CBType
	{
		CONSOLE = 0,
		TARGET,
		LOG,
		TO_GDB,
		FROM_GDB,
		ASYNC
	};
	
	enum State
	{
		CONNECTED = 0,
		DISCONNECTED,
		TARGET_SETTED,
		RUNNING,
		STOPPED
	};
	
	struct FunctionArgument
	{
		QString name;
		QString value;
	};
	typedef QList<FunctionArgument> ArgumentsList;
	
	struct Frame
	{
		int level;
		QString function;
		QString file;
		QString from; // ?
		QString full;
		int line;
		int thread_id;
		ArgumentsList arguments;
	};
	typedef QList<Frame> CallStack;
	
	QGdbDriver();
	virtual ~QGdbDriver();
	
	QString filePath( const QString& fileName ) const;

public slots:
	void log( const QString& msg );
	// preparing to execution
	void prepare_startXterm ();
	
	// target execution
	
	void exec_setCommand (const QString& command);
#if 0
	void exec_setArgs (const QString& args);
#endif
	void exec_run();
	void exec_continue();
	void exec_stepInto();
	void exec_stepOver();
	void exec_stepOut();
	void exec_pause();
	void exec_kill();
	
	// breakpoints
	void break_setBreaktoint( const QString& file, int line );
	
protected:
	State mState;
	mi_h* mHandle;
	mi_aux_term* mXterm;
	QTimer mGdbPingTimer;
	QString mTargetFileName;
	
	// migdb callbacks
	static void callbackConsole (const char *, void *);
	static void callbackTarget(const char *str, void *);
	static void callbackLog (const char *str, void *);
	static void callbackToGDB (const char *str, void *);
	static void callbackFromGDB (const char *str, void *);
	static void callbackAsync (mi_output * o, void * debuggerInstance);
	
	void setState (State);
protected slots:
	// touches gdb for make it alive
	// Without it we haven't callbacks
	void onGdbTouchTimerTick();
	void getCallStack( mi_stop* stop );
	
signals:
	void callbackMessage( const QString& message, QGdbDriver::CBType type );
	void stateChanged( QGdbDriver::State state );
	void callStackUpdated( const QGdbDriver::CallStack& stack );
	void positionChanged (const QString& fileName, int line); // should be renamed ?
	void exited( int code );
};

Q_DECLARE_METATYPE( QGdbDriver::Frame )

#endif // QGDBDRIVER_H
