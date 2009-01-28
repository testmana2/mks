#include "QGdb.h"

#include <QMetaType>

void QGdb::registerStructures()
{
	/*
	qRegisterMetaType<QGdb::State>( "QGdb::State" );
	qRegisterMetaType<QGdb::Breakpoint>( "QGdb::Breakpoint" );
	qRegisterMetaType<QGdb::CallStack>( "QGdb::CallStack" );
	qRegisterMetaType<QGdb::Signal>( "QGdb::Signal" );
	*/
}

QString QGdb::tr( const char* string )
{
	return QT_TR_NOOP( string );
}
