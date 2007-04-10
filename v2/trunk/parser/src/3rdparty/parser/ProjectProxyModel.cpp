#include "ProjectProxyModel.h"
#include "ProjectItemModel.h"
#include "ProjectItem.h"
#include <QModelIndex>
#include <QFileInfo>
//#include <iostream.h>
//
ProjectProxyModel::ProjectProxyModel( ProjectItemModel* s )
		: QSortFilterProxyModel(s), mSource( s ), mFilterOn(false)
{
	setSourceModel( mSource );
}
//
ProjectItemModel* ProjectProxyModel::project() const
{
	return mSource;
}
//
bool ProjectProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent) const
{
	if (!mFilterOn)
		return true;

	QModelIndex index;
	index = sourceModel()->index(sourceRow, 0, sourceParent);
	return index.data(ProjectItem::FilterRole).toBool();
}
//
void ProjectProxyModel::setFilter(int on)
{
	if (!on)
		mFilterOn =  false;
	else
		mFilterOn = true;

	if (mFilterOn)
	{
		setSortRole(ProjectItem::SortRole);
		sort(0);
		filterChanged ();
	}
	else
	{
		setSortRole(ProjectItem::SortRole2);
		sort(0);
		filterChanged ();
	}
}
//
void ProjectProxyModel::currentProjectName( const QModelIndex& i)
{
	QModelIndex index = mapToSource( i );
	if (index.isValid())
		emit sig_curPro("Current project : "+project()->currentProjectName(index));
}
//
