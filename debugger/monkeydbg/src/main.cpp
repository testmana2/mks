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
	
	//    Driver
	QGdbDriver debugger;
	
	//    Main window
	MainWindow window;
	window.setWindowTitle( "Monkey Debugger" );
	
	//    Mdi area
	QMdiArea* mdi = new QMdiArea();
	mdi->setViewMode( QMdiArea::SubWindowView );
	window.setCentralWidget (mdi);
	
	//    File manager
	FileManager fileManager (&app, mdi);
	
	//    Callstack widget
	QDockWidget* stackDock = new QDockWidget ("Call stack");
	CallStackWidget* stackWidget = new CallStackWidget (&debugger);
	stackDock->setWidget (stackWidget);
	window.addDockWidget (Qt::BottomDockWidgetArea, stackDock);
	
	// driver -> window
	QObject::connect (&debugger, SIGNAL (callbackMessage(const QString&, QGdbDriver::CBType)),
					  &window, SLOT (onDebuggerCallbackMessage (const QString&, QGdbDriver::CBType)));
	QObject::connect (&debugger, SIGNAL (stateChanged (QGdbDriver::State)),
						&window, SLOT (onDebuggerStateChanged (QGdbDriver::State)));
	QObject::connect (&window, SIGNAL (loadTargetRequested (const QString&)),
					&debugger, SLOT (exec_setCommand (const QString&)));					  
	// window -> driver
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
	
	// driver -> filemanager
	QObject::connect (&debugger, SIGNAL (positionChanged (const QString&, int)),
					&fileManager, SLOT (gotoFileLine (const QString&, int)));
	
	// filemanager -> window
	QObject::connect( &fileManager, SIGNAL( marginClicked( int, int, Qt::KeyboardModifiers ) ), 
					  &window, SLOT( onMarginClicked( int, int, Qt::KeyboardModifiers ) ) );
	
	// window -> filemanager
	QObject::connect( &window, SIGNAL( openFileTriggered() ), 
					  &fileManager, SLOT( openFileTriggered() ) );
	QObject::connect( &window, SIGNAL( closeFileTriggered() ), 
					  &fileManager, SLOT( closeFileTriggered() ) );
	
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
