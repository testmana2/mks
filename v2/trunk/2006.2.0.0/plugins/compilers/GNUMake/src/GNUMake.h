#ifndef GNUMAKE_H
#define GNUMAKE_H
//
#include "CompilerPlugin.h"
#include "AbstractProjectModel.h"
//
class GNUMake :  public CompilerPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin CompilerPlugin )
	//
public:
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
	bool checkForProject( const QModelIndex& );
	AbstractProjectModel* currentModel() const;
	QModelIndex currentProject() const;
	QModelIndex parentProject() const;
	//
	ConsoleCommand qmakeCommand( const QModelIndex& );
	ConsoleCommand makeCommand( const QModelIndex& );
	ConsoleCommand makeCleanCommand( const QModelIndex& );
	ConsoleCommand makeDistCleanCommand( const QModelIndex& );
	ConsoleCommand executeCommand( const QModelIndex&, const QString& = QString::null );
	//
};
//
#endif // GNUMAKE_H
