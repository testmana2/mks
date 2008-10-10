#ifndef XUPITEM_H
#define XUPITEM_H

#include <QDomElement>
#include <QMap>
#include <QIcon>
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
	// index of a child
	int childIndex( XUPItem* child );
	// set a child item for row i
	void addChild( XUPItem* item );
	// return the parent item
	XUPItem* parent() const;
	// return the item row. If item hasn't parent - -1 will be return
	int row();
	// return child count
	int childCount() const;
	
	// the type enum of this item
	XUPItem::Type type() const;

	// return the content of attribute name or defaultValue if null/invalid
	QString attribute( const QString& name, const QString& defaultValue = QString::null ) const;
	// set the attribute value for name
	void setAttribute( const QString& name, const QString& value );
	
	// return the stored temporary value for key or defaultValue
	QVariant temporaryValue( const QString& key, const QVariant& defaultValue = QVariant() ) const;
	// set the temporary value for key
	void setTemporaryValue( const QString& key, const QVariant& value );
	// clear temporary data represented by key
	void clearTemporaryValue( const QString& key );
	
	// view text, the text to shown in the item view
	QString displayText() const;
	// view icon, the icon to shown in the item view
	QIcon displayIcon() const;

protected:
	QDomElement mDomElement;
	mutable QMap<int, XUPItem*> mChildItems;
	XUPItem* mParentItem;
	QMap<QString, QVariant> mTemporaryValues;
	
	// developer must not be able to create/instanciate items itself, it must be done by the model
	XUPItem( const QDomElement& node, XUPItem* parent = 0 );
	// set the parent item. Call automaticaly from parent's addChild
	void setParent( XUPItem* parentItem );

	// return the node element associate with this item
	QDomElement node() const;
};

#endif // XUPITEM_H
