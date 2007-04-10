#ifndef UIPROJECTSETTINGSPROXYMODEL_H
#define UIPROJECTSETTINGSPROXYMODEL_H
//
#include "ui_UIProjectSettingsProxyModel.h"
//
#include <QApplication>
//
class ProjectSettingsProxyModel;
class ProjectItemModel;
//
class UIProjectSettingsProxyModel : public QDialog, public Ui::UIProjectSettingsProxyModel
{
	Q_OBJECT
	//
public:
	static void execute( ProjectItemModel*, QWidget* = QApplication::activeWindow() );
	QModelIndex currentIndex();
	//
protected:
	UIProjectSettingsProxyModel( ProjectItemModel*, QWidget* = 0 );
	~UIProjectSettingsProxyModel();
	//
	ProjectSettingsProxyModel* mProxy;
	ProjectItemModel* mProject;
	//
protected slots:
	void on_tvScopes_clicked( const QModelIndex& );
	void on_tbAdd_clicked();
	void on_tbEdit_clicked();
	void on_tbRemove_clicked();
	void on_tbClear_clicked();
	void on_tbUp_clicked();
	void on_tbDown_clicked();
	virtual void accept();
	virtual void reject();
	//
};
//
#endif // UIPROJECTSETTINGSPROXYMODEL_H
