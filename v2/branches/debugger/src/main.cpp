#include <QApplication>
#include <QMdiArea>
#include <QDockWidget>

#include "MainWindow.h"
#include "CallStackWidget.h"
#include "FileManager.h"
#include "GnuDebugger.h"

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
	
	GnuDebugger debugger;
	// start application
	
	QObject::connect (&debugger, SIGNAL (positionChanged (const QString&, int)),
			          &fileManager, SLOT (gotoFileLine (const QString&, int)));
	QObject::connect (&debugger, SIGNAL (callStackUpdate (const GnuDebugger::CallStack&)),
			          stackWidget, SLOT (update (const GnuDebugger::CallStack&)));
	
	
	
	debugger.prepare_loadTarget();
	window.show();
	return app.exec();
}
