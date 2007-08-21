#ifndef plug_H
#define plug_H

#include <QPointer>
#include <QWidget>
#include <QMouseEvent>
#include <QIcon>

#include "interface_plugin.h"



class DebuggerAt :  public QObject, public GNUDebuggerPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(GNUDebuggerPluginInterface)
 
public:
	QPointer <QWidget> pluginGetWidget();
	void pluginInitWidget();	

	QString pluginMenuName();
	QIcon pluginMenuIcon();
	QString pluginName();
	QStringList pluginInfos();
	QDialog * pluginSetting();
 
	int  pluginDataFromGdb(struct GNUDebuggerPluginStruct * debugger_struct);
	void pluginGdbStarted(bool);
private :
	struct GNUDebuggerPluginStruct debugger_struct_out;

signals : 
	void signalPluginAction(GNUDebuggerPluginInterface* , struct GNUDebuggerPluginStruct *debugger_struct);
	void signalPluginMouse(QMouseEvent *);
};

#endif
