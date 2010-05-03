#include "PathsProperties.h"

#include <XmlObjectProcessor.h>

#include <QStringList>
	
PathsProperties::PathsProperties()
{
	setPaths( SL_TEMPLATES, QStringList( "templates1" ) << "templates2" );
	setPaths( SL_APIS, QStringList( "apis1" ) << "apis2" );
}

QStringList PathsProperties::paths( PathsProperties::StorageLocation location ) const
{
	return mPaths.value( storageLocationToString( location ) ).toStringList();
}

void PathsProperties::setPaths( PathsProperties::StorageLocation location, const QStringList& paths )
{
	const QString name = storageLocationToString( location );
	
	if ( name.isEmpty() ) {
		return;
	}
	
	mPaths[ name ] = paths;
}

QString PathsProperties::storageLocationToString( PathsProperties::StorageLocation location )
{
	switch ( location )
	{
		case SL_TEMPLATES:
			return QLatin1String( "templates" );
		case SL_TRANSLATIONS:
			return QLatin1String( "translations" );
		case SL_PLUGINS:
			return QLatin1String( "plugins" );
		case SL_SCRIPTS:
			return QLatin1String( "scripts" );
		case SL_APIS:
			return QLatin1String( "apis" );
	}
	
	return QString::null;
}

void PathsProperties::processData( XmlObjectProcessor* container )
{
	container->processData( "Paths", mPaths );
}
