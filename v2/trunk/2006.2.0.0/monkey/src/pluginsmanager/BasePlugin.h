#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H
//
#include <QtPlugin>
//
#include "Workspace.h"
#include "MonkeyExport.h"
//
struct PluginInfos{
  QString Caption; // the string to show as ut s caption
  int Type; // the plugin type
  QString Name; // the plugin name for version control
  int Version; // the plugin version for version control
}
//
class Q_MONKEY_EXPORT BasePlugin
{
public:
	enum Type { iUnknow = -1, iChild, iWorkspace, iCompiler, iDebugger };
	//
	BasePlugin()
	{ mInstalled = false; mWorkspace = 0; }
	virtual ~BasePlugin() {}
	//
	virtual void initialize( Workspace* w )
	{ Q_ASSERT( w != 0 ); mWorkspace = w; }
	//
	virtual QString name() const = 0;
	virtual QString description() const = 0;
	virtual BasePlugin::Type type() const = 0;
	//
	virtual bool install() = 0;
	virtual bool uninstall() = 0;
	virtual bool isInstalled() const
	{ return mInstalled; }
	//
protected:
	bool mInstalled;
	Workspace* mWorkspace;
	//
};
//
Q_DECLARE_INTERFACE( BasePlugin, "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" )
//
#endif // BASEPLUGIN_H
