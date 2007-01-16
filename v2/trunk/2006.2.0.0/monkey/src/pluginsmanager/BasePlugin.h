#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H
//
#include <QtPlugin>
#include <QLabel>
//
#include "Workspace.h"
#include "MonkeyExport.h"
//
struct Q_MONKEY_EXPORT PluginInfos
{
	enum PluginType { iAll = -1, iChild, iWorkspace, iCompiler, iDebugger, iProject, iLast };
	QString Caption; // the string to show as ut s caption
	QString Description; // the plugin description
	PluginInfos::PluginType Type; // the plugin type
	QString Name; // the plugin name for version control
	QString Version; // the plugin version for version control
	bool Installed; // to know if this plugin is installed
};
//
class Q_MONKEY_EXPORT BasePlugin
{
public:
	BasePlugin()
	{ mPluginInfos.Installed = false; mWorkspace = 0; }
	virtual ~BasePlugin() {}
	//
	virtual void initialize( Workspace* w )
	{ Q_ASSERT( w != 0 ); mWorkspace = w; }
	//
	virtual PluginInfos infos() const
	{ return mPluginInfos; }
	//
	virtual QWidget* settingsWidget()
	{ return new QLabel( QObject::tr( "This plugin can't be configured" ) ); }
	//
	virtual bool install() = 0;
	virtual bool uninstall() = 0;
	virtual bool isInstalled() const
	{ return mPluginInfos.Installed; }
	//
protected:
	PluginInfos mPluginInfos;
	Workspace* mWorkspace;
	//
};
//
Q_DECLARE_INTERFACE( BasePlugin, "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" )
//
#endif // BASEPLUGIN_H
