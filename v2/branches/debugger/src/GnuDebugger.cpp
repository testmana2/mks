#include <QMetaType>
#include <QMessageBox>

#include "GnuDebugger.h"
#include <QDebug>

// Callbacks of debugger
void GnuDebugger::callbackConsole (const char * str, void *)
{
	printf("CONSOLE> %s\n",str);
}

/* Note that unlike what's documented in gdb docs it isn't usable. */
void GnuDebugger::callbackTarget(const char *str, void *)
{
	printf("TARGET> %s\n",str);
}

void GnuDebugger::callbackLog (const char *str, void *)
{
 printf("LOG> %s\n",str);
}

void GnuDebugger::callbackToGDB (const char *str, void *)
{
	printf(">> %s",str);
}

void GnuDebugger::callbackFromGDB (const char *str, void *)
{
	printf("<< %s\n",str);
}

void GnuDebugger::callbackAsync (mi_output * o, void * debuggerInstance)
{
	printf("ASYNC callback\n");
	Q_ASSERT (o);
	if (MI_CL_STOPPED == o->tclass)
	{
		if (QString ("exited") == o->c->v.cstr)
			static_cast<GnuDebugger*>(debuggerInstance)->mTargetBeenExited = true;
		else if ((QString ("breakpoint-hit") == o->c->v.cstr) ||
				 (QString ("end-stepping-range") == o->c->v.cstr) ||
				 (QString ("function-finished") == o->c->v.cstr))
			static_cast<GnuDebugger*>(debuggerInstance)->mTargetBeenStopped = true;
	}
	printf("callback exit\n");
}


GnuDebugger::GnuDebugger()
  : QObject(),
	mState (NOT_LOADED),
	mHandle (0),
    mTargetBeenStopped (false),
    mTargetLoaded (false)
{
	mHandle	= mi_connect_local ();
	Q_ASSERT (mHandle); // FIXME replace with some error message
	
	mi_set_console_cb (mHandle, callbackConsole, this);
	mi_set_target_cb (mHandle, callbackTarget, this);
	mi_set_log_cb (mHandle, callbackLog, this);
	mi_set_async_cb (mHandle, callbackAsync, this);
	mi_set_to_gdb_cb (mHandle, callbackToGDB, this);
	mi_set_from_gdb_cb (mHandle, callbackFromGDB, this);
	
	connect (this, SIGNAL (stopped()), this, SLOT (onStopped()));
	connect (&mGdbPingTimer, SIGNAL (timeout()), this, SLOT(onGdbTouchTimerTick ()));
	
	qRegisterMetaType<GnuDebugger::CallStack> ("GnuDebugger::CallStack");
	qRegisterMetaType<GnuDebugger::CallStack> ("GnuDebugger::State");
	
	mGdbPingTimer.setInterval (50);
	mGdbPingTimer.start();
}

GnuDebugger::~GnuDebugger()
{
	gmi_gdb_exit(mHandle);
	mi_disconnect(mHandle);
}

void GnuDebugger::prepare_startXterm ()
{
	int res = 0;
	mXterm = gmi_start_xterm();
	Q_ASSERT (mXterm);
	res = gmi_target_terminal(mHandle, mXterm->tty);
	Q_ASSERT (res != 0);
}

void GnuDebugger::exec_setCommand (const QString& command)
{
	int res = 0;
	res = gmi_set_exec(mHandle, command.toLocal8Bit(), "");
	if (0 != res)
	{
		setState (LOADED);
		emit stateChanged (mState);
	}
	else
	{
		QMessageBox::critical (NULL, tr ("Failed to load target"), tr ("GDB error: ") + mi_get_error_str());
		mTargetLoaded = false;
	}
}

#if 0
	void GnuDebugger::exec_setArgs (const QString& args)
	{
		int res = 0;
		res = gmi_set_exec(mHandle, command.toLocal8Bit(), args.toLocal8Bit());
		Q_ASSERT (res != 0);
	}
#endif

void GnuDebugger::exec_run()
{
	int res = 0;
	res = gmi_exec_run(mHandle);
	Q_ASSERT (res);
	setState (RUNNING);
}

void GnuDebugger::exec_continue()
{
	int res = 0;
	res = gmi_exec_continue (mHandle);
	Q_ASSERT (res);
	setState (RUNNING);
}

void GnuDebugger::exec_stepInto()
{
	int res = 0;
	res = gmi_exec_step (mHandle);
	Q_ASSERT (res);
	setState (RUNNING);
}

void GnuDebugger::exec_stepOver()
{
	int res = 0;
	res = gmi_exec_next (mHandle);
	Q_ASSERT (res);
	setState (RUNNING);
}

void GnuDebugger::exec_stepOut()
{
	int res = 0;
	res = gmi_exec_finish (mHandle);
	Q_ASSERT (res);
	setState (RUNNING);
}

void GnuDebugger::exec_pause()
{
	int res = 0;
	res = gmi_exec_interrupt (mHandle);
	Q_ASSERT (res);
}

void GnuDebugger::exec_kill()
{
	int res = 0;
	res = gmi_exec_kill (mHandle);
	Q_ASSERT (res);
	setState (EXITED_NORMAL);
}

void GnuDebugger::break_setBreaktoint (const QString& file, int line)
{
	mi_bkpt *bk = NULL;
	bk = gmi_break_insert(mHandle, file.toLocal8Bit(), line);
	Q_ASSERT (bk);
	mi_free_bkpt(bk);
}

void GnuDebugger::stack_Info ()
{
	mi_frames * frames = gmi_stack_list_frames (mHandle);;
	Q_ASSERT (frames);	
	mi_frames * arguments = gmi_stack_list_arguments (mHandle, 1);
	Q_ASSERT (arguments);
	
	CallStack stack;
	
	while (NULL != frames && NULL != arguments)
	{
		Frame frame;
		frame.function = frames->func;
		frame.file = frames->file;
		frame.line = frames->line;
		frame.level = frames->level;
		
		mi_results* arg = arguments->args;
		
		while (NULL != arg)
		{
			FunctionArgument argument;
			argument.name = arg->v.rs->v.cstr;
			argument.value = arg->v.rs->next->v.cstr;
			frame.arguments << argument;
			
			arg = arg->next;
		}
		
		stack << frame;
		
		frames = frames->next;
		arguments = arguments->next;
	}
	if (stack.size())
		emit positionChanged (stack[0].file, stack[0].line);
	
	emit callStackUpdate (stack);
	
	mi_free_frames(frames);
	mi_free_frames(arguments);
}

void GnuDebugger::onGdbTouchTimerTick ()
{
	if (mi_get_response(mHandle))
	{
		internalUpdate();
	}
}

void GnuDebugger::internalUpdate()
{
	if (mTargetBeenStopped)
	{
		mTargetBeenStopped = false;
		setState (PAUSED);
		onStopped();
	}
	else if (mTargetBeenExited)
	{
		setState (EXITED_NORMAL);
	}
}

void GnuDebugger::setState (State state)
{
	mState = state;
	emit stateChanged (mState);
}

void GnuDebugger::onStopped()
{
	stack_Info ();
}
