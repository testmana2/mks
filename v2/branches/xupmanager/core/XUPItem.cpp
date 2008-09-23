#include "XUPItem.h"

XUPItem::XUPItem( QDomNode& node, int row, XUPItem* parent )
{
	domNode = node;
	rowNumber = row;
	parentItem = parent;
}

XUPItem::~XUPItem()
{
	qDeleteAll( childItems );
	childItems.clear();
}

QDomNode XUPItem::node() const
{
	return domNode;
}

QString XUPItem::name() const
{
	return domNode.nodeName();
}

QString XUPItem::attributeValue( const QString& attribute ) const
{
	return domNode.toElement().attribute( attribute );
}

XUPItem* XUPItem::parent()
{
	return parentItem;
}

XUPItem* XUPItem::child( int i )
{
	if ( i < childItems.count() )
		return childItems.at( i );

	if ( i >= 0 && i < domNode.childNodes().count() )
	{
		QDomNode childNode = domNode.childNodes().item( i );
		XUPItem* childItem = new XUPItem( childNode, i, this );
		childItems.insert( i, childItem );
		return childItem;
	}
	return 0;
}

int XUPItem::row()
{
	return rowNumber;
}

