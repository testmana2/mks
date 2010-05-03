#include "AutoCompletionProperties.h"

#include <XmlObjectProcessor.h>
#include <qscintilla.h>

AutoCompletionProperties::AutoCompletionProperties()
{
	mEnabled = true;
	mCaseSensitive = true;
	mReplaceWord = true;
	mShowSingle = false;
	mThreshold = 3;
	mMode = QsciScintilla::AcsAll;
}

void AutoCompletionProperties::processData( XmlObjectProcessor* container )
{
	container->processData( "Enabled", mEnabled );
	container->processData( "CaseSensitive", mCaseSensitive );
	container->processData( "ReplaceWord", mReplaceWord );
	container->processData( "ShowSingle", mShowSingle );
	container->processData( "Threshold", mThreshold );
	container->processData( "Mode", mMode );
}
