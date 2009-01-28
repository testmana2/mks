#ifndef QGDBDRIVER_H
#define QGDBDRIVER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include "QGdbBreakpoint.h"
#include "QGdbCallStack.h"
#include "QGdbSignal.h"

class QGdbDriver : public QObject
{
	Q_OBJECT
	
public:
	QGdbDriver();
	virtual ~QGdbDriver();
	
	QString filePath( const QString& fileName ) const;
	
	QStringList sourcesPath() const;

public slots:
	void log( const QString& msg );
	// preparing to execution
	void prepare_startXterm ();
	
	// target execution
	
	void exec_setCommand (const QString& command);
	//void exec_setArgs (const QString& args);
	
	int runToMain();
	int exec_run();
	int exec_continue();
	int exec_stepInto( bool instruction = false );
	int exec_stepOver( bool instruction = false );
	int exec_stepOut();
	int exec_pause();
	int exec_kill();
	
	// breakpoints
	int break_setBreakpoint( const QString& file, int line );
	void break_breakpointToggled( const QString& file, int line );
	void clearBreakpoints( int line = -1 );
	
protected:
	QGdb::State mState;
	mi_h* mHandle;
	mi_aux_term* mXterm;
	QTimer mGdbPingTimer;
	QString mTargetFileName;
	bool mWaitingTempBreakpoint;
	QGdb::BreakpointList mBreakpoints;
	
	// migdb callbacks
	static void callbackConsole (const char *, void *);
	static void callbackTarget(const char *str, void *);
	static void callbackLog (const char *str, void *);
	static void callbackToGDB (const char *str, void *);
	static void callbackFromGDB (const char *str, void *);
	static void callbackAsync (mi_output * o, void * debuggerInstance);
	
	void setState( QGdb::State state );
	void delayedCall( const char* member );
	
protected slots:
	// touches gdb for make it alive
	// Without it we haven't callbacks
	void onGdbTouchTimerTick();
	void sendFakeBreakpoints();
	void generateCallStack( mi_frames* mframe );
	void updateCallStack( mi_stop* stop );
	void updateFullCallStack();
	
signals:
	void callbackMessage( const QString& message, QGdb::CBType type );
	void stateChanged( QGdb::State state );
	void exitSignalReceived( const QGdb::Signal& signal );
	void signalReceived( const QGdb::Signal& signal );
	void breakpointAdded( const QGdb::Breakpoint& breakpoint );
	void breakpointRemoved( const QGdb::Breakpoint& breakpoint );
	void breakpointUpdated( const QGdb::Breakpoint& breakpoint );
	void breakpointsCleared( const QString& fileName = QString::null, int line = -1 );
	void callStackUpdated( const QGdb::CallStackFrameList& stack, int selectedLevel );
	void positionChanged (const QString& fileName, int line); // should be renamed ?
	void exited( int code );
};

#endif // QGDBDRIVER_H
