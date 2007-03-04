#ifndef GNUMAKE_H
#define GNUMAKE_H
//
#include "CompilerPlugin.h"
//
class AbstractProjectItemModel;
//
class GNUMake :  public CompilerPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin CompilerPlugin )
	//
public:
    GNUMake();
	virtual ~GNUMake();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	virtual QWidget* settingsWidget();
	//
public slots:
	virtual void buildCurrent();
	virtual void buildAll();
	virtual void reBuildCurrent();
	virtual void reBuildAll();
	virtual void stop();
	virtual void cleanCurrent();
	virtual void cleanAll();
	virtual void distCleanCurrent();
	virtual void distCleanAll();
	virtual void execute();
	virtual void executeWithParameters();
	virtual void buildExecute();
	virtual void distCleanBuildExecute();
	//
private:
	bool checkForProject( AbstractProjectItemModel* );
	AbstractProjectItemModel* currentProject();
	AbstractProjectItemModel* parentProject();
	//
	ConsoleCommand qmakeCommand( AbstractProjectItemModel* );
	ConsoleCommand makeCommand( AbstractProjectItemModel* );
	ConsoleCommand makeCleanCommand( AbstractProjectItemModel* );
	ConsoleCommand makeDistCleanCommand( AbstractProjectItemModel* );
	ConsoleCommand executeCommand( AbstractProjectItemModel*, const QString& = QString::null );
	//
};
//
#endif // GNUMAKE_H
