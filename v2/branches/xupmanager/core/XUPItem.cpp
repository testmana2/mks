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
	return type() == XUPItem::Project ? static_cast<XUPProjectItem*>( const_cast<XUPItem*>( this ) ) : mParentItem->project();
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
		XUPItem* childItem = new XUPItem( childElement, i, const_cast<XUPItem*>( this ) );
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
	return XUPItem::Unknow;
}

QString XUPItem::displayText()
{
	QString text;
	
	if ( temporaryValue( "hasDisplayText", false ).toBool() )
	{
		text = temporaryValue( "DisplayText" ).toString();
	}
	else
	{
		switch ( type() )
		{
			case XUPItem::Project:
				text = attribute( "name" );
				break;
			case XUPItem::Comment:
				text = attribute( "value" );
				break;
			case XUPItem::EmptyLine:
				text = tr( QT_TR_NOOP( "%1 empty line(s)" ) ).arg( attribute( "count" ) );
				break;
			case XUPItem::Variable:
				text = project()->variableDisplayText( attribute( "name" ) );
				break;
			case XUPItem::Value:
				text = project()->valueDisplayText( const_cast<XUPItem*>( this ) );
				break;
			case XUPItem::Function:
				text = QString( "%1(%2)" ).arg( attribute( "name" ) ).arg( attribute( "parameters" ) );
				break;
			case XUPItem::Scope:
				text = attribute( "name" );
				break;
			default:
				text = "#Unknow";
				break;
		}
		setTemporaryValue( "hasDisplayText", true );
		setTemporaryValue( "DisplayText", text );
	}
	
	return text;
}

QIcon XUPItem::displayIcon()
{
	QIcon icon;
	
	if ( temporaryValue( "hasDisplayIcon", false ).toBool() )
	{
		icon = temporaryValue( "DisplayIcon" ).value<QIcon>();
	}
	else
	{
		XUPProjectItem* pItem = project();
		XUPItem* item = const_cast<XUPItem*>( this );
		QString path = pItem->iconsPath();
		QString fn = pIconManager::filePath( pItem->iconFileName( item ), path );
		
		if ( !QFile::exists( fn ) )
		{
			path = pItem->projectInfos()->pixmapsPath( XUPProjectItem::XUPProject );
		}
		
		icon = pIconManager::icon( pItem->iconFileName( item ), path );
		setTemporaryValue( "hasDisplayIcon", true );
		setTemporaryValue( "DisplayIcon", icon );
	}
	
	return icon;
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

void XUPItem::clearTemporaryValue( const QString& key )
{
	mTemporaryValues.remove( key );
}
