#include "XUPItem.h"
#include "XUPProjectItem.h"

XUPItem::XUPItem( const QDomElement& node, XUPItem* parent )
{
	mDomElement = node;
	mParentItem = parent;
}

XUPItem::~XUPItem()
{
	qDeleteAll( mChildItems );
	mChildItems.clear();
}

QDomElement XUPItem::node() const
{
	return mDomElement;
}

XUPProjectItem* XUPItem::project() const
{
	if ( type() == XUPItem::Project )
		return static_cast<XUPProjectItem*>( const_cast<XUPItem*>( this ) );
	else
		return mParentItem->project();
}

XUPItem* XUPItem::parent() const
{
	return mParentItem;
}

void XUPItem::setParent( XUPItem* parentItem )
{
	mParentItem = parentItem;
}

XUPItem* XUPItem::child( int i ) const
{
	if ( mChildItems.contains( i ) )
		return mChildItems[ i ];

	if ( i >= 0 && i < mDomElement.childNodes().count() )
	{
		QDomElement childElement = mDomElement.childNodes().item( i ).toElement();
		XUPItem* childItem = new XUPItem( childElement, const_cast<XUPItem*>( this ) );
		mChildItems[ i ] = childItem;
		return childItem;
	}
	return 0;
}

int XUPItem::childIndex( XUPItem* child )
{
	return mChildItems.key( child, -1 );
}

void XUPItem::addChild( XUPItem* item )
{
	mChildItems[ childCount() ] = item;
	item->setParent (this);
}

int XUPItem::row()
{
	if ( mParentItem )
		return mParentItem->childIndex( this );
	else
		return 0;
}

int XUPItem::childCount() const
{
	int count = mDomElement.childNodes().count();
	if ( !mChildItems.isEmpty() )
	{
		count = qMax( count, mChildItems.keys().last() +1 );
	}
	return count;
}

XUPItem::Type XUPItem::type() const
{
	const QString mType = mDomElement.nodeName();
	if ( mType == "project" )
		return XUPItem::Project;
	else if ( mType == "comment" )
		return XUPItem::Comment;
	else if ( mType == "emptyline" )
		return XUPItem::EmptyLine;
	else if ( mType == "variable" )
		return XUPItem::Variable;
	else if ( mType == "value" )
		return XUPItem::Value;
	else if ( mType == "function" )
		return XUPItem::Function;
	else if ( mType == "scope" )
		return XUPItem::Scope;
	else if ( mType == "dynamicfolder" )
		return XUPItem::DynamicFolder;
	else if ( mType == "folder" )
		return XUPItem::Folder;
	else if ( mType == "file" )
		return XUPItem::File;
	else if ( mType == "path" )
		return XUPItem::Path;
	return XUPItem::Unknow;
}

QString XUPItem::displayText() const
{
	return project()->itemDisplayText( const_cast<XUPItem*>( this ) );
}

QIcon XUPItem::displayIcon() const
{
	return project()->itemDisplayIcon( const_cast<XUPItem*>( this ) );
}

QString XUPItem::attribute( const QString& name, const QString& defaultValue ) const
{
	return mDomElement.attribute( name, defaultValue );
}

void XUPItem::setAttribute( const QString& name, const QString& value )
{
	mDomElement.setAttribute( name, value );
}

QVariant XUPItem::temporaryValue( const QString& key, const QVariant& defaultValue ) const
{
	return mTemporaryValues.value( key, defaultValue );
}

void XUPItem::setTemporaryValue( const QString& key, const QVariant& value )
{
	mTemporaryValues[ key ] = value;
}

void XUPItem::clearTemporaryValue( const QString& key )
{
	mTemporaryValues.remove( key );
}
