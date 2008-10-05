#include "XUPItem.h"
#include "XUPProjectItem.h"
#include "../iconmanager/pIconManager.h"

#include <QFile>

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
	return typeId() == XUPItem::Project ? static_cast<XUPProjectItem*>( const_cast<XUPItem*>( this ) ) : mParentItem->project();
}

XUPItem* XUPItem::parent() const
{
	return mParentItem;
}

void XUPItem::setParent( XUPItem* parentItem )
{
	mParentItem = parentItem;
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

void XUPItem::setChild( int row, XUPItem* item )
{
	mChildItems[ row ] = item;
}

int XUPItem::row()
{
	return mRowNumber;
}

void XUPItem::setRow( int row )
{
	mRowNumber = row;
}

int XUPItem::count() const
{
	int count = mDomElement.childNodes().count();
	if ( !mChildItems.isEmpty() )
	{
		count = qMax( count, mChildItems.keys().last() +1 );
	}
	return count;
}

XUPItem::Type XUPItem::typeId() const
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
	return XUPItem::Unknow;
}

QString XUPItem::displayText() const
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
			return tr( QT_TR_NOOP( "%1 empty line(s)" ) ).arg( attribute( "count" ) );
			break;
		case XUPItem::Variable:
			return project()->variableDisplayText( attribute( "name" ) );
			break;
		case XUPItem::Value:
			return project()->valueDisplayText( const_cast<XUPItem*>( this ) );
			break;
		case XUPItem::Function:
			return QString( "%1(%2)" ).arg( attribute( "name" ) ).arg( attribute( "parameters" ) );
			break;
		case XUPItem::Scope:
			return attribute( "name" );
			break;
		default:
			return "#Unknow";
			break;
	}
}

QIcon XUPItem::displayIcon() const
{
	XUPProjectItem* pItem = project();
	XUPItem* item = const_cast<XUPItem*>( this );
	QString path = pItem->iconsPath();
	QString fn = pIconManager::filePath( pItem->iconFileName( item ), path );
	
	if ( !QFile::exists( fn ) )
	{
		path = pItem->projectInfos()->pixmapsPath( XUPProjectItem::XUPProject );
	}
	
	return pIconManager::icon( pItem->iconFileName( item ), path );
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
	if ( mTemporaryValues.contains( key ) )
		return mTemporaryValues[ key ];
	return defaultValue;
}

void XUPItem::setTemporaryValue( const QString& key, const QVariant& value )
{
	mTemporaryValues[ key ] = value;
}
