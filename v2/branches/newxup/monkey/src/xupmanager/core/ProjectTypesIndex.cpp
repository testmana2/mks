#include "ProjectTypesIndex.h"
#include "XUPProjectItem.h"
#include "pIconManager.h"

#include <QDir>

#include <QDebug>

ProjectTypesIndex::ProjectTypesIndex( QObject* parent )
	: QObject( parent )
{
}

void ProjectTypesIndex::registerType( const QString& projectType, const QMetaObject* projectMetaObject )
{
	mRegisteredProjectItems[ projectType.toLower() ] = projectMetaObject;
}

void ProjectTypesIndex::unRegisterType( const QString& projectType )
{
	mRegisteredProjectItems.remove( projectType.toLower() );
	mSuffixes.remove( projectType.toLower() );
}

bool ProjectTypesIndex::fileIsAProject( const QString& fileName ) const
{
	foreach ( const QString& projectType, mSuffixes.keys() ) {
		foreach ( const Pair_String_StringList& pair, mSuffixes[ projectType ] ) {
			if ( QDir::match( pair.second, fileName ) ) {
				return true;
			}
		}
	}
	
	return false;
}

XUPProjectItem* ProjectTypesIndex::newProjectItem( const QString& fileName ) const
{
	foreach ( const QString& projectType, mSuffixes.keys() ) {
		foreach ( const Pair_String_StringList& pair, mSuffixes[ projectType ] ) {
			if ( QDir::match( pair.second, fileName ) ) {
				return qobject_cast<XUPProjectItem*>( mRegisteredProjectItems[ projectType ]->newInstance() );
			}
		}
	}
	
	return 0;
}

void ProjectTypesIndex::registerSuffixes( const QString& projectType, const Pair_String_StringList_List& suffixes )
{
	mSuffixes[ projectType.toLower() ] = suffixes;
}

QString ProjectTypesIndex::projectsFilter() const
{
	QStringList suffixes;
	QStringList filters;
	
	foreach ( const QString& projectType, mSuffixes.keys() ) {
		foreach ( const Pair_String_StringList& pair, mSuffixes[ projectType ] ) {
			suffixes << pair.second;
			filters << QString( "%1 (%2)" ).arg( pair.first ).arg( pair.second.join( " " ) );
		}
	}
	
	if ( !filters.isEmpty() ) {
		filters.prepend( tr( "All Projects (%1)" ).arg( suffixes.join( " " ) ) );
	}
	
	return filters.join( ";;" );
}
