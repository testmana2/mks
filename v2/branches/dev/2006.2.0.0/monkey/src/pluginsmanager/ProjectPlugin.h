#ifndef PROJECTPLUGIN_H
#define PROJECTPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
#include <QStringList>
//
class AbstractProjectProxy;
//
class Q_MONKEY_EXPORT ProjectPlugin : public BasePlugin
{
	Q_OBJECT
	//
public:
	virtual QStringList extensions() const
	{ return mExtensions; }
	virtual QStringList filters() const = 0;
	virtual bool openProject( const QString& ) = 0;
	//
protected:
	QStringList mExtensions;
	//
};
//
Q_DECLARE_INTERFACE( ProjectPlugin, "org.monkeystudio.MonkeyStudio.ProjectPlugin/1.0" )
//
#endif // PROJECTPLUGIN_H
