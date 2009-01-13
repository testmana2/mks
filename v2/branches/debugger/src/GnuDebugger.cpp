#include "GnuDebugger.h"

// Callbacks of debugger
static void callbackConsole (const char *, void *)
{
//	printf("CONSOLE> %s\n",str);
}

/* Note that unlike what's documented in gdb docs it isn't usable. */
static void callbackTarget(const char *str, void *)
{
	printf("TARGET> %s\n",str);
}

static void callbackLog (const char *str, void *)
{
 printf("LOG> %s\n",str);
}

static void callbackToGDB (const char *str, void *)
{
	printf(">> %s",str);
}

static void callbackFromGDB (const char *str, void *)
{
	printf("<< %s\n",str);
}

void callbackAsync (mi_output * o, void *)
{
	printf("ASYNC\n");
}


GnuDebugger::GnuDebugger()
	: QObject()
{
	mHandle	= mi_connect_local ();
	Q_ASSERT (mHandle); // FIXME replace with some error message
	
	mi_set_console_cb (mHandle, callbackConsole, this);
	mi_set_target_cb (mHandle, callbackTarget, this);
	mi_set_log_cb (mHandle, callbackLog, this);
	mi_set_async_cb (mHandle, callbackAsync, this);
	mi_set_to_gdb_cb (mHandle, callbackToGDB, this);
	mi_set_from_gdb_cb (mHandle, callbackFromGDB, this);
	
}

GnuDebugger::~GnuDebugger()
{
}

void GnuDebugger::prepare_loadTarget ()
{
	exec_setCommandAndArgs ("./target_frames","");
	
	prepare_startXterm();
	
	break_setBreaktoint ("target_frames.cc",7);
	exec_run();
	
	mi_stop *sr;
	while (!mi_get_response(mHandle))
		usleep(1000);
	/* The end of the async. */
	sr=mi_res_stop(mHandle);
	if (sr)
	{
		printf("Stopped, reason: %s\n",mi_reason_enum_to_str(sr->reason));
		mi_free_stop(sr);
	}
	else
	{
		printf("Error while waiting\n");
		printf("mi_error: %d\nmi_error_from_gdb: %s\n",mi_error,mi_error_from_gdb);
	}
	
	sleep (1);
	printf ("%s\n", gmi_gdb_show(mHandle, "v"));
	printf ("%s\n", gmi_gdb_show(mHandle, "str"));
	stack_Info ();
}

void GnuDebugger::prepare_startXterm ()
{
	int res = 0;
	mXterm = gmi_start_xterm();
	Q_ASSERT (mXterm);
	res = gmi_target_terminal(mHandle, mXterm->tty);
	Q_ASSERT (res != 0);
}

void GnuDebugger::exec_setCommandAndArgs (const QString& command, const QString& args)
{
	int res = 0;
	res = gmi_set_exec(mHandle, command.toLocal8Bit(), args.toLocal8Bit());
	printf ("res = %d\n", res);
	Q_ASSERT (res != 0);
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
	mi_wp *wp = NULL;
	bk = gmi_break_insert(mHandle, file.toLocal8Bit(), line);
	Q_ASSERT (bk);
	mi_free_bkpt(bk);

	wp = gmi_break_watch (mHandle, wm_write, "v");
	Q_ASSERT (wp);
	mi_free_wp (wp);
}

void GnuDebugger::stack_Info ()
{
	mi_frames * frames = NULL;
	frames = gmi_stack_info_frame (mHandle);
	Q_ASSERT (frames);
	emit positionChanged (frames[0].file, frames[0].line);
	mi_free_frames(frames);
}
