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
/*!
	\file QVariantHelper.h
	\date 2010-03-07
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief This is the processor of the XmlObject, it convert variaables to string, and string to variables.
*/
#ifndef XMLOBJECTPROCESSOR_H
#define XMLOBJECTPROCESSOR_H

#include "MonkeyExport.h"
#include "XmlObject.h"

class MONKEY_EXPORT_H XmlObjectProcessor
{
	friend class XmlObject;
	
public:
	enum Mode
	{
		Read = 0,
		Write
	};
	
	enum Type
	{
		Invalid = -1,
		VariantList,
		StringList,
		VariantMap,
		VariantHash
	};
	
	XmlObjectProcessor( const QDomElement& element, XmlObjectProcessor::Mode mode = XmlObjectProcessor::Read );
	
	XmlObjectProcessor( const XmlObjectProcessor& other );
	XmlObjectProcessor& operator=( const XmlObjectProcessor& other );
	bool operator==( const XmlObjectProcessor& other ) const;
	bool operator!=( const XmlObjectProcessor& other ) const;
	
	static QString xmlHeader();
	
	XmlObjectProcessor::Mode mode() const;
	XmlObjectProcessor::Type type( const QString& string ) const;
	QString typeToString( XmlObjectProcessor::Type type ) const;
	bool isReadMode() const;
	bool isWriteMode() const;
	QString toXml() const;
	
	bool processData( const QString& name, XmlObject& object, const QDomElement& forceNode = QDomElement() );
	bool processData( const QString& name, QVariant& variant, const QDomElement& forceNode = QDomElement() );
	bool processData( const QString& name, QVariantList& variant, const QDomElement& forceNode = QDomElement() );
	bool processData( const QString& name, QVariantMap& variant, const QDomElement& forceNode = QDomElement() );
	bool processData( const QString& name, QVariantHash& variant, const QDomElement& forceNode = QDomElement() );

protected:
	mutable QDomElement mCurrentElement;
	XmlObjectProcessor::Mode mMode;
	static QHash<XmlObjectProcessor::Type, QString> mTypes;
	
	QDomElement getSerializeNode( const QString& name, const QDomElement& forceNode );
	QDomElement getUnSerializeNode( const QString& name, const QDomElement& forceNode ) const;
	
	void removeChildNodes( QDomNode& node );
	
	// low level unserialize/serialize for basic type
	bool unSerialize( QVariant& variant, const QDomNode& node ) const;
	bool serialize( const QVariant& variant, QDomNode& node );
	
	// mid level unserialize/serialize may decide what is basic type or not
	bool unSerialize( QVariant& variant, const QDomElement& element ) const;
	bool serialize( const QVariant& variant, QDomElement& element);
	
	bool unSerialize( const QString& name, XmlObject& object, const QDomElement& forceNode = QDomElement() ) const;
	bool serialize( const QString& name, const XmlObject& object, const QDomElement& forceNode = QDomElement() );
	
	bool unSerialize( const QString& name, QVariant& variant, const QDomElement& forceNode = QDomElement() ) const;
	bool serialize( const QString& name, const QVariant& variant, const QDomElement& forceNode = QDomElement() );
	
	bool unSerialize( const QString& name, QVariantList& variant, const QDomElement& forceNode = QDomElement() ) const;
	bool serialize( const QString& name, const QVariantList& variant, const QDomElement& forceNode = QDomElement() );
	
	bool unSerialize( const QString& name, QStringList& list, const QDomElement& forceNode = QDomElement() ) const;
	bool serialize( const QString& name, const QStringList& list, const QDomElement& forceNode = QDomElement() );
	
	bool unSerialize( const QString& name, QVariantMap& variant, const QDomElement& forceNode = QDomElement() ) const;
	bool serialize( const QString& name, const QVariantMap& variant, const QDomElement& forceNode = QDomElement() );
	
	bool unSerialize( const QString& name, QVariantHash& variant, const QDomElement& forceNode = QDomElement() ) const;
	bool serialize( const QString& name, const QVariantHash& variant, const QDomElement& forceNode = QDomElement() );
};

#endif // XMLOBJECTPROCESSOR_H
