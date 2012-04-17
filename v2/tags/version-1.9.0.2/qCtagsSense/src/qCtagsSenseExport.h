/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
    \file qCtagsSenseExport.h
    \date 2009-04-11 22:00
    \author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
    \brief Export Symbol Macros
*/
#ifndef QCTAGSSENSE_EXPORT_H
#define QCTAGSSENSE_EXPORT_H

#include <qglobal.h>

/*!
    \def Q_MONKEY_EXPORT
    \details This macro allow symbols to be exported/imported for Window OS
*/

#ifdef QCTAGSSENSE_EXPORT_BUILD
#define QCTAGSSENSE_EXPORT Q_DECL_EXPORT
#else
#define QCTAGSSENSE_EXPORT Q_DECL_IMPORT
#endif

#endif // QCTAGSSENSE_EXPORT_H
