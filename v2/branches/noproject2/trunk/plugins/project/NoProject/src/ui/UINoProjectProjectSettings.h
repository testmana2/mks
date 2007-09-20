#ifndef UINOPROJECTPROJECTSETTINGS_H
#define UINOPROJECTPROJECTSETTINGS_H

#include <QHash>

#include "ui_UINoProjectProjectSettings.h"
#include "ProjectItem.h"

class NoProjectProjectItem;

class UINoProjectProjectSettings : public QDialog, Ui_UINoProjectProjectSettings
{
	Q_OBJECT
public:
	UINoProjectProjectSettings (NoProjectProjectItem* project);

	QList<ProjectItem::BuildAction> targets;
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
