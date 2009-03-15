/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pAbbreviationsManager.h
** Date      : 2008-01-14T00:36:49
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#ifndef PABBREVIATIONSMANAGER_H
#define PABBREVIATIONSMANAGER_H

#include <fresh.h>

#include <QString>
#include <QList>

class pEditor;

struct Q_MONKEY_EXPORT pAbbreviation
{
	pAbbreviation() {}
	pAbbreviation( const QString& t, const QString& d, const QString& l, const QString& c )
		: Template( t ), Description( d ), Language( l ), Code( c ) {}

	QString Template;
	QString Description;
	QString Language;
	QString Code;

	bool operator==( const pAbbreviation& other ) const
	{
		return Template == other.Template && Description == other.Description &&
			Language == other.Language && Code == other.Code;
	}
	
	bool operator!=( const pAbbreviation& other ) const
	{
		return !operator==( other );
	}
};

typedef QList<pAbbreviation> pAbbreviationList;

class pAbbreviationsManagerV2
{
public:
	bool addAbbreviation( const pAbbreviation& abbreviation );
	bool addAbbreviations( const pAbbreviationList& abbreviations );
	bool setAbbreviations( const pAbbreviationList& abbreviations );
	pAbbreviationList abbreviations() const;

protected:
	bool writeAbbreviations( const pAbbreviationList& abbreviations ) const;
	bool readAbbreviations( pAbbreviationList& abbreviations ) const;
};

namespace pAbbreviationsManager
{
	void registerShellCommand();
	const QList<pAbbreviation> defaultAbbreviations();
	const QList<pAbbreviation> availableAbbreviations();
	const pAbbreviation getAbbreviation( const QString& macro, const QString& language );
	void expandAbbreviation( pEditor* );

};

#endif // PABBREVIATIONSMANAGER_H
