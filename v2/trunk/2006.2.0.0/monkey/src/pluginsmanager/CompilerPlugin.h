#ifndef COMPILERPLUGIN_H
#define COMPILERPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
#include <QObject>
//
class ConsoleCommand;
typedef QList<ConsoleCommand> ConsoleCommands;
//
class Q_MONKEY_EXPORT CompilerPlugin : public BasePlugin
{
	Q_OBJECT
	//
public slots:
	virtual void buildCurrent() = 0;
	virtual void buildAll() = 0;
	virtual void reBuildCurrent() = 0;
	virtual void reBuildAll() = 0;
	virtual void stop() = 0;
	virtual void cleanCurrent() = 0;
	virtual void cleanAll() = 0;
	virtual void distCleanCurrent() = 0;
	virtual void distCleanAll() = 0;
	virtual void execute() = 0;
	virtual void executeWithParameters() = 0;
	virtual void distCleanBuildExecute() = 0;
	//
signals:
	void messageBox( const QString& );
	void clearMessageBox();
	void runConsoleCommands( ConsoleCommands* );
	void stopConsole();
	//
};
//
Q_DECLARE_INTERFACE( CompilerPlugin, "org.monkeystudio.MonkeyStudio.CompilerPlugin/1.0" )
//
#endif // COMPILERPLUGIN_H
