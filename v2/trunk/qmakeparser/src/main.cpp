#include <QApplication>
#include <QErrorMessage>
//
#include "UIMain.h"
//
int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	//
	QErrorMessage::qtHandler();
	UIMain::self()->show();
	//
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
