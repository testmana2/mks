#include "ProjectSettingsProxyModel.h"
#include "ProjectItemModel.h"
#include "ProjectItem.h"
//
ProjectSettingsProxyModel::ProjectSettingsProxyModel( ProjectItemModel* m )
	: QSortFilterProxyModel( m )
{
	setSourceModel( m );
}
//
bool ProjectSettingsProxyModel::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	QModelIndex index;
	index = sourceModel()->index( r, 0, i );
	return index.data( ProjectItem::TypeRole ).toInt() != ProjectItem::Folder
		&& index.data( ProjectItem::TypeRole ).toInt() != ProjectItem::File
		&& index.data( ProjectItem::TypeRole ).toInt() != ProjectItem::Value;
}
