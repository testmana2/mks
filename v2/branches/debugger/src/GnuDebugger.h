#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include "mi_gdb.h"

class GnuDebugger : public QObject
{
	Q_OBJECT
public:
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
		int line;
		int thread_id;
		ArgumentsList arguments;
	};
	typedef QList<Frame> CallStack;
	
	GnuDebugger();
	virtual ~GnuDebugger();
	
	// preparing to execution
	void prepare_startXterm ();
	
	// target execution
	
	void exec_setCommandAndArgs (const QString& command, const QString& args);
	void exec_run();
	void exec_continue();
	void exec_stepInto();
	void exec_stepOver();
	void exec_pause();
	
	// breakpoints
	void break_setBreaktoint (const QString& file, int line);
	
	// stack
	void stack_Info ();
	
protected:
	mi_h* mHandle;
	mi_aux_term* mXterm;
	
	bool mTargetBeenStopped;
	
	QTimer mGdbPingTimer;
	
	
	// migdb callbacks
	static void callbackConsole (const char *, void *);
	static void callbackTarget(const char *str, void *);
	static void callbackLog (const char *str, void *);
	static void callbackToGDB (const char *str, void *);
	static void callbackFromGDB (const char *str, void *);
	static void callbackAsync (mi_output * o, void * debuggerInstance);
	
	void internalUpdate ();
	
	
protected slots:
	// touches gdb for make it alive
	// Without it we haven't callbacks
	void onGdbTouchTimerTick ();
	
	void onStopped();
	
signals:
	void stopped ();
	void positionChanged (const QString& fileName, int line);
	
	void callStackUpdate (const GnuDebugger::CallStack&);
};

#endif // GNUDEBUGGER_H
