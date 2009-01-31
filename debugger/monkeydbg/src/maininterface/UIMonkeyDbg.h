#ifndef UIMONKEYDBG_H
#define UIMONKEYDBG_H

#include "ui_UIMonkeyDbg.h"
#include "QGdb-Driver.h"

#include <QMap>

class QMdiSubWindow;

typedef QPair<QListWidgetItem*, QMdiSubWindow*> ItemSubWindow;

class UIMonkeyDbg : public QMainWindow, public Ui::UIMonkeyDbg
{
	Q_OBJECT

public:
	UIMonkeyDbg( QWidget* parent = 0 );
	virtual ~UIMonkeyDbg();

protected:
	QGdb::Driver* mDebugger;
	bool mDebuggerInitialized;
	QMap<QString, ItemSubWindow> mOpenedFiles;
	
	void showEvent( QShowEvent* event );
	void initConnections();

public slots:
	void appendLog( const QString& log );
	void appendConsole( const QString& msg );
	void appendPipe( const QString& msg );
	
	bool openFile( const QString& fileName );
	void closeCurrentFile();
	void closeAllFiles();
	
	bool loadTarget( const QString& fileName );
	void debuggerRun();
	void debuggerContinue();
	void debuggerStepInto( bool instruction = false );
	void debuggerStepOver( bool instruction = false );
	void debuggerStepOut();
	void debuggerStop();
	void debuggerKill();

protected slots:
	// debugger
	void debuggerCallbackMessage( const QString& message, QGdb::CBType type );
	void debuggerStateChanged( QGdb::State state );
	void debuggerSignalReceived( const QGdb::Signal& signal );
	void debuggerExitSignalReceived( const QGdb::Signal& signal );
	void debuggerExited( int code );
	
	// gui
	void subWindow_destroyed( QObject* object );
	void on_lwFiles_itemActivated( QListWidgetItem* item );
	void on_maWorkspace_subWindowActivated( QMdiSubWindow* subWindow );
	void on_aOpenFile_triggered();
	void on_aLoadTarget_triggered();
	void on_aStepInto_triggered();
	void on_aStepOver_triggered();
};

#endif // UIMONKEYDBG_H
