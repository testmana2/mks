#ifndef APPDEBUG_H
#define APPDEBUG_H

#include <BasePlugin.h>

class AppDebug : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

protected:
	virtual void fillPluginInfos();

	virtual bool install();
	virtual bool uninstall();
	
	static void qtMessageHandler( QtMsgType type, const char* msg );
};

#endif // APPDEBUG_H
