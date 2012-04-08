#ifndef BRACESPROPERTIES_H
#define BRACESPROPERTIES_H

#include <XmlObject.h>

class BracesProperties : public XmlObject
{
public:
    BracesProperties();

protected:
    QVariant mEnabled; // bool
    QVariant mMode; // int
    QVariant mMatchedBackground; // color
    QVariant mMatchedForeground; // color
    QVariant mUnmatchedBackground; // color
    QVariant mUnmatchedForeground; // color
    
    virtual void processData( XmlObjectProcessor* container );
};

#endif // BRACESPROPERTIES_H
