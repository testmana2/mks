#include "Properties.h"

#include <XmlObjectProcessor.h>

Properties::Properties()
	: XmlObject()
{
	mFirstTimeRunning = true;
}

Properties::~Properties()
{
}

void Properties::processData( XmlObjectProcessor* container )
{
	container->processData( "FirstTimeRunning", mFirstTimeRunning );
	container->processData( "General", mGeneral );
	container->processData( "Paths", mPaths );
	container->processData( "Editor", mEditor );
}
