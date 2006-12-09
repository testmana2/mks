#include "FileType.h"
#include "Settings.h"
//
#include <QHash>
//
//QHash
//
void FileType::initialize()
{
	QSettings* s = Settings::current();
	int size = s->beginReadArray( "FileType" );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		//
	}
	s->endArray();
}
