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
	
protected:
	QLabel* mStatusLabel;
	
protected slots:
	void loadTargetTriggered();

public slots:
	void appendLog( const QString& log );
	void onDebuggerCallbackMessage( const QString& message, QGdbDriver::CBType type );
	void onDebuggerStateChanged (QGdbDriver::State);
	
	
signals:
	
	// api
	void loadTargetRequested (const QString& fileName);
	
	void exec_runTriggered();
	void exec_continueTriggered();
	void exec_stepIntoTriggered();
	void exec_stepOverTriggered();
	void exec_stepOutTriggered();
	void exec_pauseTriggered();
	void exec_killTriggered();
};

#endif // MAINWINDOW_H
