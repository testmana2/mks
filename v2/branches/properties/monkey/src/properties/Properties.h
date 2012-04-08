#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <XmlObject.h>

#include "GeneralProperties.h"
#include "PathsProperties.h"
#include "EditorProperties.h"

class Properties : public XmlObject
{
public:
    Properties();
    virtual ~Properties();

protected:
    QVariant mFirstTimeRunning; // bool
    GeneralProperties mGeneral;
    PathsProperties mPaths;
    EditorProperties mEditor;
    
    virtual void processData( XmlObjectProcessor* container );
};

#endif // PROPERTIES_H
