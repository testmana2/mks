#include "MakefileProjectItem.h"

#include <MonkeyCore.h>
#include <PluginsManager.h>
#include <UIMain.h>
#include <pFileManager.h>

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
		setLastError( tr( "Can't open file '%1'" ).arg( fileName() ) );
		return;
	}
	
	const QRegExp targetRex( "^([\\w\\-\\d]+):.*" );
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
		setLastError( tr( "Can't build Makefile projects. GNUMake plugin not enabled" ) );
		return;
	}
	
	pCommand baseCmd = make->command();
	
	baseCmd.setProject( this );
	baseCmd.setSkipOnError( false );
	baseCmd.setTryAllParsers( true );
	
	QString makeFileArg = QString( "-f %1 " ).arg( QFileInfo( fileName() ).fileName() );
	
	foreach( const QString& target, targets ) {
		baseCmd.setText( target );
		baseCmd.setArguments( makeFileArg +target );
		addCommand( baseCmd, "mBuilder" );
	}
}

void MakefileProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
	Q_UNUSED( files );
	Q_UNUSED( scope );
	setLastError( tr( "Adding files to the Makefile project does not make sense. Press 'Edit current project...' for edit your Makefile" ) );
}

void MakefileProjectItem::removeValue( XUPItem* scope )
{
	Q_UNUSED( scope );
	Q_ASSERT( 0 );
}

bool MakefileProjectItem::open( const QString& fileName, const QString& codec )
{
	const QFileInfo fileInfo = QFileInfo( fileName );
	
	if ( !fileInfo.isReadable() ) {
		setLastError( tr( "File '%1' is not readable." ).arg( fileName ) );
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

bool MakefileProjectItem::edit()
{
	MonkeyCore::fileManager()->openFile( fileName(), codec() );
	return false; /* not saved */
}

bool MakefileProjectItem::editProjectFiles()
{
	return false;
}
