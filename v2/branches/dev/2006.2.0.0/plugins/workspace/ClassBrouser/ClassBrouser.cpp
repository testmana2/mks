#include <QPushButton>
#include <QString>
#include<QStringList>
#include <QTabWidget>
#include<QDebug>

#include "ClassBrouser.h"
#include "TabToolBar.h"
#include "Ctags.h"
#include "ProjectsManager.h"


//
void ClassBrouser::initialize( Workspace* w )
{
	WorkspacePlugin::initialize( w );
	//
	currProjectTreew = NULL;
	mPluginInfos.Caption = tr( "Class Brouser" );
	mPluginInfos.Description = tr( "Tree of clases and other project entityes" );
	mPluginInfos.Type = BasePlugin::iWorkspace;
	mPluginInfos.Name = "ClassBrouser";
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Installed = false;

	dockwgt = new QDockWidget( w);
	dockwgt->hide ();
	dockwgt->setMinimumWidth (100);
	tabw = new QTabWidget ();
	projectWidget = new QWidget (dockwgt);
	projectBox = new QVBoxLayout ( projectWidget);
	projectWidget->setLayout ( projectBox );
	currProjectTreew = new EntityContainer (projectWidget,"No project selected",false);
	projectTrees.insert ( NULL, currProjectTreew);
	projectBox->addWidget ( currProjectTreew);
	fileWidget = new QWidget (dockwgt);
	fileBox = new QVBoxLayout ( fileWidget);
	fileWidget->setLayout ( fileBox );
	currFileTreew = new EntityContainer (projectWidget,"No file selected",false);
	fileTrees.insert ( NULL, currFileTreew);
	fileBox->addWidget ( currFileTreew);
	tabw->addTab ( projectWidget, "Project Tree");
	tabw->addTab ( fileWidget, "File Tree");
	dockwgt->setWidget ( tabw);
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->appendTab( dockwgt,  QPixmap( ":/icons/ClassBrouser.png" ), tr( "Class Brouser" ) );

	connect ( ProjectsManager::self(), SIGNAL (currentProjectChanged( AbstractProjectModel* )) , this, SLOT (changeProjectView(AbstractProjectModel*)));
	connect ( ProjectsManager::self(), SIGNAL (itemModelWillBeClosed( AbstractProjectItemModel* )) , this, SLOT (freeProjectView(AbstractProjectItemModel*)));	
};
//
bool ClassBrouser::install()
{
	if ( isInstalled() )
		return true;
	// create dock
	mPluginInfos.Installed = true;
	return true;
};
//
bool ClassBrouser::uninstall()
{
	delete (dockwgt);
	if ( !isInstalled() )
		return true;
	mPluginInfos.Installed = false;
	return true;
};

void ClassBrouser::changeProjectView(AbstractProjectModel* aim)
{
	qDebug ("trying to change project view");
	pTabToolBar* bar = mWorkspace->tabToolBar()->bar( TabToolBar::Right );
	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
		return;  //do not need do something, if tab not active
	EntityContainer* oldWidget = currProjectTreew;
	if ( aim == NULL)
		return;
	currProjectTreew = projectTrees.take ( aim );
	if ( currProjectTreew == NULL )
	{
		currProjectTreew = new EntityContainer ( NULL, "", true );
		projectTrees.insert ( aim,currProjectTreew );
	}
	QStringList list =  aim->absoluteFilesPath();
	QString fileName;
	foreach ( fileName, list)
	{
		currProjectTreew->updateFileInfo ( fileName );	
	}
	currProjectTreew->setHeaderLabel ( aim->name() );
	projectWidget->setUpdatesEnabled(false);
	projectBox->removeWidget (oldWidget );
	oldWidget->hide();
	projectBox->addWidget ( currProjectTreew);
	currProjectTreew->show();
	if ( currProjectTreew->topLevelItemCount () == 0 )
		dockwgt->hide();
	else
		dockwgt->show();
	projectWidget->setUpdatesEnabled(true);
}

void ClassBrouser::freeProjectView(AbstractProjectModel* p)
{
	if ( not projectTrees.contains ( p))
		return;
	if ( currProjectTreew == projectTrees.take (p) )
	{//current project will be closed, need to change displaying widget
		projectWidget->setUpdatesEnabled(false);
		projectBox->removeWidget (currProjectTreew );
		currProjectTreew->hide();
		currProjectTreew = projectTrees.take (NULL);
		projectBox->addWidget ( currProjectTreew);
		currProjectTreew->show();
		if ( currProjectTreew->topLevelItemCount () == 0 )
			dockwgt->hide();
		else
			dockwgt->show();
		projectWidget->setUpdatesEnabled(true);
	}
	delete ( projectTrees.take(p));
	projectTrees.remove ( p);
	qDebug ( "Project was deleted");
}

void ClassBrouser::changeFileView()
{
/*	pTabToolBar* bar = mWorkspace->tabToolBar()->bar( TabToolBar::Right );
	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
		return;  //do not need do something, if tab not active
	EntityContainer* oldWidget = currFileTreew;
	
	foreach ( fileName, list)
	{
		currProjectTreew->updateFileInfo ( fileName );	
	}
	currProjectTreew->setHeaderLabel ( aim->name() );
	projectWidget->setUpdatesEnabled(false);
	projectBox->removeWidget (oldWidget );
	if ( oldWidget != NULL )
		oldWidget->hide();
	projectBox->addWidget ( currProjectTreew);
	currProjectTreew->show();
	if ( currProjectTreew->topLevelItemCount () == 0 )
		dockwgt->hide();
	else
		dockwgt->show();
	projectWidget->setUpdatesEnabled(true);
*/
}

//
Q_EXPORT_PLUGIN2( WorkspaceClassBrouser, ClassBrouser )