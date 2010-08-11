#ifndef XUPITEM_H
#define XUPITEM_H

#include "MonkeyExport.h"

#include <QDomElement>
#include <QMap>
#include <QIcon>
#include <QVariant>
#include <QModelIndex>

class XUPProjectItem;
class XUPProjectModel;

typedef QList<class XUPItem*> XUPItemList;

class Q_MONKEY_EXPORT XUPItem
{
	friend class XUPProjectModel;
	friend class XUPProjectItem;
	
public:
	// possible types for a node
	enum Type {
		Unknow = -1,
		Project, // a project node
		Comment, // a comment node
		EmptyLine, // a empty line node
		Variable, // a variabel node
		Value, // a value node
		Function, // a function node
		Scope, // a scope node
		//
		Folder, // a folder node
		File, // a value that is a file node
		Path // a value that is a path node
	};
	
	// dtor
	virtual ~XUPItem();
	
	// the < operator for sorting items in tree
	virtual bool sameTypeLess( const XUPItem& other ) const;
	virtual bool operator<( const XUPItem& other ) const;
	
	QDomElement toXML() const;
	
	// project item
	XUPProjectItem* project() const;
	// return the i child item
	XUPItem* child( int i ) const;
	// return children list
	XUPItemList childrenList() const;
	// index of a child
	int childIndex( XUPItem* child ) const;
	// set a child item for row i
	void addChild( XUPItem* item );
	// return the parent item
	XUPItem* parent() const;
	// return the item row. If item hasn't parent -1 will be return
	int row() const;
	// return child count
	int childCount() const;
	// remove a child and inform the model if possible
	void removeChild( XUPItem* item );
	// create a new child of type at given row, if row is -1 the item is append to the end
	XUPItem* addChild( XUPItem::Type type, int row = -1 );
	// return the model associated with the item or null if item is not yet in a model
	XUPProjectModel* model() const;
	// return the QModelIndex of the item if it's in a model, else an invalid qmodelindex
	QModelIndex index() const;
	
	// the type enum of this item
	XUPItem::Type type() const;

	// return the content of attribute name or defaultValue if null/invalid
	QString attribute( const QString& name, const QString& defaultValue = QString::null ) const;
	// set the attribute value for name
	void setAttribute( const QString& name, const QString& value );
	
	// view text, the text to shown in the item view
	QString displayText() const;
	// view icon, the icon to shown in the item view
	QIcon displayIcon() const;
	
	QString content() const;
	void setContent(const QString&);

protected:
	XUPProjectModel* mModel;
	QDomElement mDomElement;
	mutable QMap<int, XUPItem*> mChildItems;
	XUPItem* mParentItem;
	
	// developer must not be able to create/instanciate items itself, it must be done by the model
	XUPItem( const QDomElement& node, XUPItem* parent = 0 );
	// set the parent item. Call automaticaly from parent's addChild
	void setParent( XUPItem* parentItem );

	// return the node element associate with this item
	QDomElement node() const;
};

Q_DECLARE_METATYPE( XUPItem* );

#endif // XUPITEM_H
