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
#include "workspace/pFileManager.h"
#include "workspace/pWorkspace.h"
#include "xupmanager/gui/XUPProjectManager.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "recentsmanager/pRecentsManager.h"
#include "pAbstractChild.h"
#include "coremanager/MonkeyCore.h"
#include "pMonkeyStudio.h"
#include "settingsmanager/Settings.h"
#include "shellmanager/MkSShellInterpreter.h"
#include "pluginsmanager/PluginsManager.h"
#include "maininterface/UIMain.h"
#include "shared/MkSFileDialog.h"
#include "pChild.h"

#include <pQueuedMessageToolBar.h>

pFileManager::pFileManager( QObject* o )
    : QObject( o )
{
    initializeInterpreterCommands();
    
    // files
    connect( MonkeyCore::workspace(), SIGNAL( documentOpened( pAbstractChild* ) ), this, SIGNAL( documentOpened( pAbstractChild* ) ) );
    connect( MonkeyCore::workspace(), SIGNAL( documentModifiedChanged( pAbstractChild*, bool ) ), this, SIGNAL( documentModifiedChanged( pAbstractChild*, bool ) ) );
    connect( MonkeyCore::workspace(), SIGNAL( documentChanged( pAbstractChild* ) ), this, SIGNAL( documentChanged( pAbstractChild* ) ) );
    connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( pAbstractChild* ) ), this, SIGNAL( documentAboutToClose( pAbstractChild* ) ) );
    connect( MonkeyCore::workspace(), SIGNAL( documentClosed( pAbstractChild* ) ), this, SIGNAL( documentClosed( pAbstractChild* ) ) );
    connect( MonkeyCore::workspace(), SIGNAL( documentReloaded( pAbstractChild* ) ), this, SIGNAL( documentReloaded( pAbstractChild* ) ) );
    connect( MonkeyCore::workspace(), SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SIGNAL( currentDocumentChanged( pAbstractChild* ) ) );
    
    // projects
    connect( MonkeyCore::projectsManager(), SIGNAL( projectOpened( XUPProjectItem* ) ), this, SIGNAL( opened( XUPProjectItem* ) ) );
    connect( MonkeyCore::projectsManager(), SIGNAL( projectAboutToClose( XUPProjectItem* ) ), this, SIGNAL( aboutToClose( XUPProjectItem* ) ) );
    connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem* ) ), this, SIGNAL( currentChanged( XUPProjectItem* ) ) );
    connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ), this, SIGNAL( currentChanged( XUPProjectItem*, XUPProjectItem* ) ) );
}

void pFileManager::initializeInterpreterCommands()
{
    // register command
    QString help = MkSShellInterpreter::tr
    (
        "This command manage the associations, usage:\n"
        "The suffixes are a comma separated list of suffixes (ie: \"*.txt, *.doc\")\n"
        "\tassociation add [type] [suffixes]\n"
        "\tassociation set [type] [suffixes]\n"
        "\tassociation del [type] [suffixes]\n"
        "\tassociation list [type] -- If type is missing, all suffixes will be shown.\n"
        "\tassociation clear [type] -- If type is missing, all suffixes will be cleared."
    );
    
    MonkeyCore::interpreter()->addCommandImplementation( "association", pFileManager::commandInterpreter, help, this );
}

QString pFileManager::commandInterpreter( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter, void* data )
{
    Q_UNUSED( command );
    Q_UNUSED( interpreter );
    pFileManager* manager = static_cast<pFileManager*>( data );
    const QStringList allowedOperations = QStringList( "add" ) << "set" << "del" << "list" << "clear";
    
    if ( result )
    {
        *result = MkSShellInterpreter::NoError;
    }
    
    if ( arguments.isEmpty() )
    {
        if ( result )
        {
            *result = MkSShellInterpreter::InvalidCommand;
        }
        
        return MkSShellInterpreter::tr( "Operation not defined. Available operations are: %1." ).arg( allowedOperations.join( ", " ) );
    }
    
    const QString operation = arguments.first();
    
    if ( !allowedOperations.contains( operation ) )
    {
        if ( result )
        {
            *result = MkSShellInterpreter::InvalidCommand;
        }
        
        return MkSShellInterpreter::tr( "Unknown operation: '%1'." ).arg( operation );
    }
    
    if ( operation == "add" )
    {
        if ( arguments.count() != 3 )
        {
            if ( result )
            {
                *result = MkSShellInterpreter::InvalidCommand;
            }
            
            return MkSShellInterpreter::tr( "'add' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
        }
        
        const QString type = arguments.at( 1 );
        const QStringList suffixes = arguments.at( 2 ).split( ",", QString::SkipEmptyParts );
        
        manager->addCommand( type, suffixes );
    }
    
    if ( operation == "set" )
    {
        if ( arguments.count() != 3 )
        {
            if ( result )
            {
                *result = MkSShellInterpreter::InvalidCommand;
            }
            
            return MkSShellInterpreter::tr( "'set' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
        }
        
        const QString type = arguments.at( 1 );
        const QStringList suffixes = arguments.at( 2 ).split( ",", QString::SkipEmptyParts );
        
        manager->setCommand( type, suffixes );
    }
    
    if ( operation == "del" )
    {
        if ( arguments.count() != 3 )
        {
            if ( result )
            {
                *result = MkSShellInterpreter::InvalidCommand;
            }
            
            return MkSShellInterpreter::tr( "'del' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
        }
        
        const QString type = arguments.at( 1 );
        const QStringList suffixes = arguments.at( 2 ).split( ",", QString::SkipEmptyParts );
        
        manager->removeCommand( type, suffixes );
    }
    
    if ( operation == "list" )
    {
        if ( arguments.count() > 2 )
        {
            if ( result )
            {
                *result = MkSShellInterpreter::InvalidCommand;
            }
            
            return MkSShellInterpreter::tr( "'list' operation take 0 or 1 argument, %1 given." ).arg( arguments.count() -1 );
        }
        
        const QString type = arguments.value( 1 );
        QStringList output;
        
        if ( type.isNull() )
        {
            foreach ( const QString& ctype, manager->associations().keys() )
            {
                output << QString( "%1:" ).arg( ctype );
                output << QString( "\t%1" ).arg( manager->associations( ctype ).join( ", " ) );
            }
        }
        else
        {
            output << QString( "%1:" ).arg( type );
            output << QString( "\t%1" ).arg( manager->associations( type ).join( ", " ) );
        }
        
        if ( !output.isEmpty() )
        {
            output.prepend( MkSShellInterpreter::tr( "Found associations:" ) );
        }
        else
        {
            output << MkSShellInterpreter::tr( "No associations found." );
        }
        
        return output.join( "\n" );
    }
    
    if ( operation == "clear" )
    {
        if ( arguments.size() > 2 )
        {
            if ( result )
            {
                *result = MkSShellInterpreter::InvalidCommand;
            }
            
            return MkSShellInterpreter::tr( "'clear' operation take 0 or 1 argument, %1 given." ).arg( arguments.count() -1 );
        }
        
        const QString type = arguments.value( 1 );
        
        manager->clearCommand( type );
    }
    
    return QString::null;
}

void pFileManager::clearCommand( const QString& type )
{
    if ( type.isNull() )
    {
        mAssociations.clear();
    }
    else
    {
        mAssociations.remove( type );
    }
}

void pFileManager::addCommand( const QString& type, const QStringList& suffixes )
{
    foreach ( const QString& suffix, suffixes )
    {
        const QString trimmedSuffix = suffix.trimmed();
        
        if ( !mAssociations[ type ].contains( trimmedSuffix ) )
        {
            mAssociations[ type ] << trimmedSuffix;
        }
    }
}

void pFileManager::addCommand( const QString& type, const QString& suffix )
{
    addCommand( type, QStringList( suffix ) );
}

void pFileManager::setCommand( const QString& type, const QStringList& suffixes )
{
    clearCommand( type );
    addCommand( type, suffixes );
}

void pFileManager::setCommand( const QString& type, const QString& suffix )
{
    setCommand( type, QStringList( suffix ) );
}

void pFileManager::removeCommand( const QString& type, const QStringList& suffixes )
{
    QStringList result = associations( type );
    
    foreach ( const QString& suffix, suffixes )
    {
        const QString trimmedSuffix = suffix.trimmed();
        
        result.removeOne( trimmedSuffix );
    }
    
    if ( result.isEmpty() )
    {
        clearCommand( type );
    }
    else
    {
        mAssociations[ type ] = result;
    }
}

void pFileManager::removeCommand( const QString& type, const QString& suffix )
{
    removeCommand( type, QStringList( suffix ) );
}

const QMap<QString, QStringList>& pFileManager::associations() const
{
    return mAssociations;
}

QStringList pFileManager::associations( const QString& type ) const
{
    return mAssociations.value( type );
}

void pFileManager::generateScript()
{
    // write content in utf8
    const QString fn = MonkeyCore::settings()->homePath( Settings::SP_SCRIPTS ).append( "/associations.mks" );
    QFile file( fn );
    QStringList buffer;
    
    if ( !file.open( QIODevice::WriteOnly ) )
    {
        MonkeyCore::messageManager()->appendMessage( tr( "Can't open file for generating associations script: %1" ).arg( file.errorString() ) );
        return;
    }
    
    file.resize( 0 );
    
    buffer << "# Monkey Studio IDE Associations";
    buffer << "# reset associations";
    buffer << "association clear";
    buffer << "# introduce new ones per language";
    buffer << "# association add language/type Suffixes";
    buffer << "# association set language/type Suffixes";
    
    foreach ( const QString& type, mAssociations.keys() )
    {
        buffer << QString( "# %1" ).arg( type );
        
        if ( !mAssociations[ type ].isEmpty() )
        {
            buffer << QString( "association set \"%1\" \"%2\"" )
                .arg( type )
                .arg( mAssociations[ type ].join( ", " ) );
        }
    }
    
    if ( file.write( buffer.join( "\n" ).toUtf8() ) == -1 )
    {
        MonkeyCore::messageManager()->appendMessage( tr( "Can't write generated associations script: %1" ).arg( file.errorString() ) );
    }
    
    file.close();
}

pAbstractChild* pFileManager::openedDocument( const QString& fileName ) const
{
    foreach ( pAbstractChild* document, MonkeyCore::workspace()->documents() )
    {
        if ( pMonkeyStudio::isSameFile( document->filePath(), fileName ) )
        {
            return document;
        }
    }
    
    return 0;
}

QString pFileManager::fileBuffer( const QString& fileName, const QString& codec, bool& ok ) const
{
    pAbstractChild* document = openedDocument( fileName );
    
    if ( document )
    {
        ok = true;
        return document->fileBuffer();
    }
    
    QString result;
    ok = false;
    QFile file( fileName );
    
    if ( file.exists() )
    {
        if ( file.open( QIODevice::ReadOnly ) )
        {
            QTextCodec* c = QTextCodec::codecForName( codec.toUtf8() );
            result = c->toUnicode( file.readAll() );
            ok = true;
            file.close();
        }
    }
    
    return result;
}

void pFileManager::computeModifiedBuffers()
{
    QMap<QString, QString> entries;
    
    foreach ( pAbstractChild* document, MonkeyCore::workspace()->documents() )
    {
        if ( document->isModified() )
        {
            const QString content = document->fileBuffer();
            entries[ document->filePath() ] = content;
        }
    }
    
    emit buffersChanged( entries );
}

XUPProjectItem* pFileManager::currentProject() const
{
    return MonkeyCore::projectsManager()->currentProject();
}

QString pFileManager::currentProjectFile() const
{
    XUPProjectItem* curProject = currentProject();
    return curProject ? curProject->fileName() : QString::null;
}

QString pFileManager::currentProjectPath() const
{
    XUPProjectItem* curProject = currentProject();
    return curProject ? curProject->path() : QString::null;
}

pAbstractChild* pFileManager::currentDocument() const
{
    return MonkeyCore::workspace()->currentDocument();
}

QString pFileManager::currentDocumentFile() const
{
    pAbstractChild* document = currentDocument();
    return document ? document->filePath() : QString::null;
}

QString pFileManager::currentDocumentPath() const
{
    return QFileInfo( currentDocumentFile() ).path();
}

XUPItem* pFileManager::currentItem() const
{
    return MonkeyCore::projectsManager()->currentItem();
}

QString pFileManager::currentItemFile() const
{
    XUPItem* item = currentItem();
    
    if ( item && item->type() == XUPItem::File )
    {
        QString fn = item->content();
        return item->project()->filePath( fn );
    }
    
    return QString::null;
}

QString pFileManager::currentItemPath() const
{
    XUPItem* item = currentItem();
    
    if ( item && item->type() == XUPItem::Path )
    {
        QString fn = item->content();
        return item->project()->filePath( fn );
    }
    
    return QString::null;
}

void pFileManager::fileOpen_triggered()
{
    const QString filter = pMonkeyStudio::availableFilesFilter(); // get available filters
    QString path = MonkeyCore::fileManager()->currentDocumentFile(); // path to show
    
    if ( path.isEmpty() ) {
        XUPProjectItem* curProject = MonkeyCore::projectsManager()->currentProject();
        path = curProject ? curProject->path() : pMonkeyStudio::defaultProjectsDirectory();
    }
    
    // show filedialog to user
    const pFileDialogResult result = MkSFileDialog::getOpenFileNames( true, MonkeyCore::mainWindow(), tr( "Choose the file(s) to open" ), path, filter, true );

    // open open file dialog
    const QStringList fileNames = result[ "filenames" ].toStringList();
    
    // return 0 if user cancel
    if ( fileNames.isEmpty() ) {
        return;
    }

    // for each entry, open file
    foreach ( const QString& file, fileNames ) {
        open( file, result[ "codec" ].toString() );
    }
}

void pFileManager::fileOpenText_triggered()
{
    const QString filter = pMonkeyStudio::availableFilesFilter(); // get available filters
    QString path = MonkeyCore::fileManager()->currentDocumentFile(); // path to show
    
    if ( path.isEmpty() ) {
        XUPProjectItem* curProject = MonkeyCore::projectsManager()->currentProject();
        path = curProject ? curProject->path() : pMonkeyStudio::defaultProjectsDirectory();
    }
    
    // show filedialog to user
    const pFileDialogResult result = MkSFileDialog::getOpenFileNames( true, MonkeyCore::mainWindow(), tr( "Choose the file(s) to open" ), path, filter, true );

    // open open file dialog
    const QStringList fileNames = result[ "filenames" ].toStringList();
    
    // return 0 if user cancel
    if ( fileNames.isEmpty() ) {
        return;
    }

    // for each entry, open file
    foreach ( const QString& file, fileNames ) {
        if ( openFile( file, result[ "codec" ].toString(), true ) ) {
            // append file to recents
            MonkeyCore::recentsManager()->addRecentFile( file );
        }
        else {
            // remove it from recents files
            MonkeyCore::recentsManager()->removeRecentFile( file );
        }
    }
}

pAbstractChild* pFileManager::openFile( const QString& fileName, const QString& codec, bool asText )
{
    // if it not exists
    if ( !QFile::exists( fileName ) || !QFileInfo( fileName ).isFile() ) {
        return 0;
    }
    
    // check if file is already opened
    foreach ( pAbstractChild* document, MonkeyCore::workspace()->documents() ) {
        if ( pMonkeyStudio::isSameFile( document->filePath(), fileName ) ) {
            MonkeyCore::workspace()->setCurrentDocument( document );
            return document;
        }
    }
    
    pAbstractChild* document = NULL;
    
    // get a document interface that can handle the file
    if ( !asText ) {
        document = MonkeyCore::pluginsManager()->documentForFileName( fileName );
    }
    
    // open it with pChild (text editor) instance
    if ( !document ) {
        document = new pChild;
    }
    
    // make connection if worksapce don t contains this document
    MonkeyCore::workspace()->handleDocument( document );
    
    // open file
    if ( !document->openFile( fileName, codec ) ) {
        MonkeyCore::messageManager()->appendMessage( tr( "An error occur while opening this file: '%1'" ).arg( QFileInfo( fileName ).fileName() ) );
        MonkeyCore::workspace()->closeDocument( document );
        return 0;
    }
    
    document->showMaximized();
    MonkeyCore::workspace()->setCurrentDocument( document );
    
    // return child instance
    return document;
}

void pFileManager::closeFile( const QString& fileName )
{
    MonkeyCore::workspace()->closeFile( fileName );
}

void pFileManager::goToLine( const QString& fileName, const QPoint& pos, const QString& codec, int selectionLength )
{
    MonkeyCore::workspace()->goToLine( fileName, pos, codec, selectionLength );
}

bool pFileManager::openProject( const QString& fileName, const QString& codec )
{
    return MonkeyCore::projectsManager()->openProject( fileName, codec );
}

bool pFileManager::open( const QString& fileName, const QString& codec )
{
    if ( MonkeyCore::projectTypesIndex()->fileIsAProject( fileName ) ) {
        // recents projects are handled by project manager
        return openProject( fileName, codec );
    }
    else {
        if ( openFile( fileName, codec ) ) {
            // append file to recents
            MonkeyCore::recentsManager()->addRecentFile( fileName );
            return true;
        }
        else {
            // remove it from recents files
            MonkeyCore::recentsManager()->removeRecentFile( fileName );
            return false;
        }
    }
    
    return false;
}
