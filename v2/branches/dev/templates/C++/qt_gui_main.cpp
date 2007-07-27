/*****************************************************
* AUTHOR   : %AUTHOR%
* PROJECT  : %PROJECTNAME%
* FILENAME : %FILENAME%
* DATE     : %DATE%
* TIME     : %TIME%
* LICENSE  : %LICENSE%
* COMMENT  : %COMMENT%
*****************************************************/
#include <QApplication>

int main( int argc, char** argv )
{
	// create application
	QApplication a( argc, argv );

	// init application
	a.setApplicationName( "My Application" );
	a.setOrganizationName( "Monkey Studio" );
	a.setOrganizationDomain( "www.monkeystudio.org" );

	// connection
	QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	// start application
	return a.exec();
}
