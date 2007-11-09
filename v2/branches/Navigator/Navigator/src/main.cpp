/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : main.cpp
** FileName  : main.cpp.cpp
** Date      : 2007-11-09T10:27:08
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QtGui>
#include "EntityContainer.h"
int main( int argc, char** argv )
{
	QApplication a( argc, argv );
	EntityContainer c;
	c.show();
	c.updateFileInfo( QDir::toNativeSeparators( "./CtagsWrapper/CtagsWrapper.h" ) );
	return a.exec();
}
