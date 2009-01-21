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
	
	connect( this, SIGNAL( stopped() ), this, SLOT( onStopped() ) );
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

void QGdbDriver::stack_Info()
{
	mi_frames* frames = gmi_stack_list_frames( mHandle );
	
	if ( !frames )
	{
		return;
	}
	
	mi_frames* arguments = gmi_stack_list_arguments( mHandle, 1 );
	
	if ( !arguments )
	{
		return;
	}
	
	CallStack stack;
	
	while ( frames || arguments )
	{
		Frame frame;
		frame.function = frames->func;
		frame.file = filePath( frames->file );
		frame.line = frames->line;
		frame.level = frames->level;
		
		qWarning( "from: %s", frames->from );
		
		mi_results* arg = arguments->args;
		
		while ( arg )
		{
			FunctionArgument argument;
			argument.name = arg->v.rs->v.cstr;
			argument.value = arg->v.rs->next->v.cstr;
			frame.arguments << argument;
			
			arg = arg->next;
		}
		
		stack << frame;
		
		if ( frames )
		{
			frames = frames->next;
		}
		
		if ( arguments )
		{
			arguments = arguments->next;
		}
	}
	
	if ( !stack.isEmpty() )
	{
		emit positionChanged( stack.first().file, stack.first().line );
	}
	
	emit callStackUpdate( stack );
	
	mi_free_frames( frames );
	mi_free_frames( arguments );
}

void QGdbDriver::onGdbTouchTimerTick ()
{
	if ( mi_get_response( mHandle ) )
	{
		log( "Async response in queue" );
	}
}

void QGdbDriver::setState( QGdbDriver::State state )
{
	mState = state;
	emit stateChanged( mState );
}

void QGdbDriver::onStopped()
{
	stack_Info();
}
