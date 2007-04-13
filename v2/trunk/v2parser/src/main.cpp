#include <QApplication>
//
#include "UIProjectsViewer.h"
//
int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	//
#ifndef Q_WS_WIN
	UIProjectsViewer::self()->openProject( "/home/pasnox/Documents/Development/Qt4/monkeyrepos/v2/trunk/parser/example/demos.pro" );
#else
	UIProjectsViewer::self()->openProject( "E:/Filipe/repository/v2/trunk/parser/example/demos.pro" );
#endif
	UIProjectsViewer::self()->show();
	//
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
