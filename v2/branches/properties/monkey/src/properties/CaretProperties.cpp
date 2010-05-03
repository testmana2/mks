#include "CaretProperties.h"

#include <XmlObjectProcessor.h>

#include <QColor>

CaretProperties::CaretProperties()
	: XmlObject()
{
	mForeground = QColor( 0, 0, 0 );
	mWidth = 1;
	mLineVisible = true;
	mLineBackground = QColor( 0xaa, 0xaa, 255 );
}

void CaretProperties::processData( XmlObjectProcessor* container )
{
	container->processData( "Foreground", mForeground );
	container->processData( "Width", mWidth );
	container->processData( "LineVisible", mLineVisible );
	container->processData( "LineBackground", mLineBackground );
}
