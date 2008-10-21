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

bool XUPItem::sameTypeLess( const XUPItem& other ) const
{	
	switch ( other.type() )
	{
		case XUPItem::Variable:
		{
			XUPProjectItem* pItem = project();
			QStringList filteredVariables = pItem->projectInfos()->filteredVariables( pItem->projectType() );
			return filteredVariables.indexOf( attribute( "name" ) ) < filteredVariables.indexOf( other.attribute( "name" ) );
			break;
		}
		case XUPItem::Comment:
			return row() < other.row();
			break;
		case XUPItem::EmptyLine:
			return attribute( "count" ).toInt() < other.attribute( "count" ).toInt();
			break;
		case XUPItem::Project:
		case XUPItem::Value:
		case XUPItem::Function:
		case XUPItem::Scope:
		case XUPItem::DynamicFolder:
		case XUPItem::Folder:
		case XUPItem::File:
		case XUPItem::Path:
		default:
			break;
	}
	return displayText().toLower() < other.displayText().toLower();
}

bool XUPItem::operator<( const XUPItem& other ) const
{
	if ( type() == other.type() )
		return sameTypeLess( other );
	else
	{
		switch ( type() )
		{
			case XUPItem::Project:
				return false;
				break;
			default:
				return true;
				break;
		}
	}
#warning XUPItem::operator< it may need a special test to do when not same type
	return displayText().toLower() < other.displayText().toLower();
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

int XUPItem::childIndex( XUPItem* child ) const
{
	return mChildItems.key( child, -1 );
}

void XUPItem::addChild( XUPItem* item )
{
	mChildItems[ childCount() ] = item;
	item->setParent (this);
}

int XUPItem::row() const
{
	if ( mParentItem )
		return mParentItem->childIndex( const_cast<XUPItem*>( this ) );
	else
		return 0;
}

int XUPItem::childCount() const
{
	// handle include items and custom row count
	XUPItem* item = const_cast<XUPItem*>( this );
	XUPProjectItem* pItem = project();
	pItem->handleIncludeItem( item );
	pItem->customRowCount( item );
	
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
