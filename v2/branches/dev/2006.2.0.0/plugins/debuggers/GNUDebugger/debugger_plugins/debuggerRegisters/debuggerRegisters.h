#ifndef plug_H
#define plug_H

#include <QTableWidget>
#include <QPointer>
#include <QWidget>
#include <QMouseEvent>


#include "interface_plugin.h"
 
// just for mousePressEvent redirection
class QTableWidgetEx : public QTableWidget
{
	Q_OBJECT
public slots:
	void mousePressEvent(QMouseEvent *e);

protected:
	 void resizeEvent ( QResizeEvent * event ) ;

signals:
	void signalPluginMouse(QMouseEvent *);
};



class RegisterPlugin :  public QObject, public GNUDebuggerPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(GNUDebuggerPluginInterface)
 
public:
	// call when your plugin is loaded
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
	bool bGdbStarted;
	bool bNotify;

	struct GNUDebuggerPluginStruct debugger_struct_out;

	QTableWidgetEx *twRegister;
	void registerUpdate(QString st);
	bool registerShowAll(QString st);
	void registerIsVariableChangeValue(QTableWidgetItem *id ,QString regValue);
	QString registerGetName(QString st);
	QString registerGetValue(QString st);
private slots:
	void registersChanged(QTableWidgetItem  *id);

signals : 
	void signalPluginAction(GNUDebuggerPluginInterface * ,struct GNUDebuggerPluginStruct *debugger_struct);
	void signalPluginMouse(QMouseEvent *);
};

#endif
