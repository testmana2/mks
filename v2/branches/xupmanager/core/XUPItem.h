#ifndef XUPITEM_H
#define XUPITEM_H

#include <QDomElement>
#include <QHash>
#include <QIcon>

class XUPItem
{
	friend class XUPProjectModel;
	
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
	~XUPItem();
	
	// return the i child item
	XUPItem* child( int i );
	// return the parent item
	XUPItem* parent();
	// return the item row
	int row();
	
	// a string to show when requesting null/invalid content
	inline static QString null() { return QT_TR_NOOP( "#Null" ); }
	
	// view text
	QString text() const;
	// view icon
	QIcon icon() const;
	
	// the item type ( ie: node name )
	QString type() const;
	// the enum version of type()
	XUPItem::Type typeId() const;
	
	// return true if item is type
	bool isType( const QString& type ) const;
	bool isType( XUPItem::Type type ) const;
	
	// return true if item is a variable wich values are files
	bool isFileBased() const;
	// return true if item is a variable wich values are paths
	bool isPathBased() const;
	
	// return the value associate with the item type, or defaultValue if null/invalid
	QString value( const QString& defaultValue = null() ) const;
	// set the value associate with item type
	void setValue( const QString& value );

	// return the content of attribute name or defaultValue if null/invalid
	QString attribute( const QString& name, const QString& defaultValue = null() ) const;
	// set the attribute value
	void setAttribute( const QString& name, const QString& value );

protected:
	QDomElement mDomElement;
	QHash<int, XUPItem*> mChildItems;
	XUPItem* mParentItem;
	int mRowNumber;
	
	// developer must not be able to create/instanciate items itself, it must be done by the model
	XUPItem( const QDomElement& node, int row, XUPItem* parent = 0 );
	// return the node element associate with this item
	QDomElement node() const;
};

#endif // XUPITEM_H

/*
// Project. Never NULL
XUPProjectItem project();

// Parent. Are NULL only for top level project 
XUPItem parent ();
void setParent (XUPItem*);

// Childreen 
int childCount();
XUPItem child (int index);
QList<XUPItem*> childreen();

addChild (XUPItem* child);
XUPItem* takeChild (XUPItem* child)
*/

