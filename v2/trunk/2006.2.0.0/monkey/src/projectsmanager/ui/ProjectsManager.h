#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H
//
#include "ui_ProjectsManager.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class AbstractProjectProxy;
class AbstractProjectItemModel;
//
typedef QList<AbstractProjectProxy*> QProxyList;
typedef QList<AbstractProjectItemModel*> QProjectList;
//
class Q_MONKEY_EXPORT ProjectsManager : public QDockWidget, public Ui::ProjectsManager
{
	Q_OBJECT
	//
public:
	enum TreeItemRole { ProxyIdRole = Qt::UserRole, ProjectFilePathRole };
	//
	static ProjectsManager* self( QWidget* = 0 );
	//
	QProxyList rootProxies() const;
	QProjectList rootProjects() const;
	void setCurrentProxy( AbstractProjectProxy* );
	AbstractProjectProxy* currentProxy() const;
	void setCurrentProject( AbstractProjectItemModel* );
	AbstractProjectItemModel* currentProject() const;
	QTreeWidgetItem* getItemByProxyId( int );
	QAbstractItemView* getViewByProxyId( int );
	void closeProxy( AbstractProjectProxy* );
	void closeProject( AbstractProjectItemModel* );
	void addProxy( AbstractProjectProxy*, AbstractProjectProxy* = 0 );
	//
protected:
	void showEvent( QShowEvent* );
	void closeEvent( QCloseEvent* );
	//
private:
	ProjectsManager( QWidget* = 0 );
	static QPointer<ProjectsManager> mSelf;
	//
public slots:
	void setTreeProjectsVisible( bool );
	void setComplexModel( bool );
	//
private slots:
	void on_tbSave_clicked();
	void on_tbClose_clicked();
	void on_twProjects_itemClicked( QTreeWidgetItem*, int );
	void on_swProjects_currentChanged( int );
	//
signals:
	void proxyAdded( AbstractProjectProxy* );
	void currentProxyChanged( AbstractProjectProxy* );
	void fileOpenRequested( const QString&, AbstractProjectProxy* );
	//
};
//
#endif // PROJECTSMANAGER_H
