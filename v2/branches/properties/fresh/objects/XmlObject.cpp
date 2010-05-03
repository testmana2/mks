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
#include "XmlObject.h"
#include "XmlObjectProcessor.h"

#include <QDir>

XmlObject::XmlObject()
{
}

XmlObject::~XmlObject()
{
}

XmlObject::XmlObject( const XmlObject& other )
{
	operator=( other );
}

XmlObject& XmlObject::operator=( const XmlObject& other )
{
	if ( *this != other )
	{
		readFromString( other.toXml(), other.rootTagName() );
	}

	return *this;
}

bool XmlObject::operator==( const XmlObject& other ) const
{
	return toXml() == other.toXml();
}

bool XmlObject::operator!=( const XmlObject& other ) const
{
	return !operator==( other );
}

QString XmlObject::rootTagName() const
{
	return mRootTagName.isEmpty() ? "XmlObject" : mRootTagName;
}

QString XmlObject::toXml( const QString& rootTag, bool header ) const
{
	const QString tag = rootTag.isEmpty() ? rootTagName() : rootTag;
	QDomDocument document;
	QString data;
	
	if ( header ) {
		data += XmlObjectProcessor::xmlHeader();
	}
	
	data += QString( "<%1>" ).arg( tag );
	data += QString( "</%1>" ).arg( tag );
	
	if ( document.setContent( data, &data ) ) {
		XmlObjectProcessor container( document.documentElement(), XmlObjectProcessor::Write );
		
		const_cast<XmlObject*>( this )->processData( &container );
		
		return container.toXml();
	}
	
	return data;
}

bool XmlObject::readFromDocument( const QDomDocument& document, const QString& rootTag, QString* errorString )
{
	const QString tag = rootTag.isEmpty() ? rootTagName() : rootTag;
	const QDomElement root = document.documentElement();
	
	if ( !root.isNull() && root.nodeName() == tag ) {
		XmlObjectProcessor container( root, XmlObjectProcessor::Read );
		
		processData( &container );
		
		return true;
	}
	
	if ( errorString ) {
		*errorString = "No root element node or nodeName mismatch";
	}
	
	return false;
}

bool XmlObject::readFromString( const QString& contents, const QString& rootTag, QString* errorString )
{
	const QString tag = rootTag.isEmpty() ? rootTagName() : rootTag;
	const QString data = contents;
	QDomDocument document;
	
	if ( document.setContent( data, errorString ) ) {
		return readFromDocument( document, tag, errorString );
	}
	
	return false;
}

bool XmlObject::readFromDevice( QIODevice* device, const QString& rootTag, QString* errorString )
{
	Q_ASSERT( device );
	
	const QString tag = rootTag.isEmpty() ? rootTagName() : rootTag;
	const QString data = QString::fromUtf8( device->readAll() );
	
	return readFromString( data, tag, errorString );
}

bool XmlObject::readFromFile( const QString& fileName, const QString& rootTag, QString* errorString )
{
	QFile file( fileName );
	
	if ( !file.exists() ) {
		if ( errorString ) {
			*errorString = "The file doesn't exists.";
		}
		
		return false;
	}
	
	if ( !file.open( QIODevice::ReadOnly ) ) {
		if ( errorString ) {
			*errorString = file.errorString();
		}
		
		return false;
	}
	
	return readFromDevice( &file, rootTag, errorString );
}

// WRITEXML

bool XmlObject::writeToFile( const QString& fileName, const QString& rootTag, QString* errorString )
{
	if ( fileName.isEmpty() ) {
		return false;
	}
	
	const QString tag = rootTag.isEmpty() ? rootTagName() : rootTag;
	const QString data = toXml( tag, true );
	const QString path = QFileInfo( fileName ).absolutePath();
	QDir dir( path );
	
	if ( !dir.exists() ) {
		if ( !dir.mkpath( path ) ) {
			*errorString = QString( "Can't create path: %1" ).arg( path );
			return false;
		}
	}
	
	QFile file( fileName );
	
	if ( !file.open( QIODevice::WriteOnly ) ) {
		*errorString = file.errorString();
		return false;
	}
	
	file.resize( 0 );
	const qint64 wrotten = file.write( data.toUtf8() );
	file.close();
	
	if ( wrotten == -1 ) {
		*errorString = "Can't write data";
	}
	
	return wrotten != -1;
}

void XmlObject::processData( XmlObjectProcessor* container )
{
	Q_UNUSED( container );
}
