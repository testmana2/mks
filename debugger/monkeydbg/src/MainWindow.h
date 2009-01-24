#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QGdbDriver.h"

#include "ui_MainWindow.h"

class QLabel;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow( QWidget* parent = 0 );
	virtual ~MainWindow();
	
	void loadTarget( const QString& fileName );
	
protected:
	QLabel* mStatusLabel;
	
protected slots:
	void loadTargetTriggered();

public slots:
	void appendLog( const QString& log );
	void appendConsole( const QString& msg );
	void appendPipe( const QString& msg );
	void onDebuggerCallbackMessage( const QString& message, QGdbDriver::CBType type );
	void onDebuggerSignalReceived( const QGdbDriver::Signal& signal );
	void onDebuggerStateChanged (QGdbDriver::State);
	
signals:
	void breakpointToggled( const QString& fileName, int line, bool& remove );
	
	// api
	void loadTargetRequested (const QString& fileName);
	
	void openFileTriggered ();
	void closeFileTriggered ();
	
	void exec_runTriggered();
	void exec_continueTriggered();
	void exec_stepIntoTriggered();
	void exec_stepOverTriggered();
	void exec_stepOutTriggered();
	void exec_pauseTriggered();
	void exec_killTriggered();
};

#endif // MAINWINDOW_H
