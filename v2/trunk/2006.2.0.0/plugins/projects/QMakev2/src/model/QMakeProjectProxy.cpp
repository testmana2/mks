#include "QMakeProjectProxy.h"
#include "QMakeProjectModel.h"
#include "Settings.h"
#include "UIQMakeProjectSettings.h"
#include "UISettingsQMake.h"
//
QMakeProjectProxy::QMakeProjectProxy( QMakeProjectModel* m, bool b, const QModelIndex& i )
	: AbstractProjectProxy( m ), mProjectViewMode( b ), mIndex( m->project( i ) ), mPIndex( project()->project( mIndex.parent() ) )
{
	if ( b )
	{
		setFilterRoles( QList<int>() << AbstractProjectModel::ScopeType << AbstractProjectModel::NestedScopeType << AbstractProjectModel::ScopeEndType );
		setNegateFilter( false );
		setSortRole( AbstractProjectModel::FilterRole );
	}
	setFiltering( true );
}
//
bool QMakeProjectProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	QModelIndex index = sourceModel()->index( r, 0, i );
	if ( !mProjectViewMode )
	{
		QModelIndex j = project()->project( index );
		bool b = AbstractProjectProxy::filterAcceptsRow( r, i );
		if ( b && j != mIndex && j != mPIndex )
			b = false;
		return b;
	}
	else
	{
		if ( !mFiltering )
			return true;
		int t = index.data( AbstractProjectModel::TypeRole ).toInt();
		if ( t == AbstractProjectModel::ValueType )
			return true;
		else if ( t == AbstractProjectModel::ProjectType )
			return true;
		else if ( t == AbstractProjectModel::VariableType )
		{
			QStringList l = Settings::current()->value( "Plugins/QMake/Filters" ).toStringList();
			if ( l.isEmpty() )
				l = UISettingsQMake::defaultFilters();
			QVariant v = index.data( AbstractProjectModel::ValueRole );
			int i = l.indexOf( v.toString(), Qt::CaseInsensitive );
			if ( i != -1 )
			{
				if ( index.data( AbstractProjectModel::FilterRole ).toInt() != i )
					sourceModel()->setData( index, i, AbstractProjectModel::FilterRole );
				return true;
			}
		}
		else if ( mFilterRoles.contains( t ) )
		{
			for ( int j = 0; j < sourceModel()->rowCount( index ); j++ )
				if ( filterAcceptsRow( j, index ) )
					return true;
		}
		return false;
	}
}
//
void QMakeProjectProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	emit filteringChanged( mFiltering );
	if ( mProjectViewMode )
	{
		if ( mFiltering )
			sort( 0, Qt::AscendingOrder );
		else
		{
			sort( -1 );
			reset();
		}
	}
	filterChanged();
}
//
void QMakeProjectProxy::doubleClicked( const QModelIndex& )
{
}
//
void QMakeProjectProxy::customContextMenuRequested( const QPoint& )
{
}
//
void QMakeProjectProxy::projectSettings( const QModelIndex& i )
{
	QModelIndex j = i.isValid() ? i : project()->rootProject();
	UIQMakeProjectSettings::execute( qobject_cast<QMakeProjectModel*>( project() )->itemFromIndex( j ) );
}
