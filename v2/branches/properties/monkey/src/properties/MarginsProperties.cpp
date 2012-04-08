#include "MarginsProperties.h"

#include <XmlObjectProcessor.h>
#include <qscintilla.h>

#include <QColor>

LineNumbersProperties::LineNumbersProperties()
    : XmlObject()
{
    mEnabled = true;
    mWidth = 4;
    mAutoWidth = true;
}

void LineNumbersProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "Enabled", mEnabled );
    container->processData( "Width", mWidth );
    container->processData( "AutoWidth", mAutoWidth );
}

FoldProperties::FoldProperties()
    : XmlObject()
{
    mEnabled = true;
    mMode = QsciScintilla::BoxedTreeFoldStyle;
    mForeground = QColor( 255, 255, 255 );
    mBackground = QColor( 0xc0, 0xc0, 0xc0 );
}

void FoldProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "Enabled", mEnabled );
    container->processData( "Mode", mMode );
    container->processData( "Foreground", mForeground );
    container->processData( "Background", mBackground );
}

MarginsProperties::MarginsProperties()
    : XmlObject()
{
    mGlobalEnabled = false;
    mForeground = QColor( 255, 255, 255 );
    mBackground = QColor( 0xc0, 0xc0, 0xc0 );
    mFont = QFont();
}

void MarginsProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "LineNumbers", mLineNumbers );
    container->processData( "Fold", mFold );
    container->processData( "GlobalEnabled", mGlobalEnabled );
    container->processData( "Foreground", mForeground );
    container->processData( "Background", mBackground );
    container->processData( "Font", mFont );
}
