#include <QApplication>
#include "StartForm.h"

int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	StartForm* sf = new StartForm();
	sf->show();
	return app.exec();
};
