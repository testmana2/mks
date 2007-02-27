#ifndef plug_H
#define plug_H

#include <QTextEdit>
#include <QPointer>
#include <QWidget>
#include <QMouseEvent>
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QIcon>

#include "interface_plugin.h"

class QTextEditEx: public QTextEdit
{
	Q_OBJECT
signals:
	void signalPluginMouse(QMouseEvent *);

public slots :

	void mousePressEvent ( QMouseEvent * event ) ;

};


class LogPlugin :  public QObject, public GNUDebuggerPluginInterface
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
	struct GNUDebuggerPluginStruct debugger_struct_out;

	QTextEditEx *teGdbLog;
	QFrame *frLog;
	QBoxLayout *vBoxLayoutLog;;
	QBoxLayout *hBoxLayoutLog;
	QLineEdit *leLog;
	QLabel *laLog;

private slots:
	void onUserCommand();

signals : 
	void signalPluginAction(GNUDebuggerPluginInterface * ,struct GNUDebuggerPluginStruct *debugger_struct);
	void signalPluginMouse(QMouseEvent *);
};

#endif
