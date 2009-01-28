#ifndef QGDBCALLSTACK_H
#define QGDBCALLSTACK_H

#include "QGdb.h"

#include <QMetaType>

namespace QGdb
{
	struct CallStackFrameArgument
	{
		CallStackFrameArgument();
		
		/*** For dynamic memory management (can be removed if it isn't necessary) ***/
		CallStackFrameArgument( const CallStackFrameArgument& other );
		CallStackFrameArgument& operator=( const CallStackFrameArgument& other );
		bool operator==( const CallStackFrameArgument& other ) const;
		bool operator!=( const CallStackFrameArgument& other ) const;
		/*** End of dynamic memory management ***/
		
		// properties
		QString name; // argument name
		QString value; // argument value
	};
	typedef QList<CallStackFrameArgument> CallStackFrameArgumentList;
	
	struct CallStackFrame
	{
		CallStackFrame();
		
		/*** For dynamic memory management (can be removed if it isn't necessary) ***/
		CallStackFrame( const CallStackFrame& other );
		CallStackFrame& operator=( const CallStackFrame& other );
		bool operator==( const CallStackFrame& other ) const;
		bool operator!=( const CallStackFrame& other ) const;
		/*** End of dynamic memory management ***/
		
		// properties
		int level; // frame level
		QString function; // frame function
		QString file; // frame file
		QString from; // ?
		QString full; // ?
		int line; // function line
		int thread_id; // frame thread id
		CallStackFrameArgumentList arguments; // function arguments
	};
	typedef QList<CallStackFrame> CallStackFrameList;
};

Q_DECLARE_METATYPE( QGdb::CallStackFrameArgument )
Q_DECLARE_METATYPE( QGdb::CallStackFrameArgumentList )

Q_DECLARE_METATYPE( QGdb::CallStackFrame )
Q_DECLARE_METATYPE( QGdb::CallStackFrameList )

#endif // QGDBCALLSTACK_H
	