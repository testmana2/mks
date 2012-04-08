#ifndef CARETPROPERTIES_H
#define CARETPROPERTIES_H

#include <XmlObject.h>

class CaretProperties : public XmlObject
{
public:
    CaretProperties();

protected:
    QVariant mForeground; // color
    QVariant mWidth; // int
    QVariant mLineVisible; // bool
    QVariant mLineBackground; // color
    
    virtual void processData( XmlObjectProcessor* container );
};

#endif // CARETPROPERTIES_H
