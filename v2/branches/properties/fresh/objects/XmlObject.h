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
	\brief A class that act as a properties wrapper around an xml file
*/
#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include "MonkeyExport.h"

#include <QDomDocument>
#include <QVariant>

class XmlObjectProcessor;
class QIODevice;

class MONKEY_EXPORT_H XmlObject
{
	friend class XmlObjectProcessor;
	
public:
	XmlObject();
	virtual ~XmlObject();
	
	XmlObject( const XmlObject& other );
	XmlObject& operator=( const XmlObject& other );
	bool operator==( const XmlObject& other ) const;
	bool operator!=( const XmlObject& other ) const;
	
	QString rootTagName() const;
	QString toXml( const QString& rootTag = QString::null, bool header = true ) const;
	
	// Read a QDomDocument with a 'root' xml tag named <rootTag>
	bool readFromDocument( const QDomDocument& document, const QString& rootTag = QString::null, QString* errorString = 0 );
	// Read a QString representing a xml structure with a 'root' xml tag named <rootTag>
	bool readFromString( const QString& contents, const QString& rootTag, QString* errorString = 0 );
	// Read a QIODevice representing a xml structure with a 'root' xml tag named <rootTag>
	bool readFromDevice( QIODevice* device, const QString& rootTag = QString::null, QString* errorString = 0 );
	// Read a Xml file with a 'root' xml tag named <rootTag>
	bool readFromFile( const QString& fileName, const QString& rootTag = QString::null, QString* errorString = 0 );

	// Write a Xml file with a 'root' xml tag named <rootTag>
	bool writeToFile( const QString& fileName, const QString& rootTag = QString::null, QString* errorString = 0 );

protected:
	QString mRootTagName;
	
	virtual void processData( XmlObjectProcessor* container );
};

typedef QList<XmlObject*> XmlObjectPointerList;

#endif // XMLOBJECT_H
