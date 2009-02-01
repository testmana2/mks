#ifndef QGDB_DRIVER_H
#define QGDB_DRIVER_H

#include <QObject>
#include <QString>
#include <QList>

#include "QGdb.h"
#include "QGdbBreakpoint.h"
#include "QGdbCallStack.h"
#include "QGdbSignal.h"

class QTimer;

namespace QGdb
{
	class Driver : public QObject
	{
		Q_OBJECT
		
	public:
		Driver( QObject* parent = 0 );
		virtual ~Driver();
		
		bool connectToGdb();
		void disconnectFromGdb();
		
		QString lastError() const;
		void appendLog( const QString& msg );
		
		bool exec_setExecutable( const QString& executable );
		bool exec_run();
		bool exec_continue();
		
		bool stack_listFrames();
	
	protected:
		QTimer* mAsyncPollTimer;
		mi_h* mHandle;
		QGdb::State mState;
		QString mTargetFileName;
		bool mWaitingTempBreakpoint;
		
		// migdb callbacks
		static void callbackConsole( const char* str, void* instance );
		static void callbackTarget( const char* str, void* instance );
		static void callbackLog( const char* str, void* instance );
		static void callbackToGDB( const char* str, void* instance );
		static void callbackFromGDB( const char* str, void* instance );
		static void callbackAsync( mi_output* output, void* instance );
		void handleStop( mi_stop* stop );
		
		void setState( QGdb::State state );
		bool runToMain();
		QString filePath( const QString& fileName ) const;
		void generateCallStack( mi_frames* mframe );
		void generateCallStack( mi_stop* stop );
	
	protected slots:
		void asyncPollTimer_timeout();
		
	/*	
		
		
		QStringList sourcesPath() const;

	public slots:
		// preparing to execution
		void prepare_startXterm ();
		
		// target execution
		
		void exec_setArgs (const QString& args);

		
		int exec_stepInto( bool instruction = false );
		int exec_stepOver( bool instruction = false );
		int exec_stepOut();
		int exec_pause();
		int exec_kill();
		
		void stack_selectFrame (int frame);
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
		
		QGdb::BreakpointList mBreakpoints;
		
		
		
		
		void delayedCall( const char* member );
		
	protected slots:
		// touches gdb for make it alive
		// Without it we haven't callbacks
		void onGdbTouchTimerTick();
		void sendFakeBreakpoints();
	*/
		
	signals:
		void callbackMessage( const QString& message, QGdb::CBType type );
		void stateChanged( QGdb::State state );
		void signalReceived( const QGdb::Signal& signal );
		void exitSignalReceived( const QGdb::Signal& signal );
		void exited( int code );
		void callStackUpdated( const QGdb::CallStackFrameList& stack, int selectedLevel );
		void positionChanged( const QString& fileName, int line );
		
	/*
		void breakpointAdded( const QGdb::Breakpoint& breakpoint );
		void breakpointRemoved( const QGdb::Breakpoint& breakpoint );
		void breakpointUpdated( const QGdb::Breakpoint& breakpoint );
		void breakpointsCleared( const QString& fileName = QString::null, int line = -1 );
		
		
	*/
	};
};

#endif // QGDB_DRIVER_H