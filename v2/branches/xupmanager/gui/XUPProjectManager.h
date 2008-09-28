#ifndef XUPPROJECTMANAGER_H
#define XUPPROJECTMANAGER_H

#include "ui_XUPProjectManager.h"

#include <QMap>

class XUPProjectModel;
class XUPProjectItem;

class XUPProjectManager : public QWidget, public Ui::XUPProjectManager
{
	Q_OBJECT

public:
	enum ActionType { atOpen = 0, atClose };
	XUPProjectManager( QWidget* parent = 0 );
	virtual ~XUPProjectManager();
	
	QAction* action( XUPProjectManager::ActionType type );
	XUPProjectModel* currentProject() const;
	
	void registerProjectItem( XUPProjectItem* item );
	void unRegisterProjectType( int projectType );

protected:
	QMap<XUPProjectManager::ActionType, QAction*> mActions;
	QMap<QListWidgetItem*, XUPProjectModel*> mProjects;
	QMap<int, XUPProjectItem*> mRegisteredProjectItems; // project type, project item
	
	XUPProjectItem* newProjectItem( const QString& fileName ) const;

public slots:
	void openProject();
	void closeProject();
	void setCurrentProject( XUPProjectModel* project );

protected slots:
	void on_lwOpenedProjects_itemSelectionChanged();
};

#endif // XUPPROJECTMANAGER_H
