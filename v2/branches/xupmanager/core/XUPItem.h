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
	XUPItem* child( int i );
	// return the parent item
	XUPItem* parent() const;
	// return the item row
	int row();
	// return child count
	int count() const;
	
	// a string to show when requesting null/invalid content
	inline static QString null() { return "#Null"; }
	// static tr member
	inline QString tr( const char* text ) const { return qApp->translate( "XUPProjectItem", text ); }
	
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
	
	// return the value associate with the item type, or defaultValue if null/invalid
	QString value( const QString& defaultValue = null() ) const;
	// set the value associate with item type
	void setValue( const QString& value );

	// return the content of attribute name or defaultValue if null/invalid
	QString attribute( const QString& name, const QString& defaultValue = null() ) const;
	// set the attribute value
	void setAttribute( const QString& name, const QString& value );
	
	// return the custom value associate with the item type, or defaultValue if null/invalid
	QVariant customValue( const QString& key, const QVariant& defaultValue = QVariant() ) const;
	// set the custom value associate with item type
	void setCustomValue( const QString& key, const QVariant& value );

protected:
	QDomElement mDomElement;
	QMap<int, XUPItem*> mChildItems;
	XUPItem* mParentItem;
	int mRowNumber;
	QMap<QString, QVariant> mCustomValues;
	
	// developer must not be able to create/instanciate items itself, it must be done by the model
	XUPItem( const QDomElement& node, int row, XUPItem* parent = 0 );
	// return the node element associate with this item
	QDomElement node() const;
};

#endif // XUPITEM_H
