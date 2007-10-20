#include <QPushButton>
#include <QString>
#include<QStringList>
#include <QTabWidget>
#include <QFileInfo>
#include<QDebug>
#include<QModelIndex>

#include "ClassBrouser.h"
#include "pDockToolBar.h"
#include "Ctags.h"
#include "UIProjectsManager.h"
#include "ProjectsProxy.h"
#include "ClassBrouserSettings.h"
#include "pSettings.h"
#include "pWorkspace.h"
#include "ProjectItem.h"
#include "UIMain.h"
#include "pFileManager.h"

ClassBrouser::ClassBrouser ()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Class Brouser" );
	mPluginInfos.Description = tr( "Uses Exuberant Ctags library for analizing source files, and displaying information about current file and project" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name =  "Class Brouser";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
}

bool ClassBrouser::setEnabled (bool e)
{
	if (mPluginInfos.Enabled ==e)
		return false;
	mPluginInfos.Enabled = e;
	if (mPluginInfos.Enabled)
	{
		dockwgt = new QDockWidget( pWorkspace::instance());
		//dockwgt->hide ();
		dockwgt->setMinimumWidth (100);
//		tabw = new QTabWidget ();
// 		projectWidget = new QWidget (dockwgt);
// 		projectBox = new QVBoxLayout ( projectWidget);
// 		projectWidget->setLayout ( projectBox );
// 		currProjectTreew = new EntityContainer (projectWidget,"No project selected",pSettings::instance()->value ("Plugins/ClassBrouser/projectDisplayMask",-1).toInt());
// 		projectTrees.insert ( NULL, currProjectTreew);
// 		projectBox->addWidget ( currProjectTreew);
// 		projectLock = new QPushButton (tr("Lock view"), projectWidget);
// 		projectLock->setCheckable ( true );
// 		projectBox->addWidget (projectLock);
		fileWidget = new QWidget (dockwgt);
		fileBox = new QVBoxLayout ( fileWidget);
		currFileTreew = new EntityContainer (fileWidget,"No file selected",pSettings::instance()->value ("Plugins/ClassBrouser/fileDisplayMask",-1).toInt());
		fileTrees.insert ( NULL, currFileTreew);
		fileBox->addWidget ( currFileTreew);
		fileLock = new QPushButton (tr("Lock view"), fileWidget);
		fileLock->setCheckable ( true );
		fileBox->addWidget (fileLock);
// 		tabw->addTab ( projectWidget, "Project Tree");
//		tabw->addTab ( fileWidget, "File Tree");
		dockwgt->setWidget (fileWidget);
		UIMain::instance()->dockToolBar( Qt::RightToolBarArea )->addDock( dockwgt,  tr( "Class Brouser" ), QPixmap( ":/icons/ClassBrouser.png" ) );
		connect ( pFileManager::instance(), SIGNAL (currentFileChanged( const QString& )) , this, SLOT (currentFileChanged( const QString )));
	}
	else
	{
		disconnect ( pFileManager::instance(), SIGNAL (currentFileChanged( const QString& )) , this, SLOT (currentFileChanged( const QString& )));
		delete dockwgt;
	}
	return true;
}

// QWidget* ClassBrouser::settingsWidget ()
// {
// 	return new ClassBrouserSettings (this,currProjectTreew->getDisplayMask (),currFileTreew->getDisplayMask ());
// }

// void ClassBrouser::setProjectMask (int mask)
// {
// 	currProjectTreew->setDisplayMask (mask);
// 	Settings::current()->setValue ("Plugins/ClassBrouser/projectDisplayMask",QVariant(mask));
// }
// 
// void ClassBrouser::setFileMask (int mask)
// {
// 	currFileTreew->setDisplayMask (mask);
// 	Settings::current()->setValue ("Plugins/ClassBrouser/fileDisplayMask",QVariant(mask));	
// }

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

// void ClassBrouser::currentProjectChanged( ProjectItem* it)
// {
// 	if ( it == NULL)
// 		return;
// 	if (projectLock->isChecked())
//         return;  //view is locked, do not need to change
//     qDebug ("trying to change project view");
// 	pDockToolBar* bar = pWorkspace::instance()->tabToolBar()->bar( TabToolBar::Right );
// 	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
// 		return;  //do not need do something, if tab not active
// 	EntityContainer* oldWidget = currProjectTreew;
// 	currProjectTreew = projectTrees[it];
// 	if ( currProjectTreew == NULL )
// 	{
// 		currProjectTreew = new EntityContainer ( NULL, "", true );
// 		projectTrees.insert ( it,currProjectTreew );
// 	}
// 	ProjectItemList list =  it->project()->getItemList (it, ProjectItem::ValueType, "","");
// 	foreach ( file, list)
// 	{
// 		currProjectTreew->updateFileInfo ( file->getFilePath() );	
// 	}
// 	currProjectTreew->setHeaderLabel (file->text());
// 	projectWidget->setUpdatesEnabled(false);
// 	projectBox->removeWidget (oldWidget );
// 	oldWidget->hide();
// 	projectBox->insertWidget (0, currProjectTreew);
// 	currProjectTreew->show();
// //	if ( currProjectTreew->topLevelItemCount () == 0 )
// //		dockwgt->hide();
// //	else
// //		dockwgt->show();
//  //
// 	projectWidget->setUpdatesEnabled(true);
// }

void ClassBrouser::currentFileChanged(const QString absPath)
{
 	if (fileLock->isChecked())
        return;  //view is locked, do not need to change
	showFile (absPath);
}

void ClassBrouser::showFile (const QString& absPath)
{
	QStringList files (absPath); //  'files' contains list of all paths
	QFileInfo finfo (absPath);
	QString nameWithoutDot = finfo.path()+"/"+finfo.completeBaseName ();
	//if .cpp for the .h exists
	if ( ( absPath.endsWith (".h") and ( QFileInfo(nameWithoutDot+".cpp").exists() )))
		files.append (nameWithoutDot+".cpp");
	//if .h for the .cpp exists
	else if ( ( absPath.endsWith (".cpp") and ( QFileInfo(nameWithoutDot+".h").exists() )))
		files.append (nameWithoutDot+".h");
//  	pDockToolBar* bar = mWorkspace->tabToolBar()->bar( TabToolBar::Right );
// 	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
// 		return;  //do not need do something, if tab not active
	EntityContainer* oldWidget = currFileTreew; //save current TreeView
 	currFileTreew = fileTrees [nameWithoutDot]; //Try to find Treew for requested file in the cache
	if ( currFileTreew == NULL ) //not finded
	{
		currFileTreew = new EntityContainer ( NULL, "", false );
		fileTrees.insert ( nameWithoutDot, currFileTreew );
	}//OK, not currFileTreew - actual for requested file
    for ( int i = 0; i< files.size(); i++)
	{
        currFileTreew->updateFileInfo ( files[i] );	
	}
	currFileTreew->setHeaderLabel ( QFileInfo (absPath).baseName() );
	fileWidget->setUpdatesEnabled(false);
	fileBox->removeWidget (oldWidget );
	oldWidget->hide();
	fileBox->insertWidget (0,currFileTreew);
	currFileTreew->show();
/*	if ( currFileTreew->topLevelItemCount () == 0 )
		dockwgt->hide();
	else
		dockwgt->show();
 */
	fileWidget->setUpdatesEnabled(true);
}


Q_EXPORT_PLUGIN2( BaseClassBrouser, ClassBrouser )