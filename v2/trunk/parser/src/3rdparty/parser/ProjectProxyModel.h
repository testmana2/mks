#ifndef PROJECTPROXYMODEL_H
#define PROJECTPROXYMODEL_H
//
#include <QSortFilterProxyModel>
//
class ProjectItemModel;
//
class ProjectProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	ProjectProxyModel( ProjectItemModel* );
	//
	ProjectItemModel* project() const;
	//
private:
	ProjectItemModel* mSource;
	bool mFilterOn;
private slots:
	void setFilter(int);
	void currentProjectName( const QModelIndex&);
	//
protected:
	bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
signals:
	void sig_curPro(const QString&);
};
//
#endif // PROJECTPROXYMODEL_H