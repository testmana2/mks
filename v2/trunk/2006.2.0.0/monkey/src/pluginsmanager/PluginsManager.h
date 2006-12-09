#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H
//
#include <QObject>
#include <QPointer>
#include <QDir>
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
class AbstractProject;
//
class Q_MONKEY_EXPORT PluginsManager : public QObject
{
	Q_OBJECT
	//
public:
	static PluginsManager* self( QObject* = 0 );
	~PluginsManager();
	//
	void loadsPlugins( const QString& = QString() );
	void loadsPlugins( QDir );
	bool addPlugin( QObject* );
	//
	bool childPluginOpenFile( const QString&, AbstractProject* = 0 );
	QStringList childsFilters() const;
	//
private:
	PluginsManager( QObject* = 0 );
	//
	static QPointer<PluginsManager> mSelf;
	QList<BasePlugin*> mPlugins;
	//
};
//
#endif // PLUGINSMANAGER_H
