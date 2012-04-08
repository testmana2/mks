#ifndef AUTOCOMPLETIONPROPERTIES_H
#define AUTOCOMPLETIONPROPERTIES_H

#include "XmlObject.h"

class AutoCompletionProperties : public XmlObject
{
public:
    AutoCompletionProperties();

protected:
    QVariant mEnabled; // bool
    QVariant mCaseSensitive; // bool
    QVariant mReplaceWord; // bool
    QVariant mShowSingle; // bool
    QVariant mThreshold; // int
    QVariant mMode; // int

    virtual void processData( XmlObjectProcessor* container );
};

#endif // AUTOCOMPLETIONPROPERTIES_H
