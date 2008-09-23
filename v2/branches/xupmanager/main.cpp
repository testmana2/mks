#include <QtGui>
#include <QDebug>

#include "UIMain.h"
#include "core/XUPProjectModel.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
	const QString fn = app.applicationDirPath().append( "/monkey.xup" );
	
	UIMain* w = new UIMain();
	XUPProjectModel* model = new XUPProjectModel( w );
	w->tvNative->setModel( model );
	
	if ( !model->open( fn ) )
		qWarning() << model->lastError();
	
	w->show();
	
	return app.exec();
}
