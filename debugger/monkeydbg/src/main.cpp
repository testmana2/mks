#include <QApplication>
#include <QMdiArea>
#include <QDockWidget>
#include <QFileInfo>
#include <QMessageBox>

#include "maininterface/UIMonkeyDbg.h"

#include "MainWindow.h"
#include "CallStackWidget.h"
#include "BreakpointWidget.h"
#include "FileManager.h"
#include "QGdbDriver.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "Monkey Debugger" );
	
	UIMonkeyDbg dbg;
	dbg.show();
	
	//    Driver
	QGdbDriver debugger;
	
	//    Main window
	MainWindow window;
	window.setWindowTitle( "Monkey Debugger" );
	
	//    Mdi area
	QMdiArea* mdi = new QMdiArea( &window );
	mdi->setViewMode( QMdiArea::SubWindowView );
	window.setCentralWidget( mdi );
	
	//    File manager
	FileManager fileManager( &app, mdi );
	
	// breakpoints widget
	QDockWidget* breakpointDock = new QDockWidget( "Breakpoints" );
	BreakpointWidget* breakpointWidget = new BreakpointWidget( &debugger, breakpointDock );
	breakpointDock->setWidget( breakpointWidget );
	window.addDockWidget( Qt::TopDockWidgetArea, breakpointDock );
	
	//    Callstack widget
	QDockWidget* stackDock = new QDockWidget ("Call stack");
	CallStackWidget* stackWidget = new CallStackWidget( &debugger, stackDock );
	stackDock->setWidget( stackWidget );
	window.addDockWidget( Qt::TopDockWidgetArea, stackDock );
	
	// driver -> window
	QObject::connect (&debugger, SIGNAL (callbackMessage(const QString&, QGdb::CBType)),
						&window, SLOT (onDebuggerCallbackMessage (const QString&, QGdb::CBType)));
	QObject::connect (&debugger, SIGNAL (exitSignalReceived( const QGdb::Signal& )),
						&window, SLOT (onDebuggerExitSignalReceived( const QGdb::Signal& )));
	QObject::connect (&debugger, SIGNAL (signalReceived( const QGdb::Signal& )),
						&window, SLOT (onDebuggerSignalReceived( const QGdb::Signal& )));
	QObject::connect (&debugger, SIGNAL (stateChanged (QGdb::State)),
						&window, SLOT (onDebuggerStateChanged (QGdb::State)));
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
	QObject::connect( &window, SIGNAL( breakpointToggled( const QString&, int ) ), 
					&debugger, SLOT( break_breakpointToggled( const QString&, int ) ) );
	
	// driver -> filemanager
	QObject::connect (&debugger, SIGNAL (positionChanged (const QString&, int)),
					&fileManager, SLOT (setDebuggerPosition (const QString&, int)));
	QObject::connect (&debugger, SIGNAL (breakpointsCleared ( const QString&, int )),
					&fileManager, SLOT (clearBreakpoints ( const QString&, int )));
	QObject::connect (&debugger, SIGNAL (breakpointAdded( const QGdb::Breakpoint& )),
					&fileManager, SLOT (breakpointAdded( const QGdb::Breakpoint& )));
	QObject::connect (&debugger, SIGNAL (breakpointRemoved( const QGdb::Breakpoint& )),
					&fileManager, SLOT (breakpointRemoved( const QGdb::Breakpoint& )));
	
	// filemanager -> window
	QObject::connect( &fileManager, SIGNAL( breakpointToggled( const QString&, int ) ), 
						&window, SIGNAL( breakpointToggled( const QString&, int ) ) );
	
	// window -> filemanager
	QObject::connect( &window, SIGNAL( openFileTriggered() ), 
					&fileManager, SLOT( openFileTriggered() ) );
	QObject::connect( &window, SIGNAL( closeFileTriggered() ), 
					&fileManager, SLOT( closeFileTriggered() ) );
	QObject::connect( &window, SIGNAL( clearDebuggerPositionRequested() ), 
					&fileManager, SLOT( clearDebuggerPosition() ) );
	
	// call stack -> filemanager
	QObject::connect (stackWidget, SIGNAL (frameSelected(const QGdb::CallStackFrame&)),
						&fileManager, SLOT (frameSelected (const QGdb::CallStackFrame&)));
		
		
		if ( argc == 2 )
		{
		if ( QFileInfo( argv[1] ) .isExecutable() )
			debugger.exec_setCommand( argv[1] );
		else 
			QMessageBox::critical( 0, QObject::tr( "Invalid parameter" ), QObject::tr( "%1 is not a executable file" ).arg( argv[1] ) );
	}
	
	// speed hack
	window.loadTarget( "/home/pasnox/Development/Qt4/mks/crashapp/crashapp_debug" );
	fileManager.openFile( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp" );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 35 );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 37 );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 39 );
	
	//window.loadTarget( "/home/a/code/mks/debugger/libmigdb/examples/test_target" );
	//debugger.break_setBreakpoint( "/home/a/code/mks/debugger/libmigdb/examples/test_target.cc", 10 );
	window.showMaximized();
	
	return app.exec();
}
