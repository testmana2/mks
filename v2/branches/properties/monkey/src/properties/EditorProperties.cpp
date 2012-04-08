#include "EditorProperties.h"

#include <XmlObjectProcessor.h>

#include <QColor>

EditorProperties::EditorProperties()
{
    mAutomaticSyntaxCheck = false;
    mConvertTabsAtOpen = false;
    mCreateBackupFileAtOpen = false;
    mAutomaticEOLConversion = false;
    mDefaultTextCodec = "UTF-8";
    mSelectionBackground = QColor( "#BDFF9B" );
    mSelectionForeground = QColor( "#000000" );
    mUseDocumentColors = false;
    mDocumentPen = QColor( "#000000" );
    mDocumentPaper = QColor( "#FFFFFF" );
}

void EditorProperties::processData( XmlObjectProcessor* container )
{
    container->processData( "AutomaticSyntaxCheck", mAutomaticSyntaxCheck );
    container->processData( "ConvertTabsAtOpen", mConvertTabsAtOpen );
    container->processData( "CreateBackupFileAtOpen", mCreateBackupFileAtOpen );
    container->processData( "AutomaticEOLConversion", mAutomaticEOLConversion );
    container->processData( "DefaultTextCodec", mDefaultTextCodec );
    container->processData( "SelectionBackground", mSelectionBackground );
    container->processData( "SelectionForeground", mSelectionForeground );
    container->processData( "UseDocumentColors", mUseDocumentColors );
    container->processData( "DocumentPen", mDocumentPen );
    container->processData( "DocumentPaper", mDocumentPaper );
    container->processData( "AutoCompletion", mAutoCompletion );
    container->processData( "CallTips", mCallTips );
    container->processData( "Indentation", mIndentation );
    container->processData( "Braces", mBraces );
    container->processData( "Edge", mEdge );
    container->processData( "Caret", mCaret );
    container->processData( "Margins", mMargins );
    container->processData( "Lines", mLines );
}
