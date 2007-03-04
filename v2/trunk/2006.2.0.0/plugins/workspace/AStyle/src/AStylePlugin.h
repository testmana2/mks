#ifndef ASTYLEPLUGIN_H
#define ASTYLEPLUGIN_H
//
#include "WorkspacePlugin.h"
//
class UISettingsAStyle;
class QAction;
//
class AStylePlugin : public WorkspacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )
	//
public:
    AStylePlugin();
    virtual ~AStylePlugin();
	virtual void initialize( Workspace* );
	virtual QWidget* settingsWidget();
	virtual bool install();
	virtual bool uninstall();
	//
private:
	QAction* aAStyle;
	//
private slots:
	void applyFormatter();
	//
};
//
#endif // ASTYLEPLUGIN_H
