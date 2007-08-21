#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>

#include <QMenu>
#include <QStackedWidget>
#include <QObject>

#include "interface_plugin.h"



// ============= CLASS PLUGIN_MANAGER ============

class GNUDebuggerPluginManager : public QObject
{
	Q_OBJECT
public:

	GNUDebuggerPluginManager();
	
	// loading plugins for GNU debugger
	void load(QObject * ,QMenu * , QStackedWidget *);
	// save struct
	void copyStruct(GNUDebuggerPluginStruct *);
	// notify to all plug gdb start/ stop
	void notifyGdbStarted(bool a);
	// notify to all plug projet open
	void notifyProjetOpened(bool);
	// find if plug is in struct
	int contains(QWidget *);
	// execute plugins
	void execute(QString st);
	// init or re-init
	void init()
	{
		qpPluginSender = NULL;
		iCurrentPlugin = 0;
		plugin_struct_out.iPluginCommand = -1;
		plugin_struct_out.iCurrentCommand = -1;
	}
	
	
	void setCurrentCommand(int i)
		{plugin_struct_out.iCurrentCommand = i;}

	void setPluginSender(GNUDebuggerPluginInterface *ps)
		{qpPluginSender = ps;}

	// return le pointeur sur le plugin
	GNUDebuggerPluginInterface * plugin(int i)
		{return qlPlugin.at(i).plugin_Pointer;}

	
private:

	// a plug request send data to Gdb 
	GNUDebuggerPluginInterface *  qpPluginSender;
	// data from plugin
	GNUDebuggerPluginStruct plugin_struct_out;
	// add plugin in struct list
	void add(GNUDebuggerPluginInterface *plugin, QWidget *widget);
	// index of current plugin
	int iCurrentPlugin;
	// list of all plug
	struct plugin_list
	{
		// list des pointer sur les plug
		GNUDebuggerPluginInterface *plugin_Pointer;
		// list des Widgets qui seront dans le stacked
		QWidget * plugin_Widget;
	};
	QList <plugin_list> qlPlugin;
};

