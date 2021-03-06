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
#include "MakefileProjectItem.h"

#include <coremanager/MonkeyCore.h>
#include <pluginsmanager/PluginsManager.h>
#include <maininterface/UIMain.h>
#include <workspace/pFileManager.h>

#include <QDebug>

MakefileProjectItem::MakefileProjectItem()
    : XUPProjectItem()
{
}

QString MakefileProjectItem::projectType() const
{
    return PLUGIN_NAME;
}

void MakefileProjectItem::installCommands()
{
    QFile file( fileName() );
    
    if ( !file.open( QIODevice::ReadOnly ) ) {
        showError( tr( "Can't open file '%1'" ).arg( fileName() ) );
        return;
    }
    
    const QRegExp targetRex( "^([\\w\\-_\\d]+):.*" );
    QStringList targets;
    
    while ( !file.atEnd() ) {
        QString line = file.readLine();
        int pos = targetRex.indexIn( line );
        
        if ( pos != -1 ) {
            targets << targetRex.cap( 1 );
        }
    }
    
    CLIToolPlugin* make = MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stEnabled, "GNUMake" );
    
    if ( !make ) {
        showError( tr( "Can't build Makefile projects. GNUMake plugin not enabled" ) );
        return;
    }
    
    pCommand baseCmd = make->command();
    const QString makeFileArg = QString( "-f $cp$ " );
    
    baseCmd.setProject( this );
    baseCmd.setSkipOnError( false );
    baseCmd.setTryAllParsers( true );
    
    foreach( const QString& target, targets ) {
        pCommand cmd = baseCmd;
        cmd.setName( target );
        cmd.setText( target );
        cmd.setCommand( QString( "%1 %2 %3" ).arg( cmd.command() ).arg( makeFileArg ).arg( target ) );
        addCommand( "mBuilder", cmd );
    }
}

void MakefileProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
    Q_UNUSED( files );
    Q_UNUSED( scope );
    showError( tr( "Adding files to the Makefile project does not make sense. Press 'Edit current project...' for edit your Makefile" ) );
}

void MakefileProjectItem::removeValue( XUPItem* scope, bool deleteFiles )
{
    Q_UNUSED( scope );
    Q_UNUSED( deleteFiles );
    showError( tr( "Removing values from the Makefile project does not make sense. Press 'Edit current project...' for edit your Makefile" ) );
}

bool MakefileProjectItem::open( const QString& fileName, const QString& codec )
{
    const QFileInfo fileInfo = QFileInfo( fileName );
    
    if ( !fileInfo.isReadable() ) {
        showError( tr( "File '%1' is not readable." ).arg( fileName ) );
        return false;
    }
    
    mFileName = fileName;
    mCodec = codec;
    QString parentDirName = QFileInfo( fileInfo.path() ).fileName();
    mDocument.clear();
    mDomElement = mDocument.createElement( "project" );
    setAttribute( "name", QString( "%1/%2" ).arg( parentDirName ).arg( fileInfo.fileName() ) ); // "directoryname/Makefile"
    return true;
}

UIXUPEditor* MakefileProjectItem::newEditDialog() const
{
    MonkeyCore::fileManager()->openFile( fileName(), codec() );
    return 0; /* not edited */
}
