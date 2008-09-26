#include <QtGui>

#include "gui/XUPProjectManager.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
	XUPProjectManager pm;
	pm.show();
	
	return app.exec();
}
