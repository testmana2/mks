#ifndef UIMONKEYDBG_H
#define UIMONKEYDBG_H

#include "ui_UIMonkeyDbg.h"
#include "QGdbDriver.h"

class UIMonkeyDbg : public QMainWindow, public Ui::UIMonkeyDbg
{
	Q_OBJECT

public:
	UIMonkeyDbg( QWidget* parent = 0 );
	virtual ~UIMonkeyDbg();

protected:
	QGdbDriver* mDebugger;

public slots:
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
	void on_aOpenFile_triggered();
	void on_aLoadTarget_triggered();
};

#endif // UIMONKEYDBG_H
