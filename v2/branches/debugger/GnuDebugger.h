#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <QString>

#include "mi_gdb.h"

class GnuDebugger
{
public:
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

protected:
	mi_h* mHandle;
	mi_aux_term* mXterm;
};

#endif // GNUDEBUGGER_H
