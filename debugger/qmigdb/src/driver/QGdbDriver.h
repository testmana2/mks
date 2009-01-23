#ifndef QGDBDRIVER_H
#define QGDBDRIVER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>
#include <QMetaType>

#include "mi_gdb.h"

class QGdbDriver : public QObject
{
	Q_OBJECT
	
public:
	enum CBType
	{
		CONSOLE = 0,
		TARGET,
		LOG,
		TO_GDB,
		FROM_GDB,
		ASYNC
	};
	
	enum State
	{
		CONNECTED = 0,
		DISCONNECTED,
		TARGET_SETTED,
		RUNNING,
		STOPPED
	};
 
	struct Breakpoint
	{
		Breakpoint( mi_bkpt* bp = 0 )
		{
			if ( !bp )
			{
				return;
			}
			
			number = bp->number;
			type = bp->type;
			disp = bp->disp;
			enabled = bp->enabled;
			addr = bp->addr;
			function = bp->func;
			file = bp->file;
			line = bp->line;
			ignore = bp->ignore;
			times = bp->times;
			condition = bp->cond;
			absolute_file = bp->file_abs;
			thread = bp->thread;
			mode = bp->mode;
		}
		
		QString stringType() const
		{
			switch ( type )
			{
				case t_unknown:
					return tr( "Unknown" );
					break;
				case t_breakpoint:
					return tr( "Breakpoint" );
					break;
				case t_hw:
					return tr( "hw ?" );
					break;
			}
			
			return QString::null;
		}
		
		QString stringDisp() const
		{
			switch ( disp )
			{
				case d_unknown:
					return tr( "Unknown" );
					break;
				case d_keep:
					return tr( "Keep" );
					break;
				case d_del:
					return tr( "Delete" );
					break;
			}
			
			return QString::null;
		}
		
		QString stringMode() const
		{
			switch ( mode )
			{
				case m_file_line:
					return tr( "File line" );
					break;
				case m_function:
					return tr( "Function" );
					break;
				case m_file_function:
					return tr( "File function" );
					break;
				case m_address:
					return tr( "Address" );
					break;
			}
			
			return QString::null;
		}
		
		QString textMode() const
		{
			bool functionEmpty = function.isEmpty();
			bool fileEmpty = file.isEmpty();
			QString text;
			
			if ( !functionEmpty && !fileEmpty )
			{
				text = tr( "%1 in %2" ).arg( function ).arg( file );
			}
			else if ( !functionEmpty )
			{
				text = function;
			}
			else if ( !fileEmpty )
			{
				text = file;
			}
			
			text += QString( " (%1)" ).arg( (quintptr)addr );
			
			return text.trimmed();
		}

		int number;
		mi_bkp_type type;
		mi_bkp_disp disp;
		bool enabled;
		void* addr;
		QString function;
		QString file;
		int line;
		bool ignore;
		int times;
		QString condition;
		QString absolute_file;
		int thread;
		mi_bkp_mode mode;
	};
	typedef QList<Breakpoint> BreakpointsList;
	
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
		QString from;
		QString full;
		int line;
		int thread_id;
		ArgumentsList arguments;
	};
	typedef QList<Frame> CallStack;
	
	QGdbDriver();
	virtual ~QGdbDriver();
	
	QString filePath( const QString& fileName ) const;

public slots:
	void log( const QString& msg );
	// preparing to execution
	void prepare_startXterm ();
	
	// target execution
	
	void exec_setCommand (const QString& command);
#if 0
	void exec_setArgs (const QString& args);
#endif
	void exec_run();
	void exec_continue();
	void exec_stepInto();
	void exec_stepOver();
	void exec_stepOut();
	void exec_pause();
	void exec_kill();
	
	// breakpoints
	void break_setBreakpoint( const QString& file, int line );
	void break_breakpointToggled( const QString& file, int line );
	
protected:
	State mState;
	mi_h* mHandle;
	mi_aux_term* mXterm;
	QTimer mGdbPingTimer;
	QString mTargetFileName;
	
	// migdb callbacks
	static void callbackConsole (const char *, void *);
	static void callbackTarget(const char *str, void *);
	static void callbackLog (const char *str, void *);
	static void callbackToGDB (const char *str, void *);
	static void callbackFromGDB (const char *str, void *);
	static void callbackAsync (mi_output * o, void * debuggerInstance);
	
	void setState (State);
protected slots:
	// touches gdb for make it alive
	// Without it we haven't callbacks
	void onGdbTouchTimerTick();
	void getCallStack( mi_stop* stop );
	
signals:
	void callbackMessage( const QString& message, QGdbDriver::CBType type );
	void stateChanged( QGdbDriver::State state );
	void breakpointAdded( const QGdbDriver::Breakpoint& breakpoint );
	
	void callStackUpdated( const QGdbDriver::CallStack& stack );
	void positionChanged (const QString& fileName, int line); // should be renamed ?
	void exited( int code );
};

Q_DECLARE_METATYPE( QGdbDriver::Breakpoint )
Q_DECLARE_METATYPE( QGdbDriver::Frame )

#endif // QGDBDRIVER_H
