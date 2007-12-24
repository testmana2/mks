/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : FilesList.h
** Date      : 2007-11-04T22:50:44
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef FILESLIST_H
#define FILESLIST_H
//
#include <QDockWidget>
#include <QListWidget>

#include "BasePlugin.h"

class pAbstractChild;

class FilesList: public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	//
private:
	QDockWidget* dockwgt;
	QListWidget* list;

public:
	FilesList (QObject* parent = NULL);
	bool setEnabled (bool e);
	QWidget* settingsWidget ();

public slots:
	//from pFileManager
	void fileOpened (const QString file);
	void fileClosed (const QString file);
	void currentFileChanged( pAbstractChild*, const QString& );
	//from QListWidget
	void itemActivated (QListWidgetItem* item);
};
//

#endif //  NAVIGATOR_H
