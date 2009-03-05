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

#if 0	
	if ( argc > 1 )
	{
		if ( QString("-v") == argv[1] )
		{
			debugger.enableDebugOutput( true );
		}
	}
#endif
	
	UIMonkeyDbg dbg(NULL);
	dbg.showMaximized();
	
#if 0
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
#endif
	
	return app.exec();
}
