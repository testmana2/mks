#ifndef DEBUGGERPLUGIN_H
#define DEBUGGERPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT DebuggerPlugin : public BasePlugin
{
public:
	BasePlugin::Type type() const
	{ return BasePlugin::iDebugger; }
	//
};
//
Q_DECLARE_INTERFACE( DebuggerPlugin, "org.monkeystudio.MonkeyStudio.DebuggerPlugin/1.0" )
//
#endif // DEBUGGERPLUGIN_H
