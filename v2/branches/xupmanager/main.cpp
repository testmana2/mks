#include <QtGui>
#include <QDebug>

#include "core/XUPProjectModel.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
	const QString fn = app.applicationDirPath().append( "/monkey.xup" );
	
	QTreeView tv;
	XUPProjectModel* model = new XUPProjectModel( &tv );
	if ( !model->open( fn ) )
		qWarning() << model->lastError();
	tv.setModel( model );
	tv.show();
	
	return app.exec();
}
