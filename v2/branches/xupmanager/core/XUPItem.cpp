#include "XUPItem.h"
#include "XUPProjectItem.h"
#include "../iconmanager/pIconManager.h"

XUPItem::XUPItem( const QDomElement& node, int row, XUPItem* parent )
{
	mDomElement = node;
	mRowNumber = row;
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
	return mParentItem ? mParentItem->project() : static_cast<XUPProjectItem*>( const_cast<XUPItem*>( this ) );
}

XUPItem* XUPItem::parent() const
{
	return mParentItem;
}

XUPItem* XUPItem::child( int i )
{
	if ( mChildItems.contains( i ) )
		return mChildItems[ i ];

	if ( i >= 0 && i < mDomElement.childNodes().count() )
	{
		QDomElement childElement = mDomElement.childNodes().item( i ).toElement();
		XUPItem* childItem = new XUPItem( childElement, i, this );
		mChildItems[ i ] = childItem;
		return childItem;
	}
	return 0;
}

int XUPItem::row()
{
	return mRowNumber;
}

QString XUPItem::type() const
{
	return mDomElement.nodeName();
}

XUPItem::Type XUPItem::typeId() const
{
	const QString mType = type();
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
	return XUPItem::Unknow;
}

bool XUPItem::isType( const QString& _type ) const
{
	return type() == _type;
}

bool XUPItem::isType( XUPItem::Type _type ) const
{
	return typeId() == _type;
}

QString XUPItem::text() const
{
	switch ( typeId() )
	{
		case XUPItem::Project:
			return value();
			break;
		case XUPItem::Comment:
			return value();
			break;
		case XUPItem::EmptyLine:
			return QString( QT_TR_NOOP( "%1 empty line(s)" ) ).arg( value() );
			break;
		case XUPItem::Variable:
			return value();
			break;
		case XUPItem::Value:
			return value();
			break;
		case XUPItem::Function:
			return QString( "%1(%2)" ).arg( attribute( "name" ) ).arg( value() );
			break;
		case XUPItem::Scope:
			return value();
			break;
		default:
			return QT_TR_NOOP( "#Unknow" );
			break;
	}
}

QIcon XUPItem::icon() const
{
	QString fn = QString( "%1.png" ).arg( type() );
	if ( isType( XUPItem::Value ) && project()->isFileBased( mParentItem ) )
		fn = "file.png";
	return pIconManager::icon( fn, ":/items" );
}

QString XUPItem::value( const QString& defaultValue ) const
{
	switch ( typeId() )
	{
		case XUPItem::Project:
			return attribute( "name" );
			break;
		case XUPItem::Comment:
			return attribute( "value" );
			break;
		case XUPItem::EmptyLine:
			return attribute( "count" );
			break;
		case XUPItem::Variable:
			return attribute( "name" );
			break;
		case XUPItem::Value:
			return attribute( "content" );
			break;
		case XUPItem::Function:
			return attribute( "parameters" );
			break;
		case XUPItem::Scope:
			return attribute( "name" );
			break;
		default:
			return defaultValue;
			break;
	}
}

void XUPItem::setValue( const QString& value )
{
	switch ( typeId() )
	{
		case XUPItem::Project:
			setAttribute( "name", value );
			break;
		case XUPItem::Comment:
			setAttribute( "value", value );
			break;
		case XUPItem::EmptyLine:
			setAttribute( "count", value );
			break;
		case XUPItem::Variable:
			setAttribute( "name", value );
			break;
		case XUPItem::Value:
			setAttribute( "content", value );
			break;
		case XUPItem::Function:
			setAttribute( "parameters", value );
			break;
		case XUPItem::Scope:
			setAttribute( "name", value );
			break;
		default:
			break;
	}
}

QString XUPItem::attribute( const QString& name, const QString& defaultValue ) const
{
	return mDomElement.attribute( name, defaultValue );
}

void XUPItem::setAttribute( const QString& name, const QString& value )
{
	mDomElement.setAttribute( name, value );
}
