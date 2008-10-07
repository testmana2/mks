#ifndef XUPITEM_H
#define XUPITEM_H

#include <QDomElement>
#include <QMap>
#include <QIcon>
#include <QApplication>
#include <QVariant>

class XUPProjectItem;

class XUPItem
{
	friend class XUPProjectModel;
	friend class XUPProjectItem;
	
public:
	// possible types for a node
	enum Type {
		Unknow = -1,
		Project,
		Comment,
		EmptyLine,
		Variable,
		Value,
		Function,
		Scope
	};
	
	// dtor
	virtual ~XUPItem();
	
	// project item
	XUPProjectItem* project() const;
	// return the i child item
	XUPItem* child( int i ) const;
	// set a child item for row i
	void setChild( int row, XUPItem* item );
	// return the parent item
	XUPItem* parent() const;
	// set the parent item
	void setParent( XUPItem* parentItem );
	// return the item row
	int row();
	// set the item row
	void setRow( int row );
	// return child count
	int count() const;
	
	// the enum version of type()
	XUPItem::Type type() const;

	// return the content of attribute name or defaultValue if null/invalid
	QString attribute( const QString& name, const QString& defaultValue = null() ) const;
	// set the attribute value
	void setAttribute( const QString& name, const QString& value );
	
	// return the custom value associate with the item type, or defaultValue if null/invalid
	QVariant temporaryValue( const QString& key, const QVariant& defaultValue = QVariant() ) const;
	// set the custom value associate with item type
	void setTemporaryValue( const QString& key, const QVariant& value );
	// clear temporary data represented by key
	void clearTemporaryValue( const QString& key );
	
	// a string to show when requesting null/invalid content
	inline static QString null() { return "#Null"; }
	// static tr member
	inline QString tr( const char* text ) const { return qApp->translate( "XUPItem", text ); }
	
	// view text, the text to shown in the item view
	QString displayText();
	// view icon, the icon to shown in the item view
	QIcon displayIcon();

protected:
	QDomElement mDomElement;
	mutable QMap<int, XUPItem*> mChildItems;
	XUPItem* mParentItem;
	int mRowNumber;
	QMap<QString, QVariant> mTemporaryValues;
	
	// developer must not be able to create/instanciate items itself, it must be done by the model
	XUPItem( const QDomElement& node, int row, XUPItem* parent = 0 );
	// return the node element associate with this item
	QDomElement node() const;
};

#endif // XUPITEM_H
