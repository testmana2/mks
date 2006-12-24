#include <QApplication>
#include <QErrorMessage>
//
#include "UIProjectsManager.h"
//
int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	//
	QErrorMessage::qtHandler();
	UIProjectsManager::self()->show();
	//
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
