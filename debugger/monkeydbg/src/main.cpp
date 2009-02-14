#include <QApplication>
#include <QMdiArea>
#include <QDockWidget>
#include <QFileInfo>
#include <QMessageBox>

#include "maininterface/UIMonkeyDbg.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "Monkey Debugger" );
	
	UIMonkeyDbg dbg;
	dbg.showMaximized();
	
	if ( argc == 2 )
	{
		if ( QFileInfo( argv[1] ).isExecutable() )
		{
			if ( !dbg.loadTarget( argv[1] ) )
			{
				QMessageBox::critical( 0, QObject::tr( "Invalid target" ), QObject::tr( "Can't load target %1" ).arg( argv[1] ) );
			}
		}
		else 
			QMessageBox::critical( 0, QObject::tr( "Invalid parameter" ), QObject::tr( "%1 is not a executable file" ).arg( argv[1] ) );
	}
	
	// speed hack
	/*
	dbg.loadTarget( "/home/pasnox/Development/Qt4/mks/crashapp/crashapp_debug" );
	dbg.openFile( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp" );
	*/
	/*
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 35 );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 37 );
	debugger.break_setBreakpoint( "/home/pasnox/Development/Qt4/mks/crashapp/src/main.cpp", 39 );
	*/
	
	dbg.loadTarget( "/home/a/code/mks/debugger/libmigdb/examples/target_frames" );
	dbg.openFile( "/home/a/code/mks/debugger/libmigdb/examples/target_frames.cc" );
	
	return app.exec();
}
