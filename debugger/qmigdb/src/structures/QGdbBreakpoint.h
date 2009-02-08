#ifndef QGDBBREAKPOINT_H
#define QGDBBREAKPOINT_H

#include "QGdb.h"

#include <QMetaType>

namespace QGdb
{
	struct Breakpoint
	{
		Breakpoint( mi_bkpt* bp = 0 );
		
		QString stringType() const;
		QString stringDisp() const;
		QString stringMode() const;
		QString textMode() const;
		
		/*** For dynamic memory management (can be removed if it isn't necessary) ***/
		Breakpoint( const Breakpoint& other );
		Breakpoint& operator=( const Breakpoint& other );
		bool operator==( const Breakpoint& other ) const;
		bool operator!=( const Breakpoint& other ) const;
		/*** End of dynamic memory management ***/
		
		// properties
		int number; // breakpoint number
		mi_bkp_type type; // breakpoint type
		mi_bkp_disp disp; // ?
		bool enabled; // breakpoint state
		void* addr; // breakpoint address
		QString function; // breakpoint function
		QString file; // breakpoint file
		int line; // breakpoint line
		bool ignore; // hits ignored
		int times; // how many time to ignore
		QString condition; // breakpoint condition
		QString absolute_file; // breakpoint absolute file
		int thread; // breakpoint thread id
		mi_bkp_mode mode; // breakpoint mode
	};
	typedef QList<Breakpoint> BreakpointList;
};

Q_DECLARE_METATYPE( QGdb::Breakpoint )
Q_DECLARE_METATYPE( QGdb::BreakpointList )

#endif // QGDBBREAKPOINT_H
	