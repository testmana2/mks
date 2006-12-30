#ifndef PROJECTSMANAGER_H
#define PROJECTSMANAGER_H
//
#include "ui_ProjectsManager.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class AbstractProjectProxy;
//
class Q_MONKEY_EXPORT ProjectsManager : public QWidget, public Ui::ProjectsManager
{
	Q_OBJECT
	//
public:
	enum TreeItemRole { ProxyIdRole = Qt::UserRole, ProjectFilePathRole };
	//
	static ProjectsManager* self( QWidget* = 0 );
	//
	void setCurrentProxy( AbstractProjectProxy* );
	AbstractProjectProxy* currentProxy() const;
	QTreeWidgetItem* getItemByProxyId( int );
	QAbstractItemView* getViewByProxyId( int );
	bool openProject( const QString&, int = -1 );
	void closeProject( QTreeWidgetItem* );
	//
private:
	ProjectsManager( QWidget* = 0 );
	static QPointer<ProjectsManager> mSelf;
	//
private slots:
	void on_tbOpen_clicked();
	void on_tbComplexCurrent_clicked();
	void on_tbSaveCurrent_clicked();
	void on_tbCloseCurrent_clicked();
	void on_twProjects_itemClicked( QTreeWidgetItem*, int );
	void on_swProjects_currentChanged( int );
	//
};
//
#endif // PROJECTSMANAGER_H
