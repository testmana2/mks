#include <QtGui>

#include "gui/XUPProjectManager.h"

#include "core/XUPProjectItem.h"
#include "addons/qmake/QMakeProjectItem.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
	XUPProjectManager pm;
	pm.registerProjectItem( new XUPProjectItem() );
	pm.registerProjectItem( new QMakeProjectItem() );
	pm.show();
	
	return app.exec();
}
