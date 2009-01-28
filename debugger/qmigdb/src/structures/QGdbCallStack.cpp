#include "QGdbCallStack.h"

// CALLSTACKARGUMENT

QGdb::CallStackFrameArgument::CallStackFrameArgument()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
QGdb::CallStackFrameArgument::CallStackFrameArgument( const QGdb::CallStackFrameArgument& other )
{
	operator=( other );
}

QGdb::CallStackFrameArgument& QGdb::CallStackFrameArgument::operator=( const QGdb::CallStackFrameArgument& other )
{
	if( *this != other )
	{
		name = other.name;
		value = other.value;
	}

	return *this;
}

bool QGdb::CallStackFrameArgument::operator==( const QGdb::CallStackFrameArgument& other ) const
{
	return name == other.name && value == other.value;
}

bool QGdb::CallStackFrameArgument::operator!=( const QGdb::CallStackFrameArgument& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/

// CALLSTACK

QGdb::CallStackFrame::CallStackFrame()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
QGdb::CallStackFrame::CallStackFrame( const QGdb::CallStackFrame& other )
{
	operator=( other );
}

QGdb::CallStackFrame& QGdb::CallStackFrame::operator=( const QGdb::CallStackFrame& other )
{
	if( *this != other )
	{
		level = other.level;
		function = other.function;
		file = other.file;
		from = other.from;
		full = other.full;
		line = other.line;
		thread_id = other.thread_id;
		arguments = other.arguments;
	}

	return *this;
}

bool QGdb::CallStackFrame::operator==( const QGdb::CallStackFrame& other ) const
{
	return level == other.level && function == other.function &&
		file == other.file && from == other.from &&
		full == other.full && line == other.line &&
		thread_id == other.thread_id && arguments == other.arguments;
}

bool QGdb::CallStackFrame::operator!=( const QGdb::CallStackFrame& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/
