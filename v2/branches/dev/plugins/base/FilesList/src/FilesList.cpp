/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : FilesList.cpp
** Date      : 2007-11-04T22:50:43
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QFileInfo>

#include "FilesList.h"
#include "pWorkspace.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "pFileManager.h"

FilesList::FilesList (QObject* parent)
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Files List" );
	mPluginInfos.Description = tr( "Plugin that allow you to view currenly opened files in the listbox and switch them" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name =  "FilesList";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;

}

bool FilesList::setEnabled (bool e)
{
	if (mPluginInfos.Enabled ==e)
		return false;
	mPluginInfos.Enabled = e;
	if (mPluginInfos.Enabled)
	{
		dockwgt = new QDockWidget("Files list", UIMain::instance ());
		dockwgt->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		//dockwgt->hide ();
		dockwgt->setMinimumSize (QSize(100, 100));
		dockwgt->setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
		
		list = new QListWidget (dockwgt);
		
		dockwgt->setWidget (list);
		UIMain::instance()->addDockWidget (Qt::LeftDockWidgetArea, dockwgt);
		dockwgt->show ();
		//from pFileManager
		connect (pFileManager::instance(), SIGNAL (fileOpened (const QString)), this, SLOT (fileOpened (const QString)));
		connect (pFileManager::instance(), SIGNAL (fileClosed (const QString)), this, SLOT (fileClosed (const QString)));
		connect (pFileManager::instance(), SIGNAL (currentFileChanged( pAbstractChild*, const QString& )), this, SLOT (currentFileChanged( pAbstractChild*, const QString& )));
		//from QListWidget
		connect (list, SIGNAL (itemActivated (QListWidgetItem*)), this, SIGNAL (itemActivated (QListWidgetItem*)));
	}
	else
	{
		//from pFileManager
		disconnect (pFileManager::instance(), SIGNAL (fileOpened (const QString)), this, SLOT (fileOpened (const QString)));
		disconnect (pFileManager::instance(), SIGNAL (fileClosed (const QString)), this, SLOT (fileClosed (const QString)));
		disconnect (pFileManager::instance(), SIGNAL (currentFileChanged( pAbstractChild*, const QString& )), this, SLOT (currentFileChanged( pAbstractChild*, const QString& )));
		//from QListWidget
		disconnect (list, SIGNAL (itemActivated (QListWidgetItem*)), this, SIGNAL (itemActivated (QListWidgetItem*)));
		
		delete dockwgt;
	}
	return true;
}

QWidget* FilesList::settingsWidget ()
{
	//return new FilesListSettings ();
	return NULL;
}

#include <QDebug>
void FilesList::fileOpened (const QString file)
{
	list->addItem (QFileInfo(file).fileName());
	qWarning () << file<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}

void FilesList::fileClosed (const QString file)
{
//	list->addItem (QFileInfo(file).fileName());
//	qWarning () << file<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}

void FilesList::currentFileChanged( pAbstractChild*, const QString& )
{

}

void FilesList::itemActivated (QListWidgetItem* item)
{
}

Q_EXPORT_PLUGIN2( BaseNavigator, FilesList )
