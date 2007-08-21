/************************************************************
	GNUDebuggerChild is UI Form
	used by GNUDebugger
************************************************************/
//
#include "GNUDebuggerPluginManager.h"
//
GNUDebuggerPluginManager::GNUDebuggerPluginManager()
{
qpPluginSender = NULL;
}
//
void GNUDebuggerPluginManager::load(QObject * parent,QMenu *contextualMenu , QStackedWidget *stackedWidget)
{
int havePlug=0;

	// find path App
	QCoreApplication *qApp = QCoreApplication::instance();
	QDir pluginsDir = QDir(qApp->applicationDirPath());
	// go to plugin directory
//	pluginsDir.cd("plugins/debugger");

	// find all plug
	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        	QObject *op = loader.instance();
        	if (op) 
		{
			GNUDebuggerPluginInterface * pPlugin = qobject_cast<GNUDebuggerPluginInterface *>(op);
            		if (pPlugin) 
			{
				qDebug(QString("\tGNUDebugger loading : " + fileName).toAscii());
				QWidget * pWidget = pPlugin->pluginGetWidget();
				// save plugin in struct
				add(pPlugin, pWidget);
			
				// plug have a Form ?
				if(pWidget != NULL)	// add on menu
				{
					havePlug++;
					QAction *ac = new QAction(pPlugin->pluginMenuIcon(),pPlugin->pluginMenuName(),pWidget);
					// Add shorcut to menu cotextual
					contextualMenu->addAction(ac);
					// Add plug to StackedWidget
					stackedWidget->addWidget(pWidget);
					// connect right click
					connect(op,SIGNAL(signalPluginMouse(QMouseEvent * )),parent,SLOT(onPluginMouse(QMouseEvent *)));
				}
				// connect all signal from plugin to OnPluginAction
				connect(op,SIGNAL(signalPluginAction( GNUDebuggerPluginInterface *, GNUDebuggerPluginStruct *  )),parent,SLOT(onPluginAction( GNUDebuggerPluginInterface * ,GNUDebuggerPluginStruct *)));
			}
            	}
        }
	// connect selected plug in menu contextual to onPluginMenuAction function
	connect(contextualMenu,SIGNAL(triggered ( QAction * )), parent,SLOT(onPluginMenuAction(QAction * )));

	// show first plug with Form in StackedWidget
	if(havePlug) 	
	{
		// find first plug have widget
		for(int i=0; i< qlPlugin.count(); i++)
		{
			if(qlPlugin.at(i).plugin_Widget != NULL)
			{
				stackedWidget->setCurrentWidget(qlPlugin.at(i).plugin_Widget);
				return;
			}
		}
	}
}
//
// Save struct from plugin
void GNUDebuggerPluginManager::copyStruct(struct GNUDebuggerPluginStruct *src)
{
	plugin_struct_out.qsCurrentCommand.clear();
	plugin_struct_out.qsCurrentCommand = src->qsCurrentCommand;
	plugin_struct_out.iCurrentCommandType = src->iCurrentCommandType;
	plugin_struct_out.iPluginCommand = src->iPluginCommand;
}
//
// Notify all plugin Gdb started
void GNUDebuggerPluginManager::notifyGdbStarted(bool a)
{
		for(int i=0; i<qlPlugin.count(); i++)
			qlPlugin.at(i).plugin_Pointer->pluginGdbStarted(a);
}
//
// Notify all plugin ProjetOpened
void GNUDebuggerPluginManager::notifyProjetOpened(bool b)
{
		for(int i=0; i<qlPlugin.count(); i++)
			qlPlugin.at(i).plugin_Pointer->pluginInitWidget();
}
//
// Save plugin in list
void GNUDebuggerPluginManager::add(class GNUDebuggerPluginInterface *plugin, QWidget *widget)
{
struct plugin_list  pl;

	pl.plugin_Pointer =  plugin;
	pl.plugin_Widget = widget;
	qlPlugin << pl;
}
//
// Find in list if contains W
int GNUDebuggerPluginManager::contains(QWidget *w)
{
	for(int i=0 ; i<qlPlugin.count(); i++)
	{
		if(qlPlugin.at(i).plugin_Widget == w && qlPlugin.at(i).plugin_Widget!=NULL)
			return i;
	}
	return -1;
}
//
// Execute plugin
void GNUDebuggerPluginManager::execute(QString st)
{
	// gestion des plugins
	plugin_struct_out.qsDataFromGdb.clear();
	plugin_struct_out.qsDataFromGdb = st;


	if(qpPluginSender != NULL)
	{
		
//qDebug(QString("Execute plug by sender: " + qpPluginSender->pluginName()).toAscii());

		switch(qpPluginSender->pluginDataFromGdb(&plugin_struct_out))
		{
			case PLUGIN_TERMINED : 
			{
//printQstring(qpPluginSender->pluginName() + " return TERMINED");

				iCurrentPlugin++;
				plugin_struct_out.iPluginCommand =   -1;
				qpPluginSender = NULL;
				break;
			}
	
			case PLUGIN_SEND_COMMAND : 
			case PLUGIN_WAIT : 
//printQstring(qpPluginSender->pluginName() +" return WAIT or SEND_COMMAND");
			return;

			case PLUGIN_NOTIFY : 
//printQstring(qpPluginSender->pluginName() + "Plug return NOTIFY");
				plugin_struct_out.iCurrentCommand = DEBUGGER_NOTIFY;
				plugin_struct_out.iPluginCommand =   -1;
				iCurrentPlugin = 0;
				qpPluginSender = NULL;
		}
	}

	for( ; iCurrentPlugin<qlPlugin.count(); )
	{
//	qDebug(QString("execute " + qlPlugin.at(iCurrentPlugin).plugin_Pointer->pluginName()).toAscii());
		switch( qlPlugin.at(iCurrentPlugin).plugin_Pointer->pluginDataFromGdb(&plugin_struct_out) )
		{
			case PLUGIN_TERMINED : // plug termine
			{
//				qDebug("return termined");
				iCurrentPlugin++;
  				break;
			}
			case PLUGIN_WAIT :  // attend la suite
			case PLUGIN_SEND_COMMAND : 
//qDebug("return wait or send command");
			return; // ce plug demande une commande 
		}
	}
	init();
}
