#ifndef QGDB_H
#define QGDB_H

#include "mi_gdb.h"

#include <QString>

namespace QGdb
{
	enum CBType // call back type
	{
		CONSOLE = 0,
		TARGET,
		LOG,
		TO_GDB,
		FROM_GDB,
		ASYNC,
		ERROR
	};
	
	enum State // driver state
	{
		DISCONNECTED = -1,
		CONNECTED,
		TARGET_SETTED,
		RUNNING,
		STOPPED
	};
	
	void registerStructures();
	QString tr( const char* string );
};

#endif // QGDB_H
