#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <InterpreterPlugin.h>

class $Plugin Name$ : public InterpreterPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin InterpreterPlugin CLIToolInterface )

public:
	virtual pCommand defaultCommand() const;
	virtual pCommandList defaultCommands() const;

protected:
	virtual void fillPluginInfos();

	virtual bool install();
	virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
