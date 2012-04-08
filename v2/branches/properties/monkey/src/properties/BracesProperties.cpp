#include "BracesProperties.h"

#include <XmlObjectProcessor.h>
#include <qscintilla.h>

#include <QColor>

BracesProperties::BracesProperties()
    : XmlObject()
{
    mEnabled = true;
    mMode = QsciScintilla::SloppyBraceMatch;
    mMatchedBackground = QColor( 255, 255, 0x7f );
    mMatchedForeground = QColor( 255, 0, 0 );
    mUnmatchedBackground = QColor( 255, 0, 0 );
    mUnmatchedForeground = QColor( 255, 255, 255 );
}

void BracesProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "Enabled", mEnabled );
    container->processData( "Mode", mMode );
    container->processData( "MatchedBackground", mMatchedBackground );
    container->processData( "MatchedForeground", mMatchedForeground );
    container->processData( "UnmatchedBackground", mUnmatchedBackground );
    container->processData( "UnmatchedForeground", mUnmatchedForeground );
}
