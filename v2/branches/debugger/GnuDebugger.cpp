#include "GnuDebugger.h"

GnuDebugger::GnuDebugger()
{
	mHandle	= mi_connect_local ();
	Q_ASSERT (mHandle); // FIXME replace with some error message
	exec_setCommandAndArgs ("./target_frames","");
	break_setBreaktoint ("target_frames.cc",7);
	exec_run();
}

GnuDebugger::~GnuDebugger()
{
}

void GnuDebugger::prepare_startXterm ()
{
	int res = 0;
	mXterm = gmi_start_xterm();
	Q_ASSERT (mXterm);
	res = gmi_target_terminal(mHandle, mXterm->tty);
	Q_ASSERT (res == 0);
}

void GnuDebugger::exec_setCommandAndArgs (const QString& command, const QString& args)
{
	int res = 0;
	res = gmi_set_exec(mHandle, command.toLocal8Bit(), args.toLocal8Bit());
	Q_ASSERT (res == 0);
}

void GnuDebugger::exec_run()
{
	int res = 0;
	res = gmi_exec_run(mHandle);
	Q_ASSERT (res);
}

void GnuDebugger::exec_continue()
{

}

void GnuDebugger::exec_stepInto()
{

}

void GnuDebugger::exec_stepOver()
{

}

void GnuDebugger::exec_pause()
{

}

void GnuDebugger::break_setBreaktoint (const QString& file, int line)
{
	mi_bkpt *bk = NULL;
	bk = gmi_break_insert(mHandle, file.toLocal8Bit(), line);
	Q_ASSERT (bk);
	mi_free_bkpt(bk);
	// FIXME memory leak??
}
