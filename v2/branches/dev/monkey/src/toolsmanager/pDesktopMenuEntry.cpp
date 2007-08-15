/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : pDesktopMenuEntry
** FileName  : pDesktopMenuEntry.cpp
** Date      : dim. août 12 16:15:47 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pDesktopMenuEntry.h"

#include <QFileInfo>
#include <QFile>
#include <QSettings>

#include <QDebug>

const QStringList mIconExtensions = QStringList() << "png" << "PNG" << "svg" << "SVG" << "xpm" << "XPM";

QStringList getFiles( QDir d, const QString& s )
{
	QStringList l;
	foreach ( QFileInfo f, d.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot ) )
	{
		if ( f.isFile() && f.suffix() == s )
			l << f.absoluteFilePath();
		else if ( f.isDir() )
		{
			d.cd( f.filePath() );
			l << getFiles( d, s );
			d.cdUp();
		}
	}
	return l;
}

pDesktopMenuEntry::pDesktopMenuEntry( QObject* o )
	: QObject( o )
{
	hashThemes();
	hashDirectories();
	hashApplications();
}

pDesktopMenuEntry::~pDesktopMenuEntry()
{
}

QStringList pDesktopMenuEntry::themesPath() const
{
	QString s;
	QStringList l;
	// for backwards compatibility
	s = QString( "/.icons" ).prepend( qgetenv( "HOME" ) );
	if ( QFile::exists( s ) )
		l << s;
	// freedesktop recommendation
	s = QString( "/icons" ).prepend( qgetenv( "XDG_DATA_DIRS" ) );
	if ( QFile::exists( s ) )
		l << s;
	// freedesktop recommendation
	s = "/usr/share/pixmaps";
	if ( QFile::exists( s ) )
		l << s;
	// own add for fucking system
	s = "/usr/share/icons";
	if ( !l.contains( s ) && QFile::exists( s ) )
		l << s;
	// return
	return l;
}

QString pDesktopMenuEntry::directoriesPath() const
{
	// TODO: system check
	return "/usr/share/desktop-directories";
}

QString pDesktopMenuEntry::applicationsPath() const
{
	// TODO: system check
	return "/usr/share/applications";
}

void pDesktopMenuEntry::hashThemes()
{
	foreach ( QString tp, themesPath())
	{
		foreach ( QFileInfo f, QDir( tp ).entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot ) )
		{
			// continue if fodler don't contains index.theme
			const QString fn = f.absoluteFilePath().append( "/index.theme" );
			if ( !QFile::exists( fn ) )
				continue;
			// read file
			pThemeFile t;
			QSettings s( fn, QSettings::IniFormat );
			// theme
			s.beginGroup( "Icon Theme" );
			if ( mThemes.contains( s.value( "Name" ).toString() ) )
			{
				t = mThemes[ s.value( "Name" ).toString() ];
				// update
				t.Path << f.absoluteFilePath();
				foreach ( QString s, s.value( "Inherits" ).toStringList() )
					if ( !t.Inherits.contains( s ) )
						t.Inherits << s;
			}
			else
			{
				t.Path << f.absoluteFilePath();
				t.Comment = s.value( "Comment" ).toString();
				t.Hidden = s.value( "Hidden" ).toBool();
				t.Inherits = s.value( "Inherits" ).toStringList();
			}
			// add folders
			foreach ( QString d, s.value( "Directories" ).toStringList() )
			{
				pThemeFolder tf;
				// drop if already contains this folder
				if ( t.Folders.contains( d ) )
				{
					tf = t.Folders[ d ];
					tf.Path << f.absoluteFilePath().append( "/" ).append( d );
				}
				else
				{
					s.beginGroup( d );
					tf.Path << f.absoluteFilePath().append( "/" ).append( d );
					tf.Size = s.value( "Size" ).toInt();
					tf.Context = s.value( "Context" ).toString();
					tf.Type = s.value( "Type" ).toString();
					tf.MinSize = s.value( "MinSize", s.value( "Size" ) ).toInt();
					tf.MaxSize = s.value( "MaxSize", s.value( "Size" ) ).toInt();
					tf.Threshold = s.value( "Threshold", 2 ).toInt();
					s.endGroup();
				}
				// add to folders list
				t.Folders[ d ] = tf;
			}
			// add theme to hash
			mThemes[ s.value( "Name" ).toString() ] = t;
			s.endGroup();
		}
	}
	// check datas
	/*
	foreach( pThemeFile t, mThemes )
	{
		qWarning( "Theme: %s", qPrintable( t.Name ) );
		foreach( pThemeFolder tf, t.Folders )
			qWarning( "Folder: %s", qPrintable( tf.Path ) );
		qWarning( "Count: %d", t.Folders.count() );
	}
	*/
}

void pDesktopMenuEntry::hashDirectories()
{
	foreach ( QString f, getFiles( QDir( directoriesPath() ), "directory" ) )
	{
		// read file
		QSettings s( f, QSettings::IniFormat );
		// directory
		s.beginGroup( "Desktop Entry" );
		pDesktopFile fp;
		fp.FilePath = f;
		fp.Encoding = s.value( "Encoding" ).toString();
		fp.Name = s.value( "Name" ).toString();
		fp.GenericName = s.value( "GenericName" ).toString();
		fp.Icon = s.value( "Icon" ).toString();
		fp.Type = s.value( "Type" ).toString();
		fp.Categories = s.value( "Categories" ).toString().split( ";" );
		fp.Comment = s.value( "Comment" ).toString();
		s.endGroup();
		// add directory to hash
		mDirectories[QFileInfo( f ).fileName()] = fp;
	}
	// check datas
	/*
	foreach( DesktopFile f, mDirectories )
		qWarning( "Directory: %s, icon: %s", qPrintable( f.Name ), qPrintable( f.Icon ) );
	*/
}

void pDesktopMenuEntry::hashApplications()
{
	foreach ( QString f, getFiles( QDir( applicationsPath() ), "desktop" ) )
	{
		// read file
		QSettings s( f, QSettings::IniFormat );
		// directory
		s.beginGroup( "Desktop Entry" );
		pDesktopFile fp;
		fp.FilePath = f;
		fp.Encoding = s.value( "Encoding" ).toString();
		fp.Name = s.value( "Name" ).toString();
		fp.GenericName = s.value( "GenericName" ).toString();
		fp.Icon = s.value( "Icon" ).toString();
		fp.Type = s.value( "Type" ).toString();
		fp.Categories = s.value( "Categories" ).toString().split( ";" );
		fp.Comment = s.value( "Comment" ).toString();
		s.endGroup();
		// add directory to hash
		mApplications[QFileInfo( f ).fileName()] = fp;
	}
	// check datas
	/*
	foreach( DesktopFile f, mApplications )
		qWarning( "Application: %s, icon: %s", qPrintable( f.Name ), qPrintable( f.Icon ) );
	*/
}

bool pDesktopMenuEntry::DirectorySizeDistance( const pThemeFolder& t, int s )
{
	// Fixed
	if ( t.Type.toLower() == "fixed" )
		return abs( t.Size -s );
	// Scaled
	if ( t.Type.toLower() == "scaled" )
	{
		if ( s < t.MinSize )
			return t.MinSize -s;
		if ( s > t.MaxSize )
			return s -t.MaxSize;
		return 0;
	}
	// Threshold
  if ( t.Type.toLower() == "threshold" )
  {
		if ( s < t.Size -t.Threshold )
			return t.MinSize -s;
		if ( s > t.Size +t.Threshold )
			return s -t.MaxSize;
		return 0;
	}
	// None
	return 0;
}

bool pDesktopMenuEntry::DirectoryMatchesSize( const pThemeFolder& t, int s )
{
	// Fixed
  if ( t.Type.toLower() == "fixed" )
		return t.Size == s;
  // Scaled
  if ( t.Type.toLower() == "scaled" )
		return t.MinSize <= s <= t.MaxSize;
  // Threshold
  if ( t.Type.toLower() == "threshold" )
		return t.Size -t.Threshold <= s <= t.Size +t.Threshold;
  // None
  return 0;
}

QString pDesktopMenuEntry::LookupIcon( const QString& i, int s, const QString& t )
{
	// Fixed, Threshold
	QString f;
	foreach ( pThemeFolder tf, mThemes.value( t ).Folders )
	{
		foreach ( QString d, tf.Path )
		{
			foreach ( QString e, mIconExtensions )
			{
				if ( DirectoryMatchesSize( tf, s ) )
				{
					f = QString( "%1/%2.%3" ).arg( d, i, e );
					if ( QFile::exists( f ) )
						return f;
				}
			}
		}
	}
	// Scalable
	QString cf;
	int ms = INT_MAX;
	foreach ( pThemeFolder tf, mThemes.value( t ).Folders )
	{
		foreach ( QString d, tf.Path )
		{
			foreach ( QString e, mIconExtensions )
			{
				f = QString( "%1/%2.%3" ).arg( d, i, e );
				if ( QFile::exists( f ) && DirectorySizeDistance( tf, s ) < ms )
				{
					cf = f;
					ms = DirectorySizeDistance( tf, s );
				}
			}
		}
	}
	// return
	return cf;
}

QString pDesktopMenuEntry::LookupFallbackIcon( const QString& i )
{
	foreach ( QString d, themesPath() )
	{
		foreach ( QString e, mIconExtensions )
		{
			d.append( QString( "/%1.%2" ).arg( i, e ) );
			if ( QFile::exists( d ) )
				return d;
		}
	}
	return QString();
}

QString pDesktopMenuEntry::FindIconHelper( const QString& i, int s, const QString& t )
{
	QStringList l;
  QString f = LookupIcon( i, s, t );
  if ( !f.isNull() )
    return f;
  if ( !mThemes.value( t ).Inherits.isEmpty() )
    l << mThemes.value( t ).Inherits;
  else if ( t.toLower() != "hicolor" )
    l << "hicolor";
	foreach ( QString p, l )
	{
    f = FindIconHelper( i, s, p );
    if ( !f.isEmpty() )
      return f;
  }
  return QString();
}

QString pDesktopMenuEntry::FindIcon( const QString& i, int s, const QString& t )
{
  QString f = FindIconHelper( i, s, t );
  if ( !f.isEmpty() )
    return f;
  return LookupFallbackIcon( i );
}
