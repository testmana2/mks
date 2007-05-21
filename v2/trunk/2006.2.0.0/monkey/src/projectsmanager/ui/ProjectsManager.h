#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H
//
#include "ui_ProjectsManager.h"
#include "MonkeyExport.h"
//
#include <QPointer>
#include <QAction>
//
class AbstractProjectProxy;
class AbstractProjectModel;
//
class Q_MONKEY_EXPORT ProjectsManager : public QDockWidget, public Ui::ProjectsManager
{
	Q_OBJECT
	//
public:
	static ProjectsManager* self( QWidget* = 0 );
	//
	QStringList projectsFilters() const;
	void newProject();
	bool openProject( const QString& = QString::null );
	void saveCurrent();
	void saveAll();
	void closeCurrent();
	void closeAll();
	void projectSettings();
	//
	void addProxy( AbstractProjectProxy* );
	AbstractProjectModel* modelByIndex( const QModelIndex& ) const;
	AbstractProjectProxy* proxyByIndex( const QModelIndex& ) const;
	QAbstractItemView* viewByProxy( AbstractProjectProxy* ) const;
	AbstractProjectProxy* currentProxy() const;
	AbstractProjectModel* currentModel() const;
	QAbstractItemView* currentView() const;
	QModelIndex currentProject() const;
	void setCurrentProject( const QModelIndex& );
	void closeProject( const QModelIndex& );
	//
protected:
	void showEvent( QShowEvent* );
	void closeEvent( QCloseEvent* );
	//
private:
	ProjectsManager( QWidget* = 0 );
	QAction* aProjectsList;
	QAction* aFilteredView;
	static QPointer<ProjectsManager> mSelf;
	//
public slots:
	void setFilteredModel( bool );
	//
private slots:
	void projectIsModified( bool, const QModelIndex& );
	void projectAboutToClose( const QModelIndex& );
	//
	void buildProjectTreeItems( AbstractProjectProxy*, const QModelIndex&, QTreeWidgetItem* = 0 );
	void updateProjectActions( const QModelIndex& );
	void removeProjectItem( const QModelIndex& );
	void on_twProjects_itemClicked( QTreeWidgetItem*, int );
	void tv_clicked( const QModelIndex& );
	//
signals:
	void proxyAdded( AbstractProjectProxy* );
	void currentProxyChanged( AbstractProjectProxy* );
	void currentProjectChanged( AbstractProjectModel* );
	void fileOpenRequested( const QString&, AbstractProjectProxy* );
	//
};
//
#endif // PROJECTSMANAGER_H
