/*****************************************************
* AUTHOR   : %AUTHOR%
* PROJECT  : %PROJECTNAME%
* FILENAME : %FILENAME%
* DATE     : %DATE%
* TIME     : %TIME%
* LICENSE  : %LICENSE%
* COMMENT  : %COMMENT%
*****************************************************/
#include <QCoreApplication>

int main( int argc, char** argv )
{
	// create application
	QCoreApplication a( argc, argv );

	// init application
	a.setApplicationName( "My Application" );
	a.setOrganizationName( "Monkey Studio" );
	a.setOrganizationDomain( "www.monkeystudio.org" );

	// start application
	return a.exec();
}
