#include "EdgeProperties.h"

#include <XmlObjectProcessor.h>
#include <qscintilla.h>

EdgeProperties::EdgeProperties()
	: XmlObject()
{
	mEnabled = false;
	mMode = QsciScintilla::EdgeLine;
	mColumnNumber = 80;
	mColor = QColor( 255, 0, 0 );
}

void EdgeProperties::processData( XmlObjectProcessor* container )
{
	container->processData( "Enabled", mEnabled );
	container->processData( "Mode", mMode );
	container->processData( "ColumnNumber", mColumnNumber );
	container->processData( "Color", mColor );
}
