#include <QApplication>
//
#include "UIProjectsViewer.h"
//
int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	//
	UIProjectsViewer::self()->openProject( "E:/Filipe/repository/v2/trunk/parser/example/demos.pro" );
	UIProjectsViewer::self()->show();
	//
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
