#include "QGdb-Driver.h"

#include <QTimer>
#include <QDir>
#include <QFileInfo>

QGdb::Driver::Driver( QObject* parent )
	: QObject( parent )
{
	mHandle = 0;
	mState = QGdb::DISCONNECTED;
	mWaitingTempBreakpoint = false;
	
	QGdb::registerStructures();
	
	mAsyncPollTimer = new QTimer( this );
	
	connect( mAsyncPollTimer, SIGNAL( timeout() ), this, SLOT( asyncPollTimer_timeout() ) );
	
	mAsyncPollTimer->start( 50 );
}

QGdb::Driver::~Driver()
{
	if ( mState != QGdb::DISCONNECTED )
	{
		disconnectFromGdb();
	}
}

// Callbacks of debugger
void QGdb::Driver::callbackConsole( const char* str, void* instance )
{
	QGdb::Driver* driver = static_cast<QGdb::Driver*>( instance );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdb::CONSOLE );
}

// Note that unlike what's documented in gdb docs it isn't usable.
void QGdb::Driver::callbackTarget( const char* str, void* instance )
{
	QGdb::Driver* driver = static_cast<QGdb::Driver*>( instance );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdb::TARGET );
}

void QGdb::Driver::callbackLog( const char* str, void* instance )
{
	QGdb::Driver* driver = static_cast<QGdb::Driver*>( instance );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdb::LOG );
}

void QGdb::Driver::callbackToGDB( const char* str, void* instance )
{
	QGdb::Driver* driver = static_cast<QGdb::Driver*>( instance );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdb::TO_GDB );
}

void QGdb::Driver::callbackFromGDB( const char* str, void* instance )
{
	QGdb::Driver* driver = static_cast<QGdb::Driver*>( instance );
	emit driver->callbackMessage( QString::fromLocal8Bit( str ), QGdb::FROM_GDB );
}

void QGdb::Driver::callbackAsync( mi_output* output, void* instance )
{
	QGdb::Driver* driver = static_cast<QGdb::Driver*>( instance );
	emit driver->callbackMessage( output->c->v.cstr, QGdb::ASYNC );
}

void QGdb::Driver::handleStop( mi_stop* stop )
{
	if ( stop )
	{
		if ( stop->reason == sr_unknown && mWaitingTempBreakpoint )
		{
			mWaitingTempBreakpoint = false;
			stop->reason = sr_bkpt_hit;
		}
		
		if ( stop->reason == sr_exited_signalled || stop->reason == sr_exited || stop->reason == sr_exited_normally )
		{
			setState( QGdb::TARGET_SETTED );
		}
		else
		{
			setState( QGdb::STOPPED );
		}
		
		// thread callstack
		if ( stop->have_thread_id && stop->reason != sr_exited )
		{
			stack_listFrames();
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
				emit exitSignalReceived( QGdb::Signal( stop ) );
				break;
			case sr_exited:
				emit exited( stop->exit_code );
				break;
			case sr_exited_normally:
				break;
			case sr_signal_received:
				emit signalReceived( QGdb::Signal( stop ) );
				//delayedCall( SLOT( updateFullCallStack() ) );
				break;
			case sr_unknown:
			default:
				break;
		}
	}
	else
	{
		if ( mState == QGdb::RUNNING )
		{
			setState( QGdb::STOPPED );
		}
	}
}

void QGdb::Driver::asyncPollTimer_timeout()
{
	if ( mState == QGdb::RUNNING )
	{
		if ( mi_get_response( mHandle ) )
		{
			appendLog( "Async response received" );
			
			mi_output* output = mi_retire_response( mHandle );
			
			switch ( output->tclass )
			{
				case MI_CL_STOPPED:
				{
					appendLog( "Async response MI_CL_STOPPED" );
					
					mi_stop* stop = mi_get_stopped( output->c );
					handleStop( stop );
					mi_free_stop( stop );
					break;
				}
				case MI_CL_DOWNLOAD:
					appendLog( "Async response MI_CL_DOWNLOAD" );
					break;
				case MI_CL_UNKNOWN:
					appendLog( "Async response MI_CL_UNKNOWN" );
					break;
				/*
				case MI_CL_DONE:
				case MI_CL_RUNNING:
				case MI_CL_CONNECTED:
				case MI_CL_ERROR:
				case MI_CL_EXIT:
				*/
				default:
					appendLog( "Async response NO_ASYNC_CLASS" );
					break;
			}
		}
	}
}

void QGdb::Driver::setState( QGdb::State state )
{
	mState = state;
	emit stateChanged( mState );
}

bool QGdb::Driver::runToMain()
{
	if ( mState != QGdb::STOPPED && mState != QGdb::TARGET_SETTED )
	{
		return false;
	}
	
	mi_bkpt* bp = gmi_break_insert_full( mHandle, true, false, 0, -1, -1, mi_get_main_func() );
	
	if ( !bp )
	{
		return false;
	}
	
	mi_free_bkpt( bp );
	
	mWaitingTempBreakpoint = true;
	
	return gmi_exec_run( mHandle ) != 0;
}

QString QGdb::Driver::filePath( const QString& fileName ) const
{
#warning seem not correct the binary path is not always in root sources dir, need fix that
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

void QGdb::Driver::generateCallStack( mi_frames* mframe )
{
	QGdb::CallStackFrameList stack;
	int level = -1;
	
	while ( mframe )
	{
		QGdb::CallStackFrame frame;
		frame.function = mframe->func;
		frame.file = mframe->file;
		frame.from = mframe->from;
		frame.full = filePath( mframe->file );
		frame.line = mframe->line;
		frame.level = mframe->level;
		
		mi_results* arg = mframe->args;
		
		while ( arg )
		{
			QGdb::CallStackFrameArgument argument;
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

void QGdb::Driver::generateCallStack( mi_stop* stop )
{
	generateCallStack( stop->frame );
}

bool QGdb::Driver::connectToGdb()
{
	if ( mHandle )
	{
		return true;
	}
	
	mHandle	= mi_connect_local();
	
	if ( mHandle )
	{
		mi_set_console_cb( mHandle, callbackConsole, this );
		mi_set_target_cb( mHandle, callbackTarget, this );
		mi_set_log_cb( mHandle, callbackLog, this );
		mi_set_async_cb( mHandle, callbackAsync, this );
		mi_set_to_gdb_cb( mHandle, callbackToGDB, this );
		mi_set_from_gdb_cb( mHandle, callbackFromGDB, this );
		
		setState( QGdb::CONNECTED );
	}
	
	return mHandle;
}

void QGdb::Driver::disconnectFromGdb()
{
	if ( mState != QGdb::DISCONNECTED )
	{
		gmi_gdb_exit( mHandle );
		mi_disconnect( mHandle );
	}
}

QString QGdb::Driver::lastError() const
{
	return QString::fromLocal8Bit( mi_get_error_str() );
}

void QGdb::Driver::appendLog( const QString& msg )
{
	emit callbackMessage( msg, QGdb::LOG );
}

bool QGdb::Driver::exec_setExecutable( const QString& executable )
{
	if ( mState != QGdb::CONNECTED )
	{
		return false;
	}
	
	int res = gmi_set_exec( mHandle, executable.toLocal8Bit().constData(), 0 );
	
	if ( res != 0 )
	{
		mTargetFileName = executable;
		setState( QGdb::TARGET_SETTED );
	}
	
	return res != 0;
}

bool QGdb::Driver::exec_run()
{
	if ( runToMain() )
	{
		setState( QGdb::RUNNING );
		return true;
	}
	
	return false;
}

bool QGdb::Driver::exec_continue()
{
	if ( mState != QGdb::STOPPED )
	{
		return false;
	}
	
	int res = gmi_exec_continue( mHandle );
	
	if ( res != 0 )
	{
		setState( QGdb::RUNNING );
		return true;
	}
	
	return false;
}

bool QGdb::Driver::exec_stepInto( bool instruction )
{
	int res = 0;
	
	if ( mState == QGdb::TARGET_SETTED )
	{
		return runToMain();
	}
	
	if ( mState == QGdb::STOPPED )
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
			setState( QGdb::RUNNING );
			return true;
		}
	}
	
	return false;
}

bool QGdb::Driver::exec_stepOver( bool instruction )
{
	int res = 0;

	if ( mState == QGdb::TARGET_SETTED )
	{
		return runToMain();
	}
	
	if ( mState == QGdb::STOPPED )
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
			setState( QGdb::RUNNING );
			return true;
		}
	}
	
	return false;
}

bool QGdb::Driver::exec_stepOut()
{
	if ( mState != QGdb::STOPPED )
	{
		return false;
	}
	
	int res = gmi_exec_finish( mHandle );
	
	if ( res != 0 )
	{
		setState( QGdb::RUNNING );
		return true;
	}
	
	return false;
}

bool QGdb::Driver::exec_stop()
{
	if ( mState != QGdb::RUNNING )
	{
		return false;
	}
	
	return gmi_exec_interrupt( mHandle ) != 0;
}

bool QGdb::Driver::exec_kill()
{
	if ( mState != QGdb::STOPPED && mState != QGdb::RUNNING )
	{
		return false;
	}
	
	// GDB/MI doesn't implement it (yet), so we use the regular kill.
	// Ensure confirm is off.
	char* prev = gmi_gdb_show( mHandle, "confirm" );
	
	if ( !prev )
	{
		return false;
	}
	
	if ( strcmp( prev, "off" ) )
	{
		if ( !gmi_gdb_set( mHandle, "confirm", "off" ) )
		{
			free( prev );
			return false;
		}
	}
	else
	{
		free( prev );
		prev = false;
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
		setState( QGdb::TARGET_SETTED );
		return true;
	}
	
	return false;
}

bool QGdb::Driver::stack_listFrames()
{
	if ( mState != QGdb::STOPPED )
	{
		return false;
	}
	
	mi_frames* frame = gmi_stack_list_frames( mHandle );
	
	if ( frame )
	{
		mi_frames* args = gmi_stack_list_arguments( mHandle, 1 );
		
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
	return true;
}











/*




QStringList QGdb::Driver::sourcesPath() const // need fix, it allow to found sources path gdb look for when searching a file.
{
	char* tmp = gmi_gdb_show( mHandle, "directories" );
	QStringList directories = QString::fromLocal8Bit( tmp ).remove( "\\n" ).split( ":" );
	
	qWarning() << "test" << tmp;
	
	if ( tmp )
	{
		free( tmp );
	}
	
	return directories;
}

void QGdb::Driver::prepare_startXterm ()
{
	int res = 0;
	mXterm = gmi_start_xterm();
	Q_ASSERT (mXterm);
	res = gmi_target_terminal(mHandle, mXterm->tty);
	Q_ASSERT (res != 0);
}

void QGdb::Driver::exec_setArgs( const QString& args )
{
	if ( mState != QGdb::Driver::TARGET_SETTED )
	{
		return 0;
	}
	
	int res = gmi_set_exec( mHandle, mTargetFileName.toLocal8Bit(), args.toLocal8Bit() );
	Q_ASSERT (res != 0);
}







int QGdb::Driver::exec_stepInto( bool instruction ) // ok
{
	int res = 0;
	
	if ( mState == QGdb::TARGET_SETTED )
	{
		return runToMain();
	}
	
	if ( mState == QGdb::STOPPED )
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
			setState( QGdb::RUNNING );
		}
	}
	
	return res;
}

int QGdb::Driver::exec_stepOver( bool instruction ) // ok
{
	int res = 0;

	if ( mState == QGdb::TARGET_SETTED )
	{
		return runToMain();
	}
	
	if ( mState == QGdb::STOPPED )
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
			setState( QGdb::RUNNING );
		}
	}
	
	return res;
}

int QGdb::Driver::exec_stepOut() // ok
{
	if ( mState != QGdb::STOPPED )
	{
		return 0;
	}
	
	int res = gmi_exec_finish( mHandle );
	
	if ( res != 0 )
	{
		setState( QGdb::RUNNING );
	}
	
	return res;
}

int QGdb::Driver::exec_pause() // ok
{
	if ( mState != QGdb::RUNNING )
	{
		return 0;
	}
	
	return gmi_exec_interrupt( mHandle );
}

int QGdb::Driver::exec_kill() // ok
{
	if ( mState != QGdb::STOPPED && mState != QGdb::RUNNING )
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
		setState( QGdb::TARGET_SETTED );
	}
	
	return res;
}

void QGdb::Driver::stack_selectFrame (int frame_num)
{
	if (mState != QGdb::STOPPED)
		return;
	
	int res = 0;
	res = gmi_stack_select_frame(mHandle, frame_num);
	Q_ASSERT (res);
	
	// NOTE hlamer: It's better to use mi_frames* frame = gmi_stack_info_frame (mHandle);, but function doesn't work
	mi_frames* frame = gmi_stack_list_frames (mHandle);
	mi_frames* first_frame = frame;
	Q_ASSERT (frame);
	for (int i = 0; i < frame_num; i++)
	{
		Q_ASSERT (frame);
		if (!frame)
			break;
		frame = frame->next;
	}
	
	Q_ASSERT (frame);
	if (frame)
		emit positionChanged(filePath(frame->file), frame->line );
	
	mi_free_frames (first_frame);
}

int QGdb::Driver::break_setBreakpoint( const QString& file, int line )
{
	int res = 0;
	QGdb::Breakpoint bp;
	
	if ( mState != QGdb::STOPPED && mState != QGdb::TARGET_SETTED )
	{
		bp.absolute_file = file;
	}
	else
	{
		mi_bkpt* mbp = gmi_break_insert( mHandle, file.toLocal8Bit(), line );
		
		if ( !mbp )
		{
			return res;
		}
		
		bp = QGdb::Breakpoint( mbp );
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
	
	return 1;
}

void QGdb::Driver::break_breakpointToggled( const QString& file, int line )
{
	for ( int i = 0; i < mBreakpoints.count(); i++ )
	{
		QGdb::Breakpoint& bp = mBreakpoints[ i ];
		
		if ( bp.absolute_file == file && bp.line == line )
		{
			if ( ( bp.number == -1 ) || ( mState == QGdb::TARGET_SETTED || mState == QGdb::RUNNING ) )
			{
				int res = bp.number == -1 ? 1 : gmi_break_delete( mHandle, bp.number );
				
				if ( res != 0 )
				{
					emit breakpointRemoved( bp );
					mBreakpoints.removeAt( i );
				}
			}
			
			return;
		}
	}
	
	break_setBreakpoint( file, line );
}

void QGdb::Driver::clearBreakpoints( int line )
{
	foreach ( const QGdb::Breakpoint& bp, mBreakpoints )
	{
		if ( bp.number != -1 && ( bp.line == line || line == -1 ) )
		{
			int res = gmi_break_delete( mHandle, bp.number );
			
			Q_ASSERT( res );
			
			if ( line != -1 )
			{
				emit breakpointRemoved( bp );
			}
			
			mBreakpoints.removeAll( bp );
		}
		else if ( bp.line == line || line == -1 )
		{
			if ( line != -1 )
			{
				emit breakpointRemoved( bp );
			}
			
			mBreakpoints.removeAll( bp );
		}
	}
	
	if ( mBreakpoints.isEmpty() )
	{
		emit breakpointsCleared();
	}
}

void QGdb::Driver::onGdbTouchTimerTick ()
{
	if ( mi_get_response( mHandle ) )
	{
		log( "Async response in queue" );
	}
}

void QGdb::Driver::sendFakeBreakpoints()
{
	QGdb::BreakpointList bps = mBreakpoints;
	
	clearBreakpoints();
	
	foreach ( const QGdb::Breakpoint& bp, bps )
	{
		if ( break_setBreakpoint( bp.absolute_file, bp.line ) == 0 )
		{
			QMessageBox::critical( 0, tr( "Setting breakpoint error..." ), QString::fromLocal8Bit( mi_get_error_str() ) );
		}
	}
	
#warning	ajouter pastille ici, possibilité de revoir methode add breakpoints
}





void QGdb::Driver::delayedCall( const char* member )
{
	QTimer::singleShot( 50, this, member );
}
*/
