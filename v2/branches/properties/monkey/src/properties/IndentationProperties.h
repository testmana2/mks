#ifndef INDENTATIONPROPERTIES_H
#define INDENTATIONPROPERTIES_H

#include <XmlObject.h>

class IndentationProperties : public XmlObject
{
public:
    IndentationProperties();

protected:
    QVariant mAutoIndent; // bool
    QVariant mBackspaceUnindents; // bool
    QVariant mIndentationGuides; // bool
    QVariant mIndentationUseTabs; // bool
    QVariant mTabIndents; // bool
    QVariant mAutoDetectIndentation; // bool
    QVariant mTabWidth; // int
    QVariant mIndentationWidth; // int
    QVariant mGuideBackground; // color
    QVariant mGuideForeground; // color
    
    virtual void processData( XmlObjectProcessor* container );
};

#endif // INDENTATIONPROPERTIES_H
