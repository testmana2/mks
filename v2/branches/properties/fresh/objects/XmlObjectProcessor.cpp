/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "XmlObjectProcessor.h"
#include "QVariantHelper.h"

#include <QDebug>

QHash<XmlObjectProcessor::Type, QString> XmlObjectProcessor::mTypes;

XmlObjectProcessor::XmlObjectProcessor( const QDomElement& element, XmlObjectProcessor::Mode mode )
{
	mCurrentElement = element;
	mMode = mode;
	
	if ( mTypes.isEmpty() ) {
		mTypes[ XmlObjectProcessor::Invalid ] = QLatin1String( "invalid" );
		mTypes[ XmlObjectProcessor::VariantList ] = QLatin1String( "variantlist" );
		mTypes[ XmlObjectProcessor::StringList ] = QLatin1String( "stringlist" );
		mTypes[ XmlObjectProcessor::VariantMap ] = QLatin1String( "variantmap" );
		mTypes[ XmlObjectProcessor::VariantHash ] = QLatin1String( "varianthash" );
	}
}

XmlObjectProcessor::XmlObjectProcessor( const XmlObjectProcessor& other )
{
	operator=( other );
}

XmlObjectProcessor& XmlObjectProcessor::operator=( const XmlObjectProcessor& other )
{
	if ( *this != other )
	{
		mCurrentElement = other.mCurrentElement;
		mMode = other.mMode;
	}

	return *this;
}

bool XmlObjectProcessor::operator==( const XmlObjectProcessor& other ) const
{
	return mCurrentElement == other.mCurrentElement
		&& mMode == other.mMode;
}

bool XmlObjectProcessor::operator!=( const XmlObjectProcessor& other ) const
{
	return !operator==( other );
}

QString XmlObjectProcessor::xmlHeader()
{
	return "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
}

XmlObjectProcessor::Mode XmlObjectProcessor::mode() const
{
	return mMode;
}

XmlObjectProcessor::Type XmlObjectProcessor::type( const QString& string ) const
{
	return mTypes.key( string.trimmed().toLower(), XmlObjectProcessor::Invalid );
}

QString XmlObjectProcessor::typeToString( XmlObjectProcessor::Type type ) const
{
	return mTypes.value( type, "invalid" );
}

bool XmlObjectProcessor::isReadMode() const
{
	return mMode == XmlObjectProcessor::Read;
}

bool XmlObjectProcessor::isWriteMode() const
{
	return mMode == XmlObjectProcessor::Write;
}

QString XmlObjectProcessor::toXml() const
{
	return mCurrentElement.ownerDocument().toString( 4 );
}

QDomElement XmlObjectProcessor::getSerializeNode( const QString& name, const QDomElement& forceNode )
{
	if ( !forceNode.isNull() ) {
		return forceNode;
	}
	else if ( mCurrentElement.isNull() || name.isEmpty() ) {
		return QDomElement();
	}
	
	const QDomElement element = mCurrentElement.ownerDocument().createElement( name );
	mCurrentElement.appendChild( element );
	
	return element;
}

QDomElement XmlObjectProcessor::getUnSerializeNode( const QString& name, const QDomElement& forceNode ) const
{
	if ( !forceNode.isNull() ) {
		return forceNode;
	}
	else if ( mCurrentElement.isNull() || name.isEmpty() ) {
		return QDomElement();
	}
	
	return name.isEmpty() ? mCurrentElement : mCurrentElement.firstChildElement( name );
}

void XmlObjectProcessor::removeChildNodes( QDomNode& node )
{
	const QDomNodeList nodes = node.childNodes();
	
	for ( int i = 0; i < nodes.count(); i++ ) {
		node.removeChild( nodes.at( i ) );
	}
}

bool XmlObjectProcessor::processData( const QString& name, XmlObject& object, const QDomElement& forceNode )
{
	switch ( mMode ) {
		case XmlObjectProcessor::Read:
			return unSerialize( name, object, forceNode );
		case XmlObjectProcessor::Write:
			return serialize( name, object, forceNode );
		default:
			Q_ASSERT( 0 );
			return false;
	}
}

bool XmlObjectProcessor::processData( const QString& name, QVariant& variant, const QDomElement& forceNode )
{
	switch ( mMode ) {
		case XmlObjectProcessor::Read:
			return unSerialize( name, variant, forceNode );
		case XmlObjectProcessor::Write:
			return serialize( name, variant, forceNode );
		default:
			Q_ASSERT( 0 );
			return false;
	}
}

bool XmlObjectProcessor::processData( const QString& name, QVariantList& variant, const QDomElement& forceNode )
{
	switch ( mMode ) {
		case XmlObjectProcessor::Read:
			return unSerialize( name, variant, forceNode );
		case XmlObjectProcessor::Write:
			return serialize( name, variant, forceNode );
		default:
			Q_ASSERT( 0 );
			return false;
	}
}

bool XmlObjectProcessor::processData( const QString& name, QVariantMap& variant, const QDomElement& forceNode )
{
	switch ( mMode ) {
		case XmlObjectProcessor::Read:
			return unSerialize( name, variant, forceNode );
		case XmlObjectProcessor::Write:
			return serialize( name, variant, forceNode );
		default:
			Q_ASSERT( 0 );
			return false;
	}
}

bool XmlObjectProcessor::processData( const QString& name, QVariantHash& variant, const QDomElement& forceNode )
{
	switch ( mMode ) {
		case XmlObjectProcessor::Read:
			return unSerialize( name, variant, forceNode );
		case XmlObjectProcessor::Write:
			return serialize( name, variant, forceNode );
		default:
			Q_ASSERT( 0 );
			return false;
	}
}

bool XmlObjectProcessor::unSerialize( QVariant& variant, const QDomNode& node ) const
{
	if ( node.isNull() ) {
		qWarning() << Q_FUNC_INFO << " WARNING: 0";
		return false;
	}
	
	variant = QVariantHelper::stringToVariant( node.nodeValue() );
	
	return true;
}

bool XmlObjectProcessor::serialize( const QVariant& variant, QDomNode& node )
{
	if ( node.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	const QString string = QVariantHelper::variantToString( variant );
	
	// write value if needed
	if ( string.isEmpty() ) {
		node.clear();
	}
	else {
		node.setNodeValue( string );
	}
	
	return true;
}

bool XmlObjectProcessor::unSerialize( QVariant& variant, const QDomElement& element ) const
{
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " WARNING: 0";
		return false;
	}
	
	return unSerialize( variant, element.attributes().namedItem( "content" ) );
}

bool XmlObjectProcessor::serialize( const QVariant& variant, QDomElement& element)
{
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	if ( !element.hasAttribute( "content" ) ) {
		element.setAttribute( "content", QString::null );
	}
	
	QDomNode node = element.attributes().namedItem( "content" );
	
	return serialize( variant, node );
}

bool XmlObjectProcessor::unSerialize( const QString& name, XmlObject& object, const QDomElement& forceNode ) const
{
	const QDomElement element = getUnSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " WARNING: 0";
		return false;
	}
	
	const QDomElement oldElement = mCurrentElement;
	mCurrentElement = element;
	object.processData( const_cast<XmlObjectProcessor*>( this ) );
	mCurrentElement = oldElement;
	
	return true;
}

bool XmlObjectProcessor::serialize( const QString& name, const XmlObject& object, const QDomElement& forceNode )
{
	QDomElement element = getSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	const QDomElement oldElement = mCurrentElement;
	mCurrentElement = element;
	element.setAttribute( "type", "XmlObject" );
	const_cast<XmlObject*>( &object )->processData( this );
	mCurrentElement = oldElement;
	
	return true;
}


bool XmlObjectProcessor::unSerialize( const QString& name, QVariant& variant, const QDomElement& forceNode ) const
{
	const QDomElement element = getUnSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " WARNING: 0" << name << variant;
		return false;
	}
	
	switch ( variant.type() ) {
		case QVariant::List: {
			QVariantList data;
			
			if ( !unSerialize( name, data, element ) ) {
				qWarning() << Q_FUNC_INFO << " ERROR: 0";
				return false;
			}
			
			variant = data;
			return true;
		}
		case QVariant::StringList: {
			QStringList data;
			
			if ( !unSerialize( name, data, element ) ) {
				qWarning() << Q_FUNC_INFO << " ERROR: 1";
				return false;
			}
			
			variant = data;
			return true;
		}
		case QVariant::Map: {
			QVariantMap data;
			
			if ( !unSerialize( name, data, element ) ) {
				qWarning() << Q_FUNC_INFO << " ERROR: 2";
				return false;
			}
			
			variant = data;
			return true;
		}
		case QVariant::Hash: {
			QVariantHash data;
			
			if ( !unSerialize( name, data, element ) ) {
				qWarning() << Q_FUNC_INFO << " ERROR: 3";
				return false;
			}
			
			variant = data;
			return true;
		}
		default:
			return unSerialize( variant, element );
	}
}

bool XmlObjectProcessor::serialize( const QString& name, const QVariant& variant, const QDomElement& forceNode )
{
	QDomElement element = getSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	switch ( variant.type() ) {
		case QVariant::List:
			return serialize( name, variant.toList(), element );
		case QVariant::StringList:
			return serialize( name, variant.toStringList(), element );
		case QVariant::Map:
			return serialize( name, variant.toMap(), element );
		case QVariant::Hash:
			return serialize( name, variant.toHash(), element );
		default:
			return serialize( variant, element );
	}
}

// QVariantList

bool XmlObjectProcessor::unSerialize( const QString& name, QVariantList& variant, const QDomElement& forceNode ) const
{
	const QDomElement element = getUnSerializeNode( name, forceNode );
	
	if ( element.isNull() || type( element.attribute( "type" ) ) != XmlObjectProcessor::VariantList ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	variant.clear();
	
	const QDomNodeList nodes = element.childNodes();
	
	for ( int i = 0; i < nodes.count(); i++ ) {
		const QDomElement child = nodes.at( i ).toElement();
		
		if ( child.isNull() ) {
			qWarning() << Q_FUNC_INFO << " WARNING: 0";
			continue;
		}
		
		if ( child.childNodes().isEmpty() ) {
			QVariant data;
			
			if ( unSerialize( "value", data, child ) ) {
				variant << data;
			}
			else {
				qWarning() << Q_FUNC_INFO << " ERROR: 1";
				return false;
			}
		}
		else {
			const QString _type = child.attribute( "type" );
			
			switch ( type( _type ) ) {
				case XmlObjectProcessor::VariantList: {
					QVariantList data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant << QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 2";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::StringList: {
					QStringList data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant << QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 3";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::VariantMap: {
					QVariantMap data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant << QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 4";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::VariantHash: {
					QVariantHash data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant << QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 5";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::Invalid:
					qWarning() << Q_FUNC_INFO << " WARNING: 1";
					break;
			}
		}
	}
	
	return true;
}

bool XmlObjectProcessor::serialize( const QString& name, const QVariantList& variant, const QDomElement& forceNode )
{
	QDomElement element = getSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	removeChildNodes( element );
	element.setAttribute( "type", typeToString( XmlObjectProcessor::VariantList ) );
	
	// write new data
	foreach ( const QVariant& value, variant ) {
		QDomElement child = element.ownerDocument().createElement( "value" );
		
		element.appendChild( child );
		
		if ( !serialize( "value", value, child ) ) {
			qWarning() << Q_FUNC_INFO << " ERROR: 2";
			return false;
		}
	}
	
	return true;
}

// QStringList

bool XmlObjectProcessor::unSerialize( const QString& name, QStringList& list, const QDomElement& forceNode ) const
{
	const QDomElement element = getUnSerializeNode( name, forceNode );
	
	if ( element.isNull() || type( element.attribute( "type" ) ) != XmlObjectProcessor::StringList ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	list.clear();
	
	const QDomNodeList nodes = element.childNodes();
	
	for ( int i = 0; i < nodes.count(); i++ ) {
		const QDomElement child = nodes.at( i ).toElement();
		
		if ( child.isNull() ) {
			qWarning() << Q_FUNC_INFO << " WARNING: 0";
			continue;
		}
		
		if ( child.childNodes().isEmpty() ) {
			QVariant data;
			
			if ( unSerialize( "value", data, child ) ) {
				list << data.toString();
			}
			else {
				qWarning() << Q_FUNC_INFO << " ERROR: 1";
				return false;
			}
		}
		else {
			qWarning() << Q_FUNC_INFO << " ERROR: 2";
			return false;
		}
	}
	
	return true;
}

bool XmlObjectProcessor::serialize( const QString& name, const QStringList& list, const QDomElement& forceNode )
{
	QDomElement element = getSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	removeChildNodes( element );
	element.setAttribute( "type", typeToString( XmlObjectProcessor::StringList ) );
	
	// write new data
	foreach ( const QVariant& value, list ) {
		QDomElement child = element.ownerDocument().createElement( "value" );
		
		element.appendChild( child );
		
		if ( !serialize( "value", value, child ) ) {
			qWarning() << Q_FUNC_INFO << " ERROR: 2";
			return false;
		}
	}
	
	return true;
}

// QVariantMap

bool XmlObjectProcessor::unSerialize( const QString& name, QVariantMap& variant, const QDomElement& forceNode ) const
{
	const QDomElement element = getUnSerializeNode( name, forceNode );
	
	if ( element.isNull() || type( element.attribute( "type" ) ) != XmlObjectProcessor::VariantMap ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	variant.clear();
	
	const QDomNodeList nodes = element.childNodes();
	
	for ( int i = 0; i < nodes.count(); i++ ) {
		const QDomElement child = nodes.at( i ).toElement();
		
		if ( child.isNull() ) {
			qWarning() << Q_FUNC_INFO << " WARNING: 0";
			continue;
		}
		
		if ( child.childNodes().isEmpty() ) {
			if ( !child.hasAttribute( "key" ) ) {
				qWarning() << Q_FUNC_INFO << " WARNING: 1";
				continue;
			}
			
			const QString key = child.attribute( "key" );
			QVariant content;
			
			if ( !unSerialize( content, child ) ) {
				qWarning() << Q_FUNC_INFO << " ERROR: 1";
				return false;
			}
			
			variant[ key ] = content;
		}
		else {
			if ( !child.hasAttribute( "key" ) || !child.hasAttribute( "type" ) ) {
				qWarning() << Q_FUNC_INFO << " WARNING: 2";
				continue;
			}
			
			const QString key = child.attribute( "key" );
			const QString _type = child.attribute( "type" );
			
			switch ( type( _type ) ) {
				case XmlObjectProcessor::VariantList: {
					QVariantList data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 2";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::StringList: {
					QStringList data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 3";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::VariantMap: {
					QVariantMap data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 4";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::VariantHash: {
					QVariantHash data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 5";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::Invalid:
					qWarning() << Q_FUNC_INFO << " WARNING: 3";
					break;
			}
		}
	}
	
	return true;
}

bool XmlObjectProcessor::serialize( const QString& name, const QVariantMap& variant, const QDomElement& forceNode )
{
	QDomElement element = getSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	// clear children element
	removeChildNodes( element );
	element.setAttribute( "type", typeToString( XmlObjectProcessor::VariantMap ) );
	
	// write new data
	foreach ( const QString& key, variant.keys() ) {
		const QVariant& content = variant[ key ];
		QDomElement child = element.ownerDocument().createElement( "value" );
		
		child.setAttribute( "key", key );
		element.appendChild( child );
		
		if ( !serialize( "value", content, child ) ) {
			qWarning() << Q_FUNC_INFO << " ERROR: 1";
			return false;
		}
	}
	
	return true;
}

// QVariantHash

bool XmlObjectProcessor::unSerialize( const QString& name, QVariantHash& variant, const QDomElement& forceNode ) const
{
	const QDomElement element = getUnSerializeNode( name, forceNode );
	
	if ( element.isNull() || type( element.attribute( "type" ) ) != XmlObjectProcessor::VariantHash ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	variant.clear();
	
	const QDomNodeList nodes = element.childNodes();
	
	for ( int i = 0; i < nodes.count(); i++ ) {
		const QDomElement child = nodes.at( i ).toElement();
		
		if ( child.isNull() ) {
			qWarning() << Q_FUNC_INFO << " WARNING: 0";
			continue;
		}
		
		if ( child.childNodes().isEmpty() ) {
			if ( !child.hasAttribute( "key" ) ) {
				qWarning() << Q_FUNC_INFO << " WARNING: 1";
				continue;
			}
			
			const QString key = child.attribute( "key" );
			QVariant content;
			
			if ( !unSerialize( content, child ) ) {
				qWarning() << Q_FUNC_INFO << " ERROR: 1";
				return false;
			}
			
			variant[ key ] = content;
		}
		else {
			if ( !child.hasAttribute( "key" ) || !child.hasAttribute( "type" ) ) {
				qWarning() << Q_FUNC_INFO << " WARNING: 2";
				continue;
			}
			
			const QString key = child.attribute( "key" );
			const QString _type = child.attribute( "type" );
			
			switch ( type( _type ) ) {
				case XmlObjectProcessor::VariantList: {
					QVariantList data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 2";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::StringList: {
					QStringList data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 3";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::VariantMap: {
					QVariantMap data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 4";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::VariantHash: {
					QVariantHash data;
					
					if ( unSerialize( "value", data, child ) ) {
						variant[ key ] = QVariant( data );
					}
					else {
						qWarning() << Q_FUNC_INFO << " ERROR: 5";
						return false;
					}
					
					break;
				}
				case XmlObjectProcessor::Invalid:
					qWarning() << Q_FUNC_INFO << " WARNING: 3";
					break;
			}
		}
	}
	
	return true;
}

bool XmlObjectProcessor::serialize( const QString& name, const QVariantHash& variant, const QDomElement& forceNode )
{
	QDomElement element = getSerializeNode( name, forceNode );
	
	if ( element.isNull() ) {
		qWarning() << Q_FUNC_INFO << " ERROR: 0";
		return false;
	}
	
	// clear children element
	removeChildNodes( element );
	element.setAttribute( "type", typeToString( XmlObjectProcessor::VariantHash ) );
	
	// write new data
	foreach ( const QString& key, variant.keys() ) {
		const QVariant& content = variant[ key ];
		QDomElement child = element.ownerDocument().createElement( "value" );
		
		child.setAttribute( "key", key );
		element.appendChild( child );
		
		if ( !serialize( "value", content, child ) ) {
			qWarning() << Q_FUNC_INFO << " ERROR: 1";
			return false;
		}
	}
	
	return true;
}
