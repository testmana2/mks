#ifndef EDGEPROPERTIES_H
#define EDGEPROPERTIES_H

#include <XmlObject.h>

class EdgeProperties : public XmlObject
{
public:
	EdgeProperties();

protected:
	QVariant mEnabled; // bool
	QVariant mMode; // int
	QVariant mColumnNumber; // int
	QVariant mColor; // color
	
	virtual void processData( XmlObjectProcessor* container );
};

#endif // EDGEPROPERTIES_H
