#ifndef LINESPROPERTIES_H
#define LINESPROPERTIES_H

#include <XmlObject.h>

class LinesProperties : public XmlObject
{
public:
    LinesProperties();

protected:
    QVariant mEolMode; // int
    QVariant mAutoDetectEol; // bool
    QVariant mShowEol; // bool
    QVariant mWhitespaceMode; // int
    QVariant mWrapMode; // int
    QVariant mWrapFlagStartMode; // int
    QVariant mWrapFlagEndMode; // int
    QVariant mWrapFlagIndentWidth; // int
    
    virtual void processData( XmlObjectProcessor* container );
};

#endif // LINESPROPERTIES_H
