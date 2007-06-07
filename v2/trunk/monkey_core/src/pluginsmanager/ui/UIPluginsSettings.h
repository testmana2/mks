#ifndef UIPLUGINSSETTINGS_H
#define UIPLUGINSSETTINGS_H
//
#include "ui_UIPluginsSettings.h"
//
#include <QPointer>
//
class PluginsManager;
//
class UIPluginsSettings : public QDialog, public Ui::UIPluginsSettings
{
	Q_OBJECT
	//
public:
	static UIPluginsSettings* self( PluginsManager*, QWidget* = 0 );
	void clearInfos();
	void updateList();
	//
private:
	UIPluginsSettings( PluginsManager*, QWidget* = 0 );
	static QPointer<UIPluginsSettings> mSelf;
	PluginsManager* mPluginsManager;
	//
private slots:
	void currentIndexChanged( int );
	void itemClicked( QListWidgetItem* );
	void on_cbAutoInstall_clicked( bool );
	void on_cbInstallUninstall_clicked( bool );
	virtual void accept();
	//
};
//
#endif // UIPLUGINSSETTINGS_H
