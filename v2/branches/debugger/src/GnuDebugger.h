#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <QObject>
#include <QString>

#include "mi_gdb.h"

class GnuDebugger : public QObject
{
	Q_OBJECT
public:
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
};

#endif // GNUDEBUGGER_H
