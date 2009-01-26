#include <QMetaType>
#include <QMessageBox>
#include <QDir>

#include "QGdbDriver.h"
#include <QDebug>

int QGdbDriver::Breakpoint::fakeAutoNumber = 0;

// Callbacks of debugger
void QGdbDriver::callbackConsole( const char* str, void* data )
{
	QGdbDriver* driver = static_cast<QGdbDriver*>( data );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdbDriver::CONSOLE );
}

/* Note that unlike what's documented in gdb docs it isn't usable. */
void QGdbDriver::callbackTarget( const char* str, void* data )
{
	QGdbDriver* driver = static_cast<QGdbDriver*>( data );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdbDriver::TARGET );
}

void QGdbDriver::callbackLog( const char* str, void* data )
{
	QGdbDriver* driver = static_cast<QGdbDriver*>( data );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdbDriver::LOG );
}

void QGdbDriver::callbackToGDB( const char* str, void* data )
{
	QGdbDriver* driver = static_cast<QGdbDriver*>( data );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdbDriver::TO_GDB );
}

void QGdbDriver::callbackFromGDB( const char* str, void* data )
{
	QGdbDriver* driver = static_cast<QGdbDriver*>( data );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdbDriver::FROM_GDB );
}

void QGdbDriver::callbackAsync( mi_output* output, void* data )
{
	QGdbDriver* driver = static_cast<QGdbDriver*>( data );
	
	emit driver->callbackMessage( output->c->v.cstr, QGdbDriver::ASYNC );
	
	if ( output->tclass == MI_CL_STOPPED )
	{
		mi_stop* stop = mi_get_stopped( output->c );
		
		if ( stop )
		{
			if ( stop->reason == sr_unknown && driver->mWaitingTempBreakpoint )
			{
				driver->mWaitingTempBreakpoint = false;
				stop->reason = sr_bkpt_hit;
			}
			
			if ( stop->reason == sr_exited_signalled || stop->reason == sr_exited || stop->reason == sr_exited_normally )
			{
				driver->setState( QGdbDriver::TARGET_SETTED );
			}
			else
			{
				driver->setState( QGdbDriver::STOPPED );
			}
			
			// thread callstack
			if ( stop->have_thread_id && stop->reason != sr_exited )
			{
				driver->updateCallStack( stop );
			}
			/*
			char have_thread_id;
			char have_bkptno;
			char have_exit_code;
			char have_wpno;
			*/
			
			switch ( stop->reason )
			{
				case sr_bkpt_hit:
					break;
				case sr_wp_trigger:
					break;
				case sr_read_wp_trigger:
					break;
				case sr_access_wp_trigger:
					break;
				case sr_wp_scope:
					break;
				case sr_function_finished:
					break;
				case sr_location_reached:
					break;
				case sr_end_stepping_range:
					break;
				case sr_exited_signalled:
					emit driver->exitSignalReceived( Signal( stop ) );
					break;
				case sr_exited:
					emit driver->exited( stop->exit_code );
					break;
				case sr_exited_normally:
					break;
				case sr_signal_received:
					emit driver->signalReceived( Signal( stop ) );
					driver->delayedCall( SLOT( updateFullCallStack() ) );
					break;
				case sr_unknown:
				default:
					break;
			}
			
			mi_free_stop( stop );
		}
		else
		{
			if ( driver->mState == QGdbDriver::RUNNING )
			{
				driver->setState( QGdbDriver::STOPPED );
			}
		}
	}
}

QGdbDriver::QGdbDriver()
	: QObject()
{
	mState = QGdbDriver::DISCONNECTED;
	mHandle = 0;
	mWaitingTempBreakpoint = false;
	
	mHandle	= mi_connect_local();
	
	Q_ASSERT( mHandle ); // FIXME replace with some error message
	
	setState( QGdbDriver::CONNECTED );
	
	mi_set_console_cb( mHandle, callbackConsole, this );
	mi_set_target_cb( mHandle, callbackTarget, this );
	mi_set_log_cb( mHandle, callbackLog, this );
	mi_set_async_cb( mHandle, callbackAsync, this );
	mi_set_to_gdb_cb( mHandle, callbackToGDB, this );
	mi_set_from_gdb_cb( mHandle, callbackFromGDB, this );
	
	connect( &mGdbPingTimer, SIGNAL( timeout() ), this, SLOT( onGdbTouchTimerTick() ) );
	
	qRegisterMetaType<QGdbDriver::CallStack>( "QGdbDriver::CallStack" );
	qRegisterMetaType<QGdbDriver::CallStack>( "QGdbDriver::State" );
	
	mGdbPingTimer.setInterval( 50 );
	mGdbPingTimer.start();
}

QGdbDriver::~QGdbDriver()
{
	gmi_gdb_exit(mHandle);
	mi_disconnect(mHandle);
}

QString QGdbDriver::filePath( const QString& fileName ) const
{
	if ( mTargetFileName.isEmpty() )
	{
		return fileName;
	}
	
	QDir dir( QFileInfo( mTargetFileName ).absolutePath() );
	QFileInfo fi( fileName );
	
	if ( fi.isRelative() )
	{
		return QDir::toNativeSeparators( QDir::cleanPath( dir.absoluteFilePath( fileName ) ) );
	}
	
	return QDir::toNativeSeparators( fileName );
}

void QGdbDriver::log( const QString& msg )
{
	emit callbackMessage( msg, QGdbDriver::LOG );
}

void QGdbDriver::prepare_startXterm ()
{
	int res = 0;
	mXterm = gmi_start_xterm();
	Q_ASSERT (mXterm);
	res = gmi_target_terminal(mHandle, mXterm->tty);
	Q_ASSERT (res != 0);
}

void QGdbDriver::exec_setCommand( const QString& command )
{
	int res = 0;
	res = gmi_set_exec( mHandle, command.toLocal8Bit().constData(), 0 );
	
	if ( res != 0 )
	{
		mTargetFileName = command;
		setState( QGdbDriver::TARGET_SETTED );
	}
	else
	{
		QMessageBox::critical( NULL, tr( "Failed to load target" ), tr( "GDB error: " ) +mi_get_error_str() );
	}
}

#if 0
	void QGdbDriver::exec_setArgs (const QString& args)
	{
		int res = 0;
		res = gmi_set_exec(mHandle, command.toLocal8Bit(), args.toLocal8Bit());
		Q_ASSERT (res != 0);
	}
#endif

int QGdbDriver::runToMain() // ok, need maybe rewrite ( breakpoint )
{
	if ( mState != QGdbDriver::STOPPED && mState != QGdbDriver::TARGET_SETTED )
	{
		return 0;
	}
	
	mi_bkpt* bp = gmi_break_insert_full( mHandle, true, false, 0, -1, -1, mi_get_main_func() );
	
	if ( !bp )
	{
		return 0;
	}
	
	mi_free_bkpt( bp );
	
	mWaitingTempBreakpoint = true;
	
	return exec_run();
}

int QGdbDriver::exec_run() // ok
{
	if ( mState != QGdbDriver::TARGET_SETTED )
	{
		return 0;
	}
	
	int res = gmi_exec_run( mHandle );
	
	if ( res != 0 )
	{
		setState( QGdbDriver::RUNNING );
	}
	
	return res;
}

int QGdbDriver::exec_continue() // ok
{
	if ( mState != QGdbDriver::STOPPED )
	{
		return 0;
	}
	
	int res = gmi_exec_continue( mHandle );
	
	if ( res != 0 )
	{
		setState( QGdbDriver::RUNNING );
	}
	
	return res;
}

int QGdbDriver::exec_stepInto( bool instruction ) // ok
{
	int res = 0;
	
	if ( mState == QGdbDriver::TARGET_SETTED )
	{
		return runToMain();
	}
	
	if ( mState == QGdbDriver::STOPPED )
	{
		if ( instruction )
		{
			res = gmi_exec_step_instruction( mHandle );
		}
		else
		{
			res = gmi_exec_step( mHandle );
		}
		
		if ( res != 0 )
		{
			setState( QGdbDriver::RUNNING );
		}
	}
	
	return res;
}

int QGdbDriver::exec_stepOver( bool instruction ) // ok
{
	int res = 0;

	if ( mState == QGdbDriver::TARGET_SETTED )
	{
		return runToMain();
	}
	
	if ( mState == QGdbDriver::STOPPED )
	{
		if ( instruction )
		{
			res = gmi_exec_next_instruction( mHandle );
		}
		else
		{
			res = gmi_exec_next( mHandle );
		}
		
		if ( res != 0 )
		{
			setState( QGdbDriver::RUNNING );
		}
	}
	
	return res;
}

int QGdbDriver::exec_stepOut() // ok
{
	if ( mState != QGdbDriver::STOPPED )
	{
		return 0;
	}
	
	int res = gmi_exec_finish( mHandle );
	
	if ( res != 0 )
	{
		setState( QGdbDriver::RUNNING );
	}
	
	return res;
}

int QGdbDriver::exec_pause() // ok
{
	if ( mState != QGdbDriver::RUNNING )
	{
		return 0;
	}
	
	return gmi_exec_interrupt( mHandle );
}

int QGdbDriver::exec_kill() // ok
{
	if ( mState != QGdbDriver::STOPPED && mState != QGdbDriver::RUNNING )
	{
		return 0;
	}
	
	// GDB/MI doesn't implement it (yet), so we use the regular kill.
	// Ensure confirm is off.
	char* prev = gmi_gdb_show( mHandle, "confirm" );
	
	if ( !prev )
	{
		return 0;
	}
	
	if ( strcmp( prev, "off" ) )
	{
		if ( !gmi_gdb_set( mHandle, "confirm", "off" ) )
		{
			free( prev );
			return 0;
		}
	}
	else
	{
		free( prev );
		prev = 0;
	}
	
	// do real kill
	int res = gmi_exec_kill( mHandle );
	
	// Revert confirm option if needed.
	if ( prev )
	{
		gmi_gdb_set( mHandle, "confirm", prev );
		free( prev );
	}
	
	if ( res != 0 )
	{
		setState( QGdbDriver::TARGET_SETTED );
	}
	
	return res;
}

bool QGdbDriver::break_setBreakpoint( const QString& file, int line )
{
	Breakpoint bp;
	
	if ( mState != QGdbDriver::STOPPED && mState != QGdbDriver::TARGET_SETTED )
	{
		bp.absolute_file = file;
	}
	else
	{
		mi_bkpt* mbp = gmi_break_insert( mHandle, targetDirRelativeFileName.toLocal8Bit(), line );
		
		if ( !mbp )
		{
			return false;
		}
		
		bp = Breakpoint( mbp );
		mi_free_bkpt( mbp );
	}
	
	// the breakpoint structure have null file_asb :/
	if ( bp.absolute_file.isEmpty() )
	{
		bp.absolute_file = file;
	}
	
	// on multiple location, line is incorrect ( = 0 )
	if ( bp.line != line )
	{
		bp.line = line;
	}
	
	mBreakpoints << bp;
	emit breakpointAdded( bp );
	
	return true;
}

void QGdbDriver::break_breakpointToggled( const QString& file, int line, bool& remove )
{
	for ( int i = 0; i < mBreakpoints.count(); i++ )
	{
		Breakpoint& bp = mBreakpoints[ i ];
		
		if ( bp.absolute_file == file && bp.line == line )
		{
			if ( mState == QGdbDriver::TARGET_SETTED || mState == QGdbDriver::RUNNING )
			{
				int res = gmi_break_delete( mHandle, bp.number );
				
				if ( res != 0 )
				{
					emit breakpointRemoved( bp );
					mBreakpoints.removeAt( i );
					remove = true;
				}
			}
			else
			{
				remove = false;
			}
			
			return;
		}
	}
	
	remove = !break_setBreakpoint( file, line );
}

void QGdbDriver::onGdbTouchTimerTick ()
{
	if ( mi_get_response( mHandle ) )
	{
		log( "Async response in queue" );
	}
}

void QGdbDriver::generateCallStack( mi_frames* mframe )
{
	CallStack stack;
	int level = -1;
	
	while ( mframe )
	{
		Frame frame;
		frame.function = mframe->func;
		frame.file = mframe->file;
		frame.from = mframe->from;
		frame.full = filePath( mframe->file );
		frame.line = mframe->line;
		frame.level = mframe->level;
		
		mi_results* arg = mframe->args;
		
		while ( arg )
		{
			FunctionArgument argument;
			argument.name = arg->v.rs->v.cstr;
			argument.value = arg->v.rs->next->v.cstr;
			frame.arguments << argument;
			
			arg = arg->next;
		}
		
		if ( level == -1 && !frame.file.isEmpty() )
		{
			level = frame.level;
			emit positionChanged( frame.full, frame.line );
		}
		
		stack << frame;
		mframe = mframe->next;
	}
	
	emit callStackUpdated( stack, level );
}

void QGdbDriver::updateCallStack( mi_stop* stop )
{
	generateCallStack( stop->frame );
}

void QGdbDriver::updateFullCallStack()
{
	if ( mState != QGdbDriver::STOPPED )
	{
		return;
	}
	
	mi_frames* frame = gmi_stack_list_frames( mHandle );
	
	if ( frame )
	{
		mi_frames* args = gmi_stack_list_arguments( mHandle, 1  );
		
		if ( args )
		{
			mi_frames* p = frame;
			mi_frames* p2 = args;
			
			while ( p2 && p )
			{
				p->args = p2->args;
				p2->args = NULL;
				p2 = p2->next;
				p = p->next;
			}
			
			mi_free_frames( args );
		}
	}
	
	generateCallStack( frame );
	mi_free_frames( frame );
}

void QGdbDriver::setState( QGdbDriver::State state )
{
	mState = state;
	emit stateChanged( mState );
}

void QGdbDriver::delayedCall( const char* member )
{
	QTimer::singleShot( 50, this, member );
}
