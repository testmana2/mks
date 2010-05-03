#ifndef MARGINSPROPERTIES_H
#define MARGINSPROPERTIES_H

#include <XmlObject.h>

class LineNumbersProperties : public XmlObject
{
public:
	LineNumbersProperties();

protected:
	QVariant mEnabled; // bool
	QVariant mWidth; // int
	QVariant mAutoWidth; // bool
	
	virtual void processData( XmlObjectProcessor* container );
};

class FoldProperties : public XmlObject
{
public:
	FoldProperties();

protected:
	QVariant mEnabled; // bool
	QVariant mMode; // int
	QVariant mForeground; // color
	QVariant mBackground; // color
	
	virtual void processData( XmlObjectProcessor* container );
};

class MarginsProperties : public XmlObject
{
public:
	MarginsProperties();

protected:
	LineNumbersProperties mLineNumbers;
	FoldProperties mFold;
	QVariant mGlobalEnabled; // color
	QVariant mForeground; // color
	QVariant mBackground; // color
	QVariant mFont; // font
	
	virtual void processData( XmlObjectProcessor* container );
};

#endif // MARGINSPROPERTIES_H
