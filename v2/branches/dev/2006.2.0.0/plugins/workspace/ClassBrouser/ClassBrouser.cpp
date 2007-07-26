#include <QPushButton>
#include <QString>
#include<QStringList>
#include <QTabWidget>
#include <QFileInfo>
#include<QDebug>
#include<QModelIndex>

#include "ClassBrouser.h"
#include "TabToolBar.h"
#include "Ctags.h"
#include "ProjectsManager.h"
#include "AbstractProjectProxy.h"


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
    projectLock = new QPushButton (tr("Lock view"), projectWidget);
    projectLock->setCheckable ( true );
    projectBox->addWidget (projectLock);
	fileWidget = new QWidget (dockwgt);
	fileBox = new QVBoxLayout ( fileWidget);
	fileWidget->setLayout ( fileBox );
	currFileTreew = new EntityContainer (projectWidget,"No file selected",false);
	fileTrees.insert ( NULL, currFileTreew);
	fileBox->addWidget ( currFileTreew);
    fileLock = new QPushButton (tr("Lock view"), fileWidget);
    fileLock->setCheckable ( true );
    fileBox->addWidget (fileLock);
	tabw->addTab ( projectWidget, "Project Tree");
	tabw->addTab ( fileWidget, "File Tree");
	dockwgt->setWidget ( tabw);
	mWorkspace->tabToolBar()->bar( TabToolBar::Right )->appendTab( dockwgt,  QPixmap( ":/icons/ClassBrouser.png" ), tr( "Class Brouser" ) );
	connect ( ProjectsManager::self(), SIGNAL (proxyAdded( AbstractProjectProxy* )) , this, SLOT (proxyAdded( AbstractProjectProxy* )));	
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

/*void ClassBrouser::freeProjectView(AbstractProjectModel* p)
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
*/
void ClassBrouser::proxyAdded( AbstractProjectProxy* proxy)
{
	connect ( proxy, SIGNAL (fileSelected( QString )) , this, SLOT (showFile( QString )));
 	connect ( proxy, SIGNAL (projectSelected( AbstractProjectModel*)) , this, SLOT (showProject(AbstractProjectModel*)));
}

void ClassBrouser::showProject(AbstractProjectModel* aim)
{
	if (projectLock->isChecked())
        return;  //view is locked, do not need to change
    qDebug ("trying to change project view");
	pTabToolBar* bar = mWorkspace->tabToolBar()->bar( TabToolBar::Right );
	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
		return;  //do not need do something, if tab not active
	EntityContainer* oldWidget = currProjectTreew;
	if ( aim == NULL)
		return;
	currProjectTreew = projectTrees[ aim];
	if ( currProjectTreew == NULL )
	{
		currProjectTreew = new EntityContainer ( NULL, "", true );
		projectTrees.insert ( aim,currProjectTreew );
	}
	QStringList list =  aim->absoluteFilesPath(QModelIndex(), true);
	QString fileName;
	foreach ( fileName, list)
	{
		currProjectTreew->updateFileInfo ( fileName );	
	}
	currProjectTreew->setHeaderLabel ( aim->name() );
	projectWidget->setUpdatesEnabled(false);
	projectBox->removeWidget (oldWidget );
	oldWidget->hide();
	projectBox->insertWidget (0, currProjectTreew);
	currProjectTreew->show();
//	if ( currProjectTreew->topLevelItemCount () == 0 )
//		dockwgt->hide();
//	else
//		dockwgt->show();
 //
	projectWidget->setUpdatesEnabled(true);
}

void ClassBrouser::showFile ( QString absPath)
{
 	if (fileLock->isChecked())
        return;  //view is locked, do not need to change
    QStringList files (absPath); //  'files' contains list of all paths
    QFileInfo finfo (absPath);
    QString nameWithoutDot = finfo.path()+"/"+finfo.completeBaseName ();
    //if .cpp for the .h exists
    if ( ( absPath.endsWith (".h") and ( QFileInfo(nameWithoutDot+".cpp").exists() )))
        files.append (nameWithoutDot+".cpp");
     //if .h for the .cpp exists
    else if ( ( absPath.endsWith (".cpp") and ( QFileInfo(nameWithoutDot+".h").exists() )))
        files.append (nameWithoutDot+".h");
 	pTabToolBar* bar = mWorkspace->tabToolBar()->bar( TabToolBar::Right );
	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
		return;  //do not need do something, if tab not active
	EntityContainer* oldWidget = currFileTreew; //save current TreeView
 	currFileTreew = fileTrees [nameWithoutDot]; //Try to find Treew for requested file in the cache
	if ( currFileTreew == NULL ) //not finded
	{
		currFileTreew = new EntityContainer ( NULL, "", false );
		fileTrees.insert ( nameWithoutDot, currFileTreew );
	}//OK, not currFileTreew - actual for requested file
    else
    for ( int i = 0; i< files.size(); i++)
        currFileTreew->updateFileInfo ( files[i] );	
	currFileTreew->setHeaderLabel ( QFileInfo (absPath).baseName() );
	fileWidget->setUpdatesEnabled(false);
	fileBox->removeWidget (oldWidget );
	oldWidget->hide();
	fileBox->addWidget ( currFileTreew);
	currFileTreew->show();
/*	if ( currFileTreew->topLevelItemCount () == 0 )
		dockwgt->hide();
	else
		dockwgt->show();
 */
	fileWidget->setUpdatesEnabled(true);
}

//
Q_EXPORT_PLUGIN2( WorkspaceClassBrouser, ClassBrouser )