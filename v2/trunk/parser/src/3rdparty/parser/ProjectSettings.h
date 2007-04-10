#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QDialog>
#include <QDir>
#include "ui_ProjectSettings.h"

class ProjectItemModel;

class ProjectSettings : public QDialog, public Ui::ProjectSettings
{
	Q_OBJECT
public:
	ProjectSettings(ProjectItemModel*);
private:
	ProjectItemModel* mModel;
	QDir currDir;
	//
	void initialize();
	QStringList getSettingsList(const QString&);
	void setSettingsList(	const QString&,	const QStringList&,	QString = QString());
	QString relativeFilePath(const QString&);
	void setSubdirs(bool);
private slots:
	void slot_rbClicked(bool);
	void slot_QtClicked(bool);
	void slot_changeTemplate(int);
	void fileDoubleClicked( QTreeWidgetItem * , int );
	void initExpWindow(/*int = 0*/);
	void on_cbIncludeFile_currentIndexChanged( const QString& );
	void on_pbAdd_clicked();
	void on_pbClear_clicked();
	void on_pbDelete_clicked();
	void on_pbEdit_clicked();
	void on_tbDestDir_clicked();
	void itemTextChanged(QListWidgetItem*);
	void save_lwContents_changes();
	//void enterDirectory(const QModelIndex &);
};

#endif //PROJECTSETTINGS_H