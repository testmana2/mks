#ifndef CLASSBROUSER_H
#define CLASSBROUSER_H
//
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHash>
#include <QString>
#include <QTabWidget>
#include <QTreeWidget>

#include "WorkspacePlugin.h"
#include "Ctags.h"
#include "EntityContainer.h"
#include "AbstractProjectModel.h"

// I am not sure with the English language terminology, if you ( reader) know it better, correct it

class ClassBrouser : public WorkspacePlugin 
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )
	//
private:
	QHash <AbstractProjectModel* , EntityContainer*> projectTrees;
	QHash <QString , EntityContainer*> fileTrees;
	QDockWidget* dockwgt;
	QTabWidget* tabw;
	QWidget* projectWidget ;
	QWidget* fileWidget ;
	QVBoxLayout* projectBox;
	QVBoxLayout* fileBox;
	EntityContainer* currProjectTreew;
	EntityContainer* currFileTreew;
public:
	virtual void initialize( Workspace* );
//	virtual QWidget* settingsWidget();
	virtual bool install();
	virtual bool uninstall();
	//
public slots:
	//void changeProjectView(AbstractProjectModel*, QModelIndex);
	//void freeProjectView(AbstractProjectModel*);
	//void changeFileView();
    void showFile ( QString absPath);
    void showProject (  AbstractProjectModel*);
    void proxyAdded( AbstractProjectProxy* );
};
//

#endif //  CLASSBROUSER_H
