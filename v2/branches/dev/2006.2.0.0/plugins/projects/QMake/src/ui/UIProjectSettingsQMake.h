#ifndef UIPROJECTSETTINGSQMAKE_H
#define UIPROJECTSETTINGSQMAKE_H
//
#include "ui_UIProjectSettingsQMake.h"
//
#include <QApplication>
//
class QMakeProjectProxy;
//
class UIProjectSettingsQMake : public QDialog, public Ui::UIProjectSettingsQMake
{
	Q_OBJECT
	//
public:
	static void execute( QMakeProjectProxy*, QWidget* = QApplication::activeWindow() );
	//
private:
	UIProjectSettingsQMake( QMakeProjectProxy*, QWidget* = 0 );
	QMakeProjectProxy* mProxy;
	//
private slots:
	void on_tvScopes_clicked( const QModelIndex& );
	virtual void accept();
	virtual void reject();
	//
};
//
#endif // UIPROJECTSETTINGSQMAKE_H
