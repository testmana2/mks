/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.cpp
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#include "pTemplatesManager.h"
#include "pSettings.h"
#include "pMonkeyStudio.h"



#include "ProjectItem.h"
#include "pFileManager.h"

#include <QHash>
#include <QDebug>
#include <QMessageBox>

using namespace pMonkeyStudio;

pTemplatesManager::pTemplatesManager( QObject* o )
	: QObject( o )
{}

void pTemplatesManager::setTemplatesPath( const QStringList& l )
{ pSettings::instance()->setValue( "Templates/DefaultDirectories", l ); }

QStringList pTemplatesManager::templatesPath() const
{
	QString s = "/../templates";
#ifdef Q_OS_MAC
	s.prepend( "/../.." );
#endif
	return pSettings::instance()->value( "Templates/DefaultDirectories", QStringList( QApplication::applicationDirPath().append( s ) ) ).toStringList();
}

void pTemplatesManager::setTemplatesHeader( const QString& l, const QString& s )
{ pSettings::instance()->setValue( QString( "Templates/Header/" ).append( l ), s ); }

QString pTemplatesManager::templatesHeader( const QString& l ) const
{
	QString s = pSettings::instance()->value( QString( "Templates/Header/" ).append( l ), QString() ).toString();
	return s.isEmpty() ? defaultTemplatesHeader( l ) : s;
}

QString pTemplatesManager::defaultTemplatesHeader( const QString& l ) const
{
	// default comment
	QString s( "/****************************************************************************\n"
				"**\n"
				"** 		Created using $editor_version_string$\n"
				"** Author    : $author$\n"
				"** Project   : $name$\n"
				"** FileName  : $filename$\n"
				"** Date      : $date$\n"
				"** License   : $license$\n"
				"** Comment   : $comment$\n"
				"** Home Page : $homepage$\n"
				"**\n"
				"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
				"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
				"**\n"
				"****************************************************************************/\n" );
	
	if ( l == "HTML" )
	{
		s.prepend( "<!--\n" );
		s.append( "-->\n" );
	}
	else if ( l != "C++" )
		s.clear();
	
	// default
	return s;
}

pTemplate pTemplatesManager::getTemplate( const QString& s )
{
	// open ini file
	QSettings set( s, QSettings::IniFormat );
	
	// fill template infos
	pTemplate t;
	t.Name = QFileInfo( s ).dir().dirName();
	t.Language = set.value( "Language", tr( "Other" ) ).toString();
	t.Type = set.value( "Type", tr( "Wrong template type" ) ).toString();
	t.Description = set.value( "Description", tr( "No desctiption" ) ).toString();
	t.Icon = set.value( "Icon" ).toString();
	t.Script = set.value( "Script" ).toString();
	t.DirPath = QFileInfo( s ).absolutePath().append( "/" );
	t.Files = set.value( "Files" ).toStringList();
	t.FilesToOpen = set.value( "FilesToOpen" ).toStringList();
	t.ProjectsToOpen = set.value( "ProjectsToOpen" ).toStringList();
	t.Variables = VarList();
	
	// set template vars
	foreach( QString v, set.value( "Variables" ).toStringList() )
		t.Variables[v] = set.value( v +"Values" ).toStringList();
	
	// return template
    return t;
}

TemplateList pTemplatesManager::getTemplates()
{
	TemplateList l;
	foreach( QString p, templatesPath() )
		foreach ( QFileInfo f, pMonkeyStudio::getFiles( QDir( unTokenizeHome( p ) ), QStringList( "template.ini" ), true ) )
			l << getTemplate( f.absoluteFilePath() );
	return l;
}

bool pTemplatesManager::realiseTemplate( const pTemplate& t, const VariablesManager::Dictionary& d )
{
	// get destination
	QString dest = d["Destination"];
	
	// check destination
	if ( dest.isEmpty() )
	{
		warning( tr( "Error..." ), tr( "Destination not set." ) );
		return false;
	}
	
	// check destination exists
	if ( !QDir( dest ).exists() )
	{
        if ( !QDir().mkpath( dest ) )
        {
            warning( tr( "Error..." ), tr( "Can't create destination '%1'" ).arg( dest ) );
            return false;
        }
	}
	
	// append final slashe
	if ( !dest.endsWith( "/" ) )
		dest.append( '/' );
	
	// get files
	QHash<QString, QString> files;
	foreach( QString f, t.Files )
	{
		// process variables
		QString s = VariablesManager::instance()->replaceAllVariables( f, d );
		
		// check value validity
		if ( s.isEmpty() )
		{
			warning( tr( "Error..." ), tr( "Empty filename detected for file %1" ).arg( f ) );
			return false;
		}
		
		// append file to list
		files[f] = s;
	}
	
	// create files
	foreach( QString f, files.keys() )
	{
		// get file name
		QString s = QString( "%1%2" ).arg( dest, files[f] );
		
		// check file destination exists
		QDir fd( QFileInfo( s ).path() );
		if ( !fd.exists() )
		{
			if ( !fd.mkpath( fd.path() ) )
			{
				warning( tr( "Error..." ), tr( "Can't create destination '%1'" ).arg( fd.path() ) );
				return false;
			}
		}
		
		// copy file
		if ( !QFile::copy( QString( "%1%2" ).arg( t.DirPath, f ), s ) )
		{
			warning( tr( "Error..." ), tr( "Can't copy '%1%2' to '%3'" ).arg( t.DirPath, f, s ) );
			return false;
		}
		
		// open file
		QFile file( s );
		if ( !file.open( QIODevice::ReadWrite | QIODevice::Text ) )
		{
			warning( tr( "Error..." ), tr ( "Can't edit file %1: %2" ).arg( s, file.errorString() ) );
			return false;
		}
		
		// get contents
		QString c = QString::fromLocal8Bit( file.readAll() );
		
		// add header licensing
		c.prepend( templatesHeader( t.Language ) );
		
		// reset file
		file.resize( 0 );
		
		// write process contents
		file.write( VariablesManager::instance()->replaceAllVariables( c, d ).toLocal8Bit() );
		
		// close file
		file.close();
		
		// open files if needed
        if ( t.FilesToOpen.contains( f ) )
            pFileManager::instance()->openFile( s );
        if ( t.ProjectsToOpen.contains( f ) )
            pFileManager::instance()->openProject( s );
	}
	
	// return process state
	return true;
}
