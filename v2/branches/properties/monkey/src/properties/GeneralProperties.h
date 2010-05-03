#ifndef GENERALPROPERTIES_H
#define GENERALPROPERTIES_H

#include <XmlObject.h>

class GeneralProperties : public XmlObject
{
public:
	GeneralProperties();

protected:
	QVariant mSaveFilesOnCustomAction; // bool
	QVariant mDefaultProjectDirectory; // string
	QVariant mShowQuickFileAccess; // bool
	QVariant mSortingMode; // int
	QVariant mTabMode; // int
	QVariant mSaveSessionOnClose; // bool
	QVariant mRestoreSessionOnOpen; // bool

	virtual void processData( XmlObjectProcessor* container );
};

#endif // GENERALPROPERTIES_H
