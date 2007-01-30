#ifndef QMAKE_H
#define QMAKE_H
//
#include "ProjectPlugin.h"
//
class QMake : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )
	//
public:
	virtual ~QMake();
	virtual void initialize( Workspace* );
	virtual QWidget* settingsWidget();
	virtual bool install();
	virtual bool uninstall();
	virtual QStringList filters() const;
	virtual bool openProject( const QString&, AbstractProjectProxy* = 0 );
	//
};
//
#endif // QMAKE_H
