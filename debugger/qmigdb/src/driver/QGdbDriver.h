#ifndef QGDBDRIVER_H
#define QGDBDRIVER_H

#include <QObject>
#include <QString>
#include <QList>

#include "QGdb.h"
#include "QGdbBreakpoint.h"
#include "QGdbCallStack.h"
#include "QGdbSignal.h"
#include "QGdbVariable.h"
#include "VariablesModelItem.h"

//FIXME remove when developmet will be finished
#include <QDebug>



class QTimer;

namespace QGdb
{
	class Driver : public QObject
	{
		Q_OBJECT
		
	public:
		Driver( QObject* parent = 0 );
		virtual ~Driver();
		
		void enableDebugOutput (bool enable);
		
		bool connectToGdb();
		void disconnectFromGdb();
		
		QString lastError() const;
		void appendLog( const QString& msg );
		
		bool exec_setExecutable( const QString& executable );
		bool exec_run();
		bool exec_continue();
		bool exec_stepInto( bool instruction = false );
		bool exec_stepOver( bool instruction = false );
		bool exec_stepOut();
		bool exec_stop();
		bool exec_kill();
		
		bool stack_listFrames();
		bool stack_selectFrame( int level );
		
		void printVariable( const QGdb::Variable& variable );
		bool stack_listLocals();
		
		bool break_setBreakpoint( const QString& file, int line );
		
		void readLocals (VariablesModelItem* storage);
		void readArguments (VariablesModelItem* storage);
		
	public slots:
		void break_breakpointToggled( const QString& file, int line );
		void break_breakpointEdited( const QGdb::Breakpoint& before, const QGdb::Breakpoint& after );
		
		void stack_frameSelected( const QGdb::CallStackFrame& frame );
	
	protected:
		static bool mEnableDebugOutput;
		QTimer* mAsyncPollTimer;
		mi_h* mHandle;
		QGdb::State mState;
		QString mTargetFileName;
		bool mWaitingTempBreakpoint;
		QGdb::BreakpointList mBreakpoints;
		
		QGdb::VariableList mVariables;
		
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
		void generateListLocals( mi_results* results, QGdb::Variable* variable = 0 );
		
		// convert variable presented as mi_results to row of VariablesModelItem's
		bool varIsStructure( const QString& value );
		QList<QGdb::VariablesModelItem*> getStructureFields( const QString& value );
		QGdb::VariablesModelItem* getVariableItem(mi_results* variable);
	
	protected slots:
		void asyncPollTimer_timeout();
		
	/*
		QStringList sourcesPath() const;

	public slots:
		// preparing to execution
		void prepare_startXterm ();
		// target execution
		void exec_setArgs (const QString& args);
		// breakpoints
		void clearBreakpoints( int line = -1 );
		
	protected:
		mi_aux_term* mXterm;
		
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
		void breakpointAdded( const QGdb::Breakpoint& breakpoint );
		void breakpointRemoved( const QGdb::Breakpoint& breakpoint );
		void breakpointEdited( const QGdb::Breakpoint& before, const QGdb::Breakpoint& after );
		void localsUpdated();
		void argumentsUpdated();
		
	/*
		void breakpointsCleared( const QString& fileName = QString::null, int line = -1 );
	*/
	};
};

#endif // QGDBDRIVER_H
