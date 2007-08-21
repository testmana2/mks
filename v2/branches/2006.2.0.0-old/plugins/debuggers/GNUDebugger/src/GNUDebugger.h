#ifndef GNUDebugger_H
#define GNUDebugger_H
//
#include "WorkspacePlugin.h"
#include  "MonkeyEditor.h"
//
#include <QTimer>
class UiGNUDebugger;

class GNUDebugger : /*public QObject,*/ public WorkspacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin WorkspacePlugin )
	//
public:
	virtual ~GNUDebugger();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	virtual QWidget* settingsWidget();
	//
private:
	void modifySetting(QString file, QList <int> list);

	QPointer<UiGNUDebugger> mGNUDebugger;
	// list of all breakpoint for all file
	QStringList settingBreakpoint;

	QTimer timer;
	QPixmap breakpoint;
	QPixmap debuggerAt;


public slots:
	void onEditorOpened(MonkeyEditor *);
	void onMarginClicked(MonkeyEditor *,QString, int);
	void onProjectOpened(bool b);
	void onProjectName(QString name);

	void onGotoLine(QString file, int line);

	// enable disable menu
	void onDebuggerStarted();
	void onDebuggerStopped();

	// set breakpoint on editor
	void onSetBreakpointOnOpened();
	
signals:
	void signalSetMarker(MonkeyEditor *, QPixmap , int);
	void signalAddMarker(MonkeyEditor *,  int, int);
	void signalAddMarker(QString,  int, int);
	void signalDelMarker(MonkeyEditor *, int ,int);
	void signalDelAllMarker(MonkeyEditor *,int);
	void signalDelAllMarker(QString ,int);
	void signalGotoLine(QString, int);

};
//
#endif // GNUDebugger_H
