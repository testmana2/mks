#ifndef XUPITEM_H
#define XUPITEM_H

#include <QDomNode>
#include <QList>

class XUPItem
{
public:
	XUPItem( QDomNode& node, int row, XUPItem* parent = 0 );
	~XUPItem();
	XUPItem* child( int i );
	XUPItem* parent();
	QDomNode node() const;
	int row();

private:
	QDomNode domNode;
	QList<XUPItem*> childItems;
	XUPItem* parentItem;
	int rowNumber;
};

#endif // XUPITEM_H
