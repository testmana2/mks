#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H
//
#include "ui_ProjectsManager.h"
#include "MonkeyExport.h"
//
#include <QPointer>
#include <QAbstractItemView>
//
class AbstractProjectModel;
class AbstractProjectProxy;
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
	void addProxy( AbstractProjectProxy*, QAbstractItemView* = 0 );
	AbstractProjectModel* modelByIndex( const QModelIndex& ) const;
	AbstractProjectProxy* proxyByIndex( const QModelIndex& ) const;
	QAbstractItemView* viewByProxy( AbstractProjectProxy* ) const;
	AbstractProjectProxy* currentProxy() const;
	AbstractProjectModel* currentModel() const;
	QAbstractItemView* currentView() const;
	QModelIndex currentProject() const;
	void setCurrentProject( const QModelIndex& );
	//
private:
	ProjectsManager( QWidget* = 0 );
	static QPointer<ProjectsManager> mSelf;
	//  
public slots:
	void setFiltering( bool );
	//
private slots:
	void projectIsModified( bool, const QModelIndex& );
	void projectAboutToClose( const QModelIndex& );
	void updateProjectActions( const QModelIndex& );
	void view_clicked( const QModelIndex& );
	//
signals:
	void proxyAdded( AbstractProjectProxy* );
	void currentProxyChanged( AbstractProjectProxy* );
	void currentProjectChanged( AbstractProjectModel*);
	void fileOpenRequested( const QString&, AbstractProjectProxy* );
	//
};
//
#endif // PROJECTSMANAGER_H
