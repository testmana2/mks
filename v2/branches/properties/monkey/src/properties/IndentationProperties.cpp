#include "IndentationProperties.h"

#include <XmlObjectProcessor.h>

#include <QColor>

IndentationProperties::IndentationProperties()
	: XmlObject()
{
	mAutoIndent = true;
	mBackspaceUnindents = true;
	mIndentationGuides = true;
	mIndentationUseTabs = false;
	mTabIndents = true;
	mAutoDetectIndentation = true;
	mTabWidth = 4;
	mIndentationWidth = 4;
	mGuideBackground = QColor( 0, 0, 255 );
	mGuideForeground = QColor( 0, 0, 255 );
}

void IndentationProperties::processData( XmlObjectProcessor* container )
{
	container->processData( "AutoIndent", mAutoIndent );
	container->processData( "BackspaceUnindents", mBackspaceUnindents );
	container->processData( "IndentationGuides", mIndentationGuides );
	container->processData( "IndentationUseTabs", mIndentationUseTabs );
	container->processData( "TabIndents", mTabIndents );
	container->processData( "AutoDetectIndentation", mAutoDetectIndentation );
	container->processData( "TabWidth", mTabWidth );
	container->processData( "IndentationWidth", mIndentationWidth );
	container->processData( "GuideBackground", mGuideBackground );
	container->processData( "GuideForeground", mGuideForeground );
}
