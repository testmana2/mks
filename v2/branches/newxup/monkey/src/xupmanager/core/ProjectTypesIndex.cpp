#include "ProjectTypesIndex.h"
#include "XUPProjectItem.h"
#include "pIconManager.h"

#include <QDir>
#include <QDebug>

ProjectTypesIndex::ProjectTypesIndex( QObject* parent )
	: QObject( parent )
{
}

void ProjectTypesIndex::registerType( const QString& projectType, const QMetaObject* projectMetaObject, const DocumentFilterMap& filters )
{
	mRegisteredProjectItems[ projectType ] = projectMetaObject;
	mFilters[ projectType ] = filters;
}

void ProjectTypesIndex::unRegisterType( const QString& projectType )
{
	mRegisteredProjectItems.remove( projectType );
	mFilters.remove( projectType );
}

DocumentFilterMap ProjectTypesIndex::typeFilters( const QString& projectType ) const
{
	return mFilters.value( projectType );
}

bool ProjectTypesIndex::fileIsAProject( const QString& fileName ) const
{
	foreach ( const QString& projectType, mFilters.keys() ) {
		foreach ( const DocumentFilter& filter, mFilters[ projectType ] ) {
			if ( QDir::match( filter.filters, fileName ) ) {
				return true;
			}
		}
	}
	
	return false;
}

XUPProjectItem* ProjectTypesIndex::newProjectItem( const QString& fileName ) const
{
	foreach ( const QString& projectType, mFilters.keys() ) {
		foreach ( const DocumentFilter& filter, mFilters[ projectType ] ) {
			if ( QDir::match( filter.filters, fileName ) ) {
				return qobject_cast<XUPProjectItem*>( mRegisteredProjectItems[ projectType ]->newInstance() );
			}
		}
	}
	
	return 0;
}

QMap<QString, QStringList> ProjectTypesIndex::suffixes() const
{
	QMap<QString, QStringList> suffixes;
	
	foreach ( const QString& projectType, mFilters.keys() ) {
		foreach ( const DocumentFilter& filter, mFilters[ projectType ] ) {
			suffixes[ filter.label ] = filter.filters;
		}
	}
	
	return suffixes;
}
