#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H
//
#include <QtPlugin>
#include <QLabel>
#include <QDir>
#include <QCoreApplication>
//
#include "Workspace.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT BasePlugin : public QObject
{
	Q_OBJECT
	Q_ENUMS( Type )
	//
public:
	// plugin type enums
	enum Type
	{	iAll = -1,
		iBase,
		iChild,
		iWorkspace,
		iCompiler,
		iDebugger,
		iProject,
		iLast };
	// plugin info structure
	struct Q_MONKEY_EXPORT PluginInfos
	{
		QString Caption; // the string to show as caption
		QString Description; // the plugin description
		BasePlugin::Type Type; // the plugin type
		QString Name; // the plugin name for version control
		QString Version; // the plugin version for version control
		bool Installed; // to know if this plugin is installed
	};
	//
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
	virtual bool isInstalled() const
	{ return mPluginInfos.Installed; }

    //
    virtual void saveCodeCoverage(const QString &) { /* Needs to be implemented in the plugin itself */ }
    static QString  codeCoverageFile()
    {
#if Q_WS_WIN32
      QString monkeypath=QCoreApplication::applicationDirPath();
#else
      QString monkeypath=QDir::homePath ();
#endif
      QString monkeyCoverageDir="monkeystudio_tests";
      QDir monkeyPath(monkeypath);
      if (!monkeyPath.exists(monkeyCoverageDir))
        monkeyPath.mkdir(monkeyCoverageDir);

      return QDir::toNativeSeparators(monkeypath+"/"+monkeyCoverageDir+"/monkey_cov"); 
    }
    // NEED REIMPLEMENTATION
	virtual bool install() = 0;
	virtual bool uninstall() = 0;
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
