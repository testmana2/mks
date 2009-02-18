#include <QApplication>
#include <QMdiArea>
#include <QDockWidget>
#include <QFileInfo>
#include <QMessageBox>

#include "maininterface/UIMonkeyDbg.h"
#include "QGdbDriver.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "Monkey Debugger" );
	
	QGdb::Driver debugger( NULL );
	
	if ( argc > 1 )
	{
		if ( QString("-v") == argv[1] )
		{
			debugger.enableDebugOutput( true );
		}
	}
	
	UIMonkeyDbg dbg( NULL, &debugger );
	dbg.showMaximized();
	
	// TODO write good command line parser
	if ( argc > 1 )
	{
		if ( QFileInfo( argv[argc - 1] ).isExecutable() )
		{
			if ( !dbg.loadTarget( argv[argc - 1] ) )
			{
				QMessageBox::critical( 0, QObject::tr( "Invalid target" ), QObject::tr( "Can't load target %1" ).arg( argv[1] ) );
			}
		}
	}
	
	// speed hack
	//dbg.loadTarget( "/home/pasnox/Development/Qt4/mks/crashapp/crashapp_debug" );
	//dbg.openFile( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp" );
	/*
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 35 );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 37 );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 39 );
	*/
	
	dbg.loadTarget( "/home/a/code/mks/debugger/libmigdb/examples/target_frames" );
	dbg.openFile( "/home/a/code/mks/debugger/libmigdb/examples/target_frames.cc" );
	
	
	return app.exec();
}
