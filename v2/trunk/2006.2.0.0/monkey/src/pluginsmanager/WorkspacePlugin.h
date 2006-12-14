#ifndef WORKSPACEPLUGIN_H
#define WORKSPACEPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT WorkspacePlugin : public BasePlugin
{
public:
	virtual BasePlugin::Type type() const
	{ return BasePlugin::iWorkspace; }
	//
};
//
Q_DECLARE_INTERFACE( WorkspacePlugin, "org.monkeystudio.MonkeyStudio.WorkspacePlugin/1.0" )
//
#endif // WORKSPACEPLUGIN_H
