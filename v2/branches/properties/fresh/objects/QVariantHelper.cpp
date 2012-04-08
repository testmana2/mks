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
#include "QVariantHelper.h"

#include <QFont>
#include <QColor>
#include <QDebug>

/*
    Not directly handled variant type:
    QVariant::Bitmap
    QVariant::Brush
    QVariant::Char
    QVariant::Cursor
    QVariant::Icon
    QVariant::Image
    QVariant::Matrix
    QVariant::Transform
    QVariant::Matrix4x4
    QVariant::Palette
    QVariant::Pen
    QVariant::Pixmap
    QVariant::Polygon
    QVariant::Quaternion
    QVariant::RegExp
    QVariant::Region
    QVariant::SizePolicy
    QVariant::TextFormat <-
    QVariant::TextLength
    QVariant::Vector2D
    QVariant::Vector3D
    QVariant::Vector4D
    QVariant::UserType
*/

QVariantHelper::SplitArgsResult QVariantHelper::splitArgs( const QString& _string )
{
    const QString string = _string.simplified();
    static const QSet<QChar> splitters = QSet<QChar>() << ',' << ' ' << 'x';
    QVariantHelper::SplitArgsResult result;
    QMap<QChar, int> foundSplitters;
    QString part;
    bool ok;
    qreal value;
    
    // get values
    for ( int i = 0; i < string.count(); i++ ) {
        const QChar c = string.at( i ).toLower();
        
        if ( splitters.contains( c ) ) {
            foundSplitters[ c ]++;
            value = QVariant( part ).toReal( &ok );
            
            if ( !ok ) {
                return QVariantHelper::SplitArgsResult();
            }
            
            result.args << value;
            part.clear();
        }
        else {
            part.append( c );
        }
    }
    
    if ( !part.isEmpty() ) {
        value = QVariant( part ).toReal( &ok );
        
        if ( ok ) {
            result.args << value;
        }
    }
    
    // determine variant type
    static QMap<int, QSet<QVariant::Type> > typesMap;
    
    if ( typesMap.isEmpty() ) {
        typesMap[ 2 ] << QVariant::PointF;
        typesMap[ 2 ] << QVariant::SizeF;
        typesMap[ 4 ] << QVariant::RectF;
        typesMap[ 4 ] << QVariant::LineF;
        typesMap[ 4 ] << QVariant::Color;
    }
    
    // get finalist list
    const QSet<QVariant::Type> types = typesMap.value( result.args.count() );
    
    // determine final type
    foreach ( const QVariant::Type& type, types ) {
        switch ( type )
        {
            case QVariant::PointF: {
                if ( foundSplitters.value( ',' ) == 1 ) {
                    result.type = type;
                }
                
                break;
            }
            case QVariant::SizeF: {
                if ( foundSplitters.value( 'x' ) == 1 ) {
                    result.type = type;
                }
                
                break;
            }
            case QVariant::RectF: {
                if ( foundSplitters.value( ',' ) == 1 && foundSplitters.value( 'x' ) == 1 ) {
                    result.type = type;
                }
                
                break;
            }
            case QVariant::LineF: {
                if ( foundSplitters.value( ',' ) == 2 ) {
                    result.type = type;
                }
                
                break;
            }
            case QVariant::Color: {
                if ( foundSplitters.value( ' ' ) == 4 ) {
                    result.type = type;
                }
                
                break;
            }
            default: {
                qWarning() << Q_FUNC_INFO << type << types << foundSplitters << result.args << string;
                Q_ASSERT( 0 );
                break;
            }
        }
        
        if ( result.type != QVariant::Invalid ) {
            break;
        }
    }

    return result;
}

QString QVariantHelper::variantToString( const QVariant& variant )
{
    QString result;

    switch ( variant.type() ) {
        case QVariant::Invalid:
            result = QString::null;
            break;

        case QVariant::ByteArray: {
            const QByteArray data = variant.toByteArray().toBase64();
            result = QString( "ByteArray:%1" ).arg( QString::fromLatin1( data, data.length() ) );
            break;
        }

        case QVariant::Point:
        case QVariant::PointF: {
            const QPointF point = variant.toPointF();
            result = QString( "%1,%2" )
                .arg( point.x() )
                .arg( point.y() );
            break;
        }
        
        case QVariant::Size:
        case QVariant::SizeF: {
            const QSizeF size = variant.toSizeF();
            result = QString( "%1x%2" )
                .arg( size.width() )
                .arg( size.height() );
            break;
        }
        
        case QVariant::Rect:
        case QVariant::RectF: {
            const QRectF rect= variant.toRectF();
            result = QString( "%1 %2" )
                .arg( variantToString( rect.topLeft() ) )
                .arg( variantToString( rect.size() ) );
            break;
        }
        
        case QVariant::Line:
        case QVariant::LineF: {
            const QLineF line = variant.toLine();
            result = QString( "%1 %2" )
                .arg( variantToString( line.p1() ) )
                .arg( variantToString( line.p2() ) );
            break;
        }
        
        case QVariant::Color: {
            const QColor color = variant.value<QColor>();
            result = QString( "%1 %2 %3 %4" )
                .arg( color.red() )
                .arg( color.green() )
                .arg( color.blue() )
                .arg( color.alpha() );
            break;
        }
        
        case QVariant::Font: {
            result = variant.value<QFont>().toString();
            break;
        }
        
        case QVariant::Locale: {
            const QLocale locale = variant.value<QLocale>();
            result = QString( "Locale:%1" ).arg( locale.name() );
            break;
        }

        case QVariant::DateTime:
        case QVariant::Date:
        case QVariant::Time:
        case QVariant::String:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::Url:
        case QVariant::KeySequence: {
            result = variant.toString();
            break;
        }

        default:
        {
            QByteArray data;
            
            {
                QDataStream stream( &data, QIODevice::WriteOnly );
                stream.setVersion( QDataStream::Qt_4_0 );
                stream << variant;
            }
            
            data = data.toBase64();
            result = QString( "Variant:%1" ).arg( QString::fromLatin1( data, data.length() ) );
            break;
        }
    }

    return result;
}

QVariant QVariantHelper::stringToVariant( const QString& string )
{
    if ( string == QString::null ) {
        return QVariant();
    }
    else if ( string.startsWith( QLatin1String( "ByteArray:" ) ) ) {
        return QVariant( QByteArray::fromBase64( string.mid( 10 ).toLatin1() ) );
    }
    else if ( string.startsWith( QLatin1String( "Variant:" ) ) ) {
        QByteArray data = QByteArray::fromBase64( string.mid( 8 ).toLatin1() );
        QDataStream stream( &data, QIODevice::ReadOnly );
        stream.setVersion( QDataStream::Qt_4_0 );
        QVariant result;
        stream >> result;
        return result;
    }
    else if ( string.startsWith( QLatin1String( "Locale:" ) ) ) {
        return QVariant( QLocale( string.mid( 7 ) ) );
    }
    
    const QVariantHelper::SplitArgsResult result = splitArgs( string );
    const QList<qreal>& args = result.args;
    
    switch ( result.type )
    {
        case QVariant::Point:
        case QVariant::PointF:
            return QVariant( QPointF( args[0], args[1] ) );
        case QVariant::Size:
        case QVariant::SizeF:
            return QVariant( QSizeF( args[0], args[1] ) );
        case QVariant::Rect:
        case QVariant::RectF:
            return QVariant( QRectF( args[0], args[1], args[2], args[3] ) );
        case QVariant::Line:
        case QVariant::LineF:
            return QVariant( QLineF( QPointF( args[0], args[1] ), QPointF( args[2], args[3] ) ) );
        case QVariant::Color:
            return QVariant( QColor( args[0], args[1], args[2], args[3] ) );
        default:
            break;
    }
    
    return QVariant( string );
}
