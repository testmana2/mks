#ifndef QGDBSIGNAL_H
#define QGDBSIGNAL_H

#include "QGdb.h"

#include <QMetaType>

namespace QGdb
{
	struct Signal
	{
		Signal( mi_stop* stop = 0 );
		
		/*** For dynamic memory management (can be removed if it isn't necessary) ***/
		Signal( const Signal& other );
		Signal& operator=( const Signal& other );
		bool operator==( const Signal& other ) const;
		bool operator!=( const Signal& other ) const;
		/*** End of dynamic memory management ***/
		
		// properties
		QString name; // signal name
		QString meaning; // signal description
	};
	typedef QList<Signal> SignalList;
};

Q_DECLARE_METATYPE( QGdb::Signal )
Q_DECLARE_METATYPE( QGdb::SignalList )

#endif // QGDBSIGNAL_H
	