#ifndef CLASSBROUSER_H
#define CLASSBROUSER_H
//
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHash>
#include <QString>
#include <QTabWidget>
#include <QTreeWidget>

#include "BasePlugin.h"
#include "Ctags.h"
#include "EntityContainer.h"
#include "ProjectsModel.h"
#include "pWorkspace.h"

class ProjectItem;
// I am not sure with the English language terminology, if you ( reader) know it better, correct it

class ClassBrouser : public BasePlugin 
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	//
private:
	QHash <ProjectItem* , EntityContainer*> projectTrees;
	QHash <QString , EntityContainer*> fileTrees;
	QDockWidget* dockwgt;
//	QTabWidget* tabw;
//	QWidget* projectWidget ;
	QWidget* fileWidget ;
//	QVBoxLayout* projectBox;
	QVBoxLayout* fileBox;
//	EntityContainer* currProjectTreew;
	EntityContainer* currFileTreew;
//	QPushButton* projectLock;
	QPushButton* fileLock;

public:
	ClassBrouser ();
	bool setEnabled (bool e);
//	virtual QWidget* settingsWidget ();

//	void setProjectMask (int mask);
//	void setFileMask (int mask);
	//
public slots:
	//void changeProjectView(AbstractProjectModel*, QModelIndex);
	//void freeProjectView(AbstractProjectModel*);
	//void changeFileView();
    void showFile (const QString&);
	void currentFileChanged(const QString);
	void currentProjectChanged( ProjectItem* );
};
//

#endif //  CLASSBROUSER_H
