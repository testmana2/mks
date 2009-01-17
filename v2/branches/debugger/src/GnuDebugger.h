#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <QObject>
#include <QString>
#include <QList>

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
	void prepare_loadTarget ();
	
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

signals:
	void positionChanged (const QString& fileName, int line);
	
	void callStackUpdate (const GnuDebugger::CallStack&);
};

#endif // GNUDEBUGGER_H
