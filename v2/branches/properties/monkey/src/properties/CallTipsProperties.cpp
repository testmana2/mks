#include "CallTipsProperties.h"

#include <XmlObjectProcessor.h>
#include <qscintilla.h>

CallTipsProperties::CallTipsProperties()
{
	mEnabled = true;
	mVisibleCount = -1;
	mContext = QsciScintilla::CallTipsContext;
	mBackground = QColor( 255, 255, 0x9b );
	mForeground = QColor( 0, 0, 0 );
	mHighlight = QColor( 255, 0, 0 );
}

void CallTipsProperties::processData( XmlObjectProcessor* container )
{
	container->processData( "Enabled", mEnabled );
	container->processData( "VisibleCount", mVisibleCount );
	container->processData( "Context", mContext );
	container->processData( "Background", mBackground );
	container->processData( "Foreground", mForeground );
	container->processData( "Highlight", mHighlight );
}
