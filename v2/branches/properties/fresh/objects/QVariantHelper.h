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
    \brief A helper to convert variant to/from strings
*/
#ifndef QVARIANTHELPER_H
#define QVARIANTHELPER_H

#include "MonkeyExport.h"

#include <QVariant>
#include <QStringList>

#ifndef QT_NO_GEOM_VARIANT
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QLine>
#endif

#ifndef QT_NO_DATASTREAM
#include <QDataStream>
#endif

namespace QVariantHelper
{
    MONKEY_EXPORT_H struct SplitArgsResult
    {
        SplitArgsResult()
        { type = QVariant::Invalid; }
        
        QList<qreal> args;
        QVariant::Type type;
    };
    
    MONKEY_EXPORT_H QVariantHelper::SplitArgsResult splitArgs( const QString& string );
    MONKEY_EXPORT_H QString variantToString( const QVariant& variant );
    MONKEY_EXPORT_H QVariant stringToVariant( const QString& string );
};

#endif // QVARIANTHELPER_H
