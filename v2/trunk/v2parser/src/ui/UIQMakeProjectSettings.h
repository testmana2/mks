#ifndef UIQMAKEPROJECTSETTINGS_H
#define UIQMAKEPROJECTSETTINGS_H
//
#include "ui_UIQMakeProjectSettings.h"
//
#include <QApplication>
//
class QMakeProjectScopesProxy;
class QMakeProjectModel;
//
class UIQMakeProjectSettings : public QDialog, public Ui::UIQMakeProjectSettings
{
	Q_OBJECT
	//
public:
	static void execute( QMakeProjectModel*, QWidget* = QApplication::activeWindow() );
	QModelIndex currentIndex();
	void setCurrentIndex( const QModelIndex& );
	//
protected:
	UIQMakeProjectSettings( QMakeProjectModel*, QWidget* = 0 );
	~UIQMakeProjectSettings();
	//
	QMakeProjectScopesProxy* mProxy;
	QMakeProjectModel* mProject;
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
#endif // UIQMAKEPROJECTSETTINGS_H
