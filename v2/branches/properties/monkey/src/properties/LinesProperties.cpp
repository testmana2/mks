#include "LinesProperties.h"
#include "pMonkeyStudio.h"

#include <XmlObjectProcessor.h>
#include <qscintilla.h>

LinesProperties::LinesProperties()
    : XmlObject()
{
    mEolMode = pMonkeyStudio::eolMode(); // int
    mAutoDetectEol = true;
    mShowEol = false;
    mWhitespaceMode = QsciScintilla::WsInvisible;
    mWrapMode = QsciScintilla::WrapNone;
    mWrapFlagStartMode = QsciScintilla::WrapFlagNone;
    mWrapFlagEndMode = QsciScintilla::WrapFlagNone;
    mWrapFlagIndentWidth = 4;
}

void LinesProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "EolMode", mEolMode );
    container->processData( "AutoDetectEol", mAutoDetectEol );
    container->processData( "ShowEol", mShowEol );
    container->processData( "WhitespaceMode", mWhitespaceMode );
    container->processData( "WrapMode", mWrapMode );
    container->processData( "WrapFlagStartMode", mWrapFlagStartMode );
    container->processData( "WrapFlagEndMode", mWrapFlagEndMode );
    container->processData( "WrapFlagIndentWidth", mWrapFlagIndentWidth );
}
