#include <QApplication>

#include "asdf.h"

int main( int argc, char** argv )
{
	// create application
	QApplication a( argc, argv );

	// init application
	a.setApplicationName( "My Application" );
	a.setOrganizationName( "Monkey Studio" );
	a.setOrganizationDomain( "www.monkeystudio.org" );

    (new pStringListEditor ())->show();
	// connection
	QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	// start application
	return a.exec();
}