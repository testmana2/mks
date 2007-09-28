/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : NoProject project plugin
** FileName  : NoProjectProjectSettings.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : NoProject project settings
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UINOPROJECTPROJECTSETTINGS_H
#define UINOPROJECTPROJECTSETTINGS_H

#include <QHash>

#include "ui_UINoProjectProjectSettings.h"

#include "NoProjectProjectItem.h"

class UINoProjectProjectSettings : public QDialog, Ui_UINoProjectProjectSettings
{
	Q_OBJECT
public:
	UINoProjectProjectSettings (NoProjectProjectItem* project);

	QList<NoProjectProjectItem::Target> targets;
private slots:
	void accept ();
	void pathDialogRequested ();
	void selectedRowChanged(int);
	void actionNameEdited(QString name);
	void addAction ();
	void removeAction ();
	void actionCommandEdited ();
private:
	NoProjectProjectItem* project;
};

#endif // UINOPROJECTPROJECTSETTINGS_H
