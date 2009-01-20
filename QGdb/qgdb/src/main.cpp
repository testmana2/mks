#include <QApplication>
#include <QMdiArea>
#include <QDockWidget>
#include <QFileInfo>
#include <QMessageBox>

#include "MainWindow.h"
#include "CallStackWidget.h"
#include "FileManager.h"
#include "QGdbDriver.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "Monkey Debugger" );

	MainWindow window;
	window.setWindowTitle( "Monkey Debugger" );

	QMdiArea* mdi = new QMdiArea();
	mdi->setViewMode (QMdiArea::TabbedView);
	window.setCentralWidget (mdi);
	
	QDockWidget* stackDock = new QDockWidget ("Call stack");
	CallStackWidget* stackWidget = new CallStackWidget ();
	stackDock->setWidget (stackWidget);
	
	window.addDockWidget (Qt::BottomDockWidgetArea, stackDock);
	
	FileManager fileManager (&app, mdi);
	
	QGdbDriver debugger;
	// start application
	
	QObject::connect (&debugger, SIGNAL (callbackMessage(const QString&, QGdbDriver::CBType)),
						&window, SLOT (onDebuggerCallbackMessage (const QString&, QGdbDriver::CBType)));
	QObject::connect (&debugger, SIGNAL (stateChanged (QGdbDriver::State)),
						&window, SLOT (onDebuggerStateChanged (QGdbDriver::State)));
		QObject::connect (&window, SIGNAL (loadTargetRequested (const QString&)),
						&debugger, SLOT (exec_setCommand (const QString&)));					  
						
		QObject::connect (&window, SIGNAL (exec_runTriggered ()),
						&debugger, SLOT (exec_run ())); 
		QObject::connect (&window, SIGNAL (exec_continueTriggered ()),
						&debugger, SLOT (exec_continue ())); 
		QObject::connect (&window, SIGNAL (exec_stepIntoTriggered ()),
						&debugger, SLOT (exec_stepInto ())); 
		QObject::connect (&window, SIGNAL (exec_stepOverTriggered ()),
						&debugger, SLOT (exec_stepOver ())); 
		QObject::connect (&window, SIGNAL (exec_stepOutTriggered ()),
						&debugger, SLOT (exec_stepOut ())); 
		QObject::connect (&window, SIGNAL (exec_pauseTriggered ()),
						&debugger, SLOT (exec_pause ())); 
		QObject::connect (&window, SIGNAL (exec_killTriggered ()),
						&debugger, SLOT (exec_kill ())); 
		
		QObject::connect (&debugger, SIGNAL (positionChanged (const QString&, int)),
						&fileManager, SLOT (gotoFileLine (const QString&, int)));
		QObject::connect (&debugger, SIGNAL (callStackUpdate (const QGdbDriver::CallStack&)),
						stackWidget, SLOT (update (const QGdbDriver::CallStack&)));
		
		if (argc == 2)
		{
		if (QFileInfo (argv[1]).isExecutable())
			debugger.exec_setCommand(argv[1]);
		else 
			QMessageBox::critical (NULL, QObject::tr ("Invalid parameter"), 
											QObject::tr ("%1 is not a executable file").arg(argv[1]));
		}
		
		// test scenario
		//debugger.exec_setCommand("/home/pasnox/Development/Qt4/mks/crashapp/crashapp_debug");
		//debugger.break_setBreaktoint ("main.cpp", 14);
		//debugger.exec_run();
		
		
		window.show();
		return app.exec();
}
