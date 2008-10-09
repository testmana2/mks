#include <QtGui>

#include "gui/XUPProjectManager.h"

#include "core/XUPProjectItem.h"
#include "addons/qmake/QMakeProjectItem.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
	// register project type
	( new XUPProjectItem() )->registerProjectType();
	( new QMakeProjectItem() )->registerProjectType();
	
	// create project manager
	XUPProjectManager pm;
	pm.show();
	pm.move( 0, 0 );
	pm.resize( 400, app.desktop()->availableGeometry().height() );
	
	return app.exec();
}
