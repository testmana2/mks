#include <QApplication>
//
#include "UIProjectsViewer.h"
//
int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	// main widget
	UIProjectsViewer::self()->show();
	//
	return app.exec();
};
