#include "XUPProjectItemInfos.h"
#include "XUPProjectItem.h"
#include "pIconManager.h"

#include <QDir>

#include <QDebug>

XUPProjectItemInfos::XUPProjectItemInfos()
{
}

bool XUPProjectItemInfos::isRegisteredType( int projectType ) const
{
	return mRegisteredProjectItems.keys().contains( projectType );
}

void XUPProjectItemInfos::registerType( int projectType, XUPProjectItem* projectItem )
{
	if ( !isRegisteredType( projectType ) )
		mRegisteredProjectItems[ projectType ] = projectItem;
}

void XUPProjectItemInfos::unRegisterType( int projectType )
{
	delete mRegisteredProjectItems.take( projectType );
	mOperators.remove( projectType );
	mFilteredVariables.remove( projectType );
	mSuffixes.remove( projectType );
}

XUPProjectItem* XUPProjectItemInfos::newProjectItem( const QString& fileName ) const
{
	int projectType = projectTypeForFileName( fileName );
	return projectType == XUPProjectItem::InvalidProject ? 0 : mRegisteredProjectItems[ projectType ]->newProject();
}

void XUPProjectItemInfos::registerOperators( int projectType, const QStringList& operators )
{
	mOperators[ projectType ] = operators;
}

QStringList XUPProjectItemInfos::operators( int projectType ) const
{
	return mOperators.value( projectType );
}

void XUPProjectItemInfos::registerFilteredVariables( int projectType, const QStringList& variables )
{
	mFilteredVariables[ projectType ] = variables;
}

QStringList XUPProjectItemInfos::filteredVariables( int projectType ) const
{
	return mFilteredVariables.value( projectType );
}

void XUPProjectItemInfos::registerSuffixes( int projectType, const StringStringListList& suffixes )
{
	mSuffixes[ projectType ] = suffixes;
}

QString XUPProjectItemInfos::projectsFilter() const
{
	QStringList suffixes;
	QStringList filters;
	foreach ( const int& projectType, mSuffixes.keys() )
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

int XUPProjectItemInfos::projectTypeForFileName( const QString& fileName ) const
{
	foreach ( const int& projectType, mSuffixes.keys() )
	{
		foreach ( const PairStringStringList& p, mSuffixes[ projectType ] )
		{
			if ( QDir::match( p.second, fileName ) )
				return projectType;
		}
	}
	return XUPProjectItem::InvalidProject;
}

QStringList XUPProjectItemInfos::knowVariables( int projectType ) const
{
	QStringList variables;
	
	foreach ( const QString& variable, mFilteredVariables.value( projectType ) )
	{
		if ( !variables.contains( variable ) )
		{
			variables << variable;
		}
	}
	
	variables.sort();
	return variables;
}
