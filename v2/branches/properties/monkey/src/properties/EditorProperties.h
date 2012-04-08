#ifndef EDITORPROPERTIES_H
#define EDITORPROPERTIES_H

#include <XmlObject.h>

#include "AutoCompletionProperties.h"
#include "CallTipsProperties.h"
#include "IndentationProperties.h"
#include "BracesProperties.h"
#include "EdgeProperties.h"
#include "CaretProperties.h"
#include "MarginsProperties.h"
#include "LinesProperties.h"

class EditorProperties : public XmlObject
{
public:
    EditorProperties();

protected:
    QVariant mAutomaticSyntaxCheck; // bool
    QVariant mConvertTabsAtOpen; // bool
    QVariant mCreateBackupFileAtOpen; // bool
    QVariant mAutomaticEOLConversion; // bool
    QVariant mDefaultTextCodec; // string
    QVariant mSelectionBackground; // color
    QVariant mSelectionForeground; // color
    QVariant mUseDocumentColors; // bool
    QVariant mDocumentPen; // color
    QVariant mDocumentPaper; // color
    AutoCompletionProperties mAutoCompletion;
    CallTipsProperties mCallTips;
    IndentationProperties mIndentation;
    BracesProperties mBraces;
    EdgeProperties mEdge;
    CaretProperties mCaret;
    MarginsProperties mMargins;
    LinesProperties mLines;

    virtual void processData( XmlObjectProcessor* container );
};

#endif // EDITORPROPERTIES_H
