#include "ProjectTypesIndex.h"
#include "XUPProjectItem.h"
#include "pIconManager.h"

#include <QDir>
#include <QDebug>

ProjectTypesIndex::ProjectTypesIndex( QObject* parent )
	: QObject( parent )
{
}

void ProjectTypesIndex::registerType( const QString& projectType, const QMetaObject* projectMetaObject, const Pair_String_StringList_List& suffixes )
{
	mRegisteredProjectItems[ projectType ] = projectMetaObject;
	mSuffixes[ projectType ] = suffixes;
}

void ProjectTypesIndex::unRegisterType( const QString& projectType )
{
	mRegisteredProjectItems.remove( projectType );
	mSuffixes.remove( projectType );
}

Pair_String_StringList_List ProjectTypesIndex::typeSuffixes( const QString& projectType ) const
{
	return mSuffixes.value( projectType );
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

QMap<QString, QStringList> ProjectTypesIndex::suffixes() const
{
	QMap<QString, QStringList> suffixes;
	
	foreach ( const QString& projectType, mSuffixes.keys() ) {
		foreach ( const Pair_String_StringList& pair, mSuffixes[ projectType ] ) {
			suffixes[ pair.first ] = pair.second;
		}
	}
	
	return suffixes;
}
