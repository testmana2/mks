#include <QMetaType>
#include <QMessageBox>
#include <QDir>

#include "QGdbDriver.h"
#include <QDebug>

volatile int asyncCount = 0;

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
	
	emit driver->callbackMessage( QString::number( asyncCount ), QGdbDriver::ASYNC );
	
	if ( output->tclass == MI_CL_STOPPED )
	{
		mi_stop* stop = mi_get_stopped( output->c );
		
		if ( stop )
		{
			if ( stop->reason == sr_unknown /*&& waitingTempBkpt*/ )
			{
				//waitingTempBkpt = 0;
				stop->reason = sr_bkpt_hit;
			}
			
			switch ( stop->reason )
			{
				case sr_exited_signalled:
				case sr_exited:
				case sr_exited_normally:
					driver->setState( QGdbDriver::TARGET_SETTED );
					break;
				case sr_signal_received:
				case sr_bkpt_hit:
				case sr_end_stepping_range:
				case sr_function_finished:
				default:
					driver->setState( QGdbDriver::STOPPED );
					break;
			}
			
			// thread callstack
			if ( stop->have_thread_id && stop->reason != sr_exited )
			{
				driver->getCallStack( stop );
			}
			/*
			char have_thread_id;
			char have_bkptno;
			char have_exit_code;
			char have_wpno;
			*/
			
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
	
	asyncCount++;
}

QGdbDriver::QGdbDriver()
	: QObject()
{
	mState = QGdbDriver::DISCONNECTED;
	mHandle = 0;
	
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

void QGdbDriver::exec_run()
{
	int res = 0;
	res = gmi_exec_run( mHandle );
	Q_ASSERT( res );
	setState( QGdbDriver::RUNNING );
}

void QGdbDriver::exec_continue()
{
	int res = 0;
	res = gmi_exec_continue (mHandle);
	Q_ASSERT (res);
	setState( QGdbDriver::RUNNING );
}

void QGdbDriver::exec_stepInto()
{
	int res = 0;
	res = gmi_exec_step (mHandle);
	Q_ASSERT (res);
	setState( QGdbDriver::RUNNING );
}

void QGdbDriver::exec_stepOver()
{
	int res = 0;
	res = gmi_exec_next (mHandle);
	Q_ASSERT (res);
	setState( QGdbDriver::RUNNING );
}

void QGdbDriver::exec_stepOut()
{
	int res = 0;
	res = gmi_exec_finish (mHandle);
	Q_ASSERT (res);
	setState( QGdbDriver::RUNNING );
}

void QGdbDriver::exec_pause()
{
	int res = 0;
	res = gmi_exec_interrupt (mHandle);
	Q_ASSERT (res);
}

void QGdbDriver::exec_kill()
{
	int res = 0;
	res = gmi_exec_kill (mHandle);
	Q_ASSERT (res);
	setState( QGdbDriver::TARGET_SETTED );
}

void QGdbDriver::break_setBreaktoint (const QString& file, int line)
{
	mi_bkpt *bk = NULL;
	bk = gmi_break_insert(mHandle, file.toLocal8Bit(), line);
	Q_ASSERT (bk);
	mi_free_bkpt(bk);
}

void QGdbDriver::onGdbTouchTimerTick ()
{
	if ( mi_get_response( mHandle ) )
	{
		log( "Async response in queue" );
	}
}

void QGdbDriver::getCallStack( mi_stop* stop )
{
	CallStack stack;
	mi_frames* mframe = stop->frame;
	
	if ( !mframe )
	{
		emit callStackUpdated( stack );
		return;
	}
	
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
		
		stack << frame;
		
		mframe = mframe->next;
	}
	
	if ( !stack.isEmpty() )
	{
		emit positionChanged( stack.first().full, stack.first().line );
	}
	
	emit callStackUpdated( stack );
}

void QGdbDriver::setState( QGdbDriver::State state )
{
	mState = state;
	emit stateChanged( mState );
}
