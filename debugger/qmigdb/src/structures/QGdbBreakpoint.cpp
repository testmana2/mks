#include "QGdbBreakpoint.h"

QGdb::Breakpoint::Breakpoint( mi_bkpt* bp )
{
	if ( bp )
	{
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
	else
	{
		number = -1; // mean target not yet specified, will be created at target setted
		type = t_breakpoint;
		disp = d_keep;
		enabled = true;
		addr = 0;
		function.clear();
		file.clear();
		line = -1;
		ignore = false;
		times = 0;
		condition.clear();
		absolute_file.clear();
		thread = -1;
		mode = m_file_line;
	}
}

QString QGdb::Breakpoint::stringType() const
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

QString QGdb::Breakpoint::stringDisp() const
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

QString QGdb::Breakpoint::stringMode() const
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

QString QGdb::Breakpoint::textMode() const
{
	const QString tFile = file.isEmpty() ? absolute_file : file;
	bool functionEmpty = function.isEmpty();
	bool fileEmpty = tFile.isEmpty();
	QString text;
	
	if ( !functionEmpty && !fileEmpty )
	{
		text = tr( "%1 in %2" ).arg( function ).arg( tFile );
	}
	else if ( !functionEmpty )
	{
		text = function;
	}
	else if ( !fileEmpty )
	{
		text = tFile;
	}
	
	text += QString( " (%1)" ).arg( (quintptr)addr );
	
	return text.trimmed();
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
QGdb::Breakpoint::Breakpoint( const QGdb::Breakpoint& other )
{
	operator=( other );
}

QGdb::Breakpoint& QGdb::Breakpoint::operator=( const QGdb::Breakpoint& other )
{
	if( *this != other )
	{
		number = other.number;
		type = other.type;
		disp = other.disp;
		enabled = other.enabled;
		addr = other.addr;
		function = other.function;
		file = other.file;
		line = other.line;
		ignore = other.ignore;
		times = other.times;
		condition = other.condition;
		absolute_file = other.absolute_file;
		thread = other.thread;
		mode = other.mode;
	}

	return *this;
}

bool QGdb::Breakpoint::operator==( const QGdb::Breakpoint& other ) const
{
	return number == other.number && type == other.type && disp == other.disp && enabled == other.enabled &&
		addr == other.addr && function == other.function && file == other.file && line == other.line &&
		ignore == other.ignore && times == other.times && condition == other.condition &&
		absolute_file == other.absolute_file && thread == other.thread && mode == other.mode;
}

bool QGdb::Breakpoint::operator!=( const QGdb::Breakpoint& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/
