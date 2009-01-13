#include <QApplication>
#include <QMdiArea>

#include "MainWindow.h"
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
	
	FileManager fileManager (&app, mdi);
	
	GnuDebugger debugger;
	// start application
	
	QObject::connect (&debugger, SIGNAL (positionChanged (const QString&, int)),
			          &fileManager, SLOT (gotoFileLine (const QString&, int)));
	
	debugger.prepare_loadTarget();
	window.show();
	return app.exec();
}
