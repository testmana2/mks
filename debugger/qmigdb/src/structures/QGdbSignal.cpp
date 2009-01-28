#include "QGdbSignal.h"

QGdb::Signal::Signal( mi_stop* stop )
{
	if ( stop )
	{
		name = stop->signal_name;
		meaning = stop->signal_meaning;
	}
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
QGdb::Signal::Signal( const QGdb::Signal& other )
{
	operator=( other );
}

QGdb::Signal& QGdb::Signal::operator=( const QGdb::Signal& other )
{
	if( *this != other )
	{
		name = other.name;
		meaning = other.meaning;
	}

	return *this;
}

bool QGdb::Signal::operator==( const QGdb::Signal& other ) const
{
	return name == other.name && meaning == other.meaning;
}

bool QGdb::Signal::operator!=( const QGdb::Signal& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/
