#ifndef COMPILERPLUGIN_H
#define COMPILERPLUGIN_H
//
#include "BasePlugin.h"
//
class CompilerPlugin : public BasePlugin
{
public:
	virtual BasePlugin::Type type() const
	{ return BasePlugin::iCompiler; }
	//
};
//
Q_DECLARE_INTERFACE( CompilerPlugin, "org.monkeystudio.MonkeyStudio.CompilerPlugin/1.0" )
//
#endif // COMPILERPLUGIN_H
