#include <QApplication>
#include "MainWindow.h"
#include "GnuDebugger.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "Monkey Debugger" );

	MainWindow window;
	window.setWindowTitle( "Monkey Debugger" );
	window.show();

	GnuDebugger debugger;
	// start application
	return app.exec();
}
