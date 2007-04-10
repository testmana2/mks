#ifndef PROJECTSETTINGSPROXYMODEL_H
#define PROJECTSETTINGSPROXYMODEL_H
//
#include <QSortFilterProxyModel>
//
class ProjectItemModel;
//
class ProjectSettingsProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	ProjectSettingsProxyModel( ProjectItemModel* );
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
};
//
#endif // PROJECTSETTINGSPROXYMODEL_H
