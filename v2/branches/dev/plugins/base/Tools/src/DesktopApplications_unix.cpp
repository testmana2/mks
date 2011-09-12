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
#include "DesktopApplications.h"

#include <pMonkeyStudio.h>

#include <QSet>
#include <QDir>
#include <QSettings>

QStringList DesktopApplications::startMenuPaths() const
{
    QStringList xdg_paths = QString::fromLocal8Bit( qgetenv( "XDG_DATA_DIRS" ) ).split( ':' );
    QSet<QString> paths;
    
    if ( xdg_paths.isEmpty() ) {
        xdg_paths << "/usr/share" << "/usr/local/share";
    }
    
    foreach ( const QString& path, xdg_paths ) {
        paths << QDir::cleanPath( QString( "%1/applications" ).arg( path ) );
    }
    
    return paths.toList();
}

bool DesktopApplications::categoriesAvailable() const
{
    return true;
}

void DesktopApplications::scan()
{
    // check all applications path
    foreach ( const QString& menuPath, startMenuPaths() ) {
        foreach ( const QFileInfo& applicationFile, pMonkeyStudio::getFiles( QDir( menuPath ), "*.desktop" ) ) {
            // get folder object
            DesktopFolder* df = &mStartMenu;
            // get relative path
            const QString applicationPath = applicationFile.absolutePath().remove( menuPath ).remove( 0, 1 );
            // get last folder object
            QString path;
            
            foreach ( const QString& part, applicationPath.split( "/", QString::SkipEmptyParts ) ) {
                path += part +"/";
                
                if ( df->folders.contains( part ) ) {
                    df = &df->folders[ part ];
                }
                else {
                    df->folders[ part ] = DesktopFolder( df );
                    df = &df->folders[ part ];
                    df->path = menuPath +"/" +path;
                    
                    if ( df->path.endsWith( "/" ) ) {
                        df->path.chop( 1 );
                    }
                }
            }
            
            // update application for breaking freeze
            QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
            
            // open file as settings file
            QSettings settings( applicationFile.absoluteFilePath(), QSettings::IniFormat );
            
            settings.beginGroup( "Desktop Entry" );
            
            if ( !df->applications.contains( applicationFile.absoluteFilePath() )
                && !settings.value( "Name" ).toString().isEmpty() ) {
                // add application
                DesktopApplication application( df );
                application.name = settings.value( "Name" ).toString();
                application.icon = settings.value( "Icon" ).toString();
                application.genericName = settings.value( "GenericName" ).toString();
                application.comment = settings.value( "Comment" ).toString();
                application.categories = settings.value( "Categories" ).toStringList();
                df->applications[ applicationFile.absoluteFilePath() ] = application;
            }
            
            settings.endGroup();
        }
    }
}
