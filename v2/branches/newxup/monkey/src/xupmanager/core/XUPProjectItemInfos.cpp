#include "XUPProjectItemInfos.h"
#include "XUPProjectItem.h"
#include "pIconManager.h"

#include <QDir>

#include <QDebug>

XUPProjectItemInfos::XUPProjectItemInfos()
{
}

void XUPProjectItemInfos::registerType( QString projectType, XUPProjectItem* projectItem )
{
	mRegisteredProjectItems[ projectType ] = projectItem;
}

void XUPProjectItemInfos::unRegisterType( QString projectType )
{
	delete mRegisteredProjectItems.take( projectType );
	mSuffixes.remove( projectType );
}

XUPProjectItem* XUPProjectItemInfos::newProjectItem( const QString& fileName ) const
{
	foreach ( const QString& projectType, mSuffixes.keys() )
	{
		foreach ( const PairStringStringList& p, mSuffixes[ projectType ] )
		{
			if ( QDir::match( p.second, fileName ) )
				return mRegisteredProjectItems[ projectType ]->newProject();
		}
	}
	return NULL;
}

void XUPProjectItemInfos::registerSuffixes( QString projectType, const StringStringListList& suffixes )
{
	mSuffixes[ projectType ] = suffixes;
}

QString XUPProjectItemInfos::projectsFilter() const
{
	QStringList suffixes;
	QStringList filters;
	foreach ( const QString& projectType, mSuffixes.keys() )
	{
		foreach ( const PairStringStringList& p, mSuffixes[ projectType ] )
		{
			suffixes << p.second;
			filters << QString( "%1 (%2)" ).arg( p.first ).arg( p.second.join( " " ) );
		}
	}
	if ( !filters.isEmpty() )
		filters.prepend( tr( QT_TR_NOOP( "All Projects (%1)" ) ).arg( suffixes.join( " " ) ) );
	return filters.join( ";;" );
}
