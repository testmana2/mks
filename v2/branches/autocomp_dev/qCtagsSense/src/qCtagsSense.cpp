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
#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSenseIndexer.h"

#include <QMetaType>
#include <QDebug>

qCtagsSenseIndexer* qCtagsSense::mIndexer = 0;

qCtagsSense::qCtagsSense( QObject* parent )
    : QObject( parent )
{
    qCtagsSenseUtils::initMaps();
    
    qRegisterMetaType<qCtagsSenseEntry>( "qCtagsSenseEntry" );
    qRegisterMetaType<qCtagsSenseEntry*>( "qCtagsSenseEntry*" );
    
    if( !mIndexer )
        mIndexer = new qCtagsSenseIndexer();
    
    connect( mIndexer, SIGNAL( indexingStarted() ), this, SIGNAL( indexingStarted() ) );
    connect( mIndexer, SIGNAL( indexingProgress( int, int ) ), this, SIGNAL( indexingProgress( int, int ) ) );
    connect( mIndexer, SIGNAL( indexingFinished() ), this, SIGNAL( indexingFinished() ) );
    connect( mIndexer, SIGNAL( indexingChanged() ), this, SIGNAL( indexingChanged() ) );
}

qCtagsSense::~qCtagsSense()
{
    delete mIndexer;
}

qCtagsSenseProperties qCtagsSense::properties() const
{
    return mProperties;
}

qCtagsSenseIndexer* qCtagsSense::indexer() const
{
    return mIndexer;
}

void qCtagsSense::setProperties( const qCtagsSenseProperties& properties )
{
    if ( properties != mProperties )
    {
        mProperties = properties;
        mIndexer->setFilteredSuffixes( properties.FilteredSuffixes );
        
        foreach ( const QString& path, properties.SystemPaths )
        {
            mIndexer->indexFile( path, properties );
        }
    }
}

void qCtagsSense::tagEntry( const QString& fileName )
{
    mIndexer->indexFile( fileName, mProperties );
}

void qCtagsSense::tagEntries( const QStringList& fileNames )
{
    mIndexer->indexFiles( fileNames, mProperties );
}

void qCtagsSense::tagEntries( const QMap<QString, QString>& entries )
{
    mIndexer->indexBuffers( entries, mProperties );
}
