#ifndef CALLTIPSPROPERTIES_H
#define CALLTIPSPROPERTIES_H

#include "XmlObject.h"

class CallTipsProperties : public XmlObject
{
public:
	CallTipsProperties();

protected:
	QVariant mEnabled; // bool
	QVariant mVisibleCount; // int
	QVariant mContext; // int
	QVariant mBackground; // QColor
	QVariant mForeground; // QColor
	QVariant mHighlight; // QColor

	virtual void processData( XmlObjectProcessor* container );
};

#endif // CALLTIPSPROPERTIES_H
