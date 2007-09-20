#ifndef UINOPROJECTPROJECTSETTINGS_H
#define UINOPROJECTPROJECTSETTINGS_H

#include <QHash>

#include "ui_UINoProjectProjectSettings.h"

class NoProjectProjectItem;

class UINoProjectProjectSettings : public QDialog, Ui_UINoProjectProjectSettings
{
	Q_OBJECT
public:
	UINoProjectProjectSettings (NoProjectProjectItem* project);

	QHash <QString,QString> commands;
private slots:
	void accept ();
	void pathDialogRequested ();
	void selectedActionChanged(QString);
	void actionNameEdited(QString name);
	void addAction ();
	void removeAction ();
	void actionCommandEdited ();
private:
	NoProjectProjectItem* project;
};

#endif // UINOPROJECTPROJECTSETTINGS_H
