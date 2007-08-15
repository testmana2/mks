/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : pDesktopMenuEntry
** FileName  : pDesktopMenuEntry.h
** Date      : dim. août 12 16:15:47 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PDESKTOPMENUENTRY_H
#define PDESKTOPMENUENTRY_H

#include "MonkeyExport.h"

#include <QObject>
#include <QHash>
#include <QDir>

struct Q_MONKEY_EXPORT pDesktopFile
{
	QString FilePath;
	QString Encoding;
	QString Name;
	QString GenericName; // Only for Application
	QString Icon;
	QString Type; // Application, Directory
	QStringList Categories;
	QString Comment;
};

struct Q_MONKEY_EXPORT pThemeFolder
{
	pThemeFolder()	{}
	/*
	ThemeFolder( const QStringList& p, int s, const QString& c, const QString& t, int mn, int mx, int th )
		: Path( p ), Size( s ), Context( c ), Type( t ), MinSize( mn ), MaxSize( mx ), Threshold( th ) {}
	*/
		
	QStringList Path; // first entry defined the infos
	int Size;
	QString Context; // Applications, Actions, Animations, Categories, Devices, Emblems, Emotes, FileSystems, International, MimeTypes, Places, Status, Stock, 
	QString Type; // Threshold, Fixed, Scalable
	int MinSize; // Only Scalable
	int MaxSize; // Only Scalable
	int Threshold; // Only Threshold
};

struct Q_MONKEY_EXPORT pThemeFile
{
	pThemeFile() {}
	/*
	ThemeFile( const QStringList& p, const QString& c, bool h, const QStringList& i )
		: Path( p ), Comment( c ), Hidden( h ), Inherits( i ) {}
	*/
		
	QStringList Path; // first entry defined the infos
	QString Comment;
	bool Hidden;
	QStringList Inherits;
	QHash<QString, pThemeFolder> Folders;
};

class Q_MONKEY_EXPORT pDesktopMenuEntry : public QObject
{
	Q_OBJECT

public:
	pDesktopMenuEntry( QObject* = 0 );
	~pDesktopMenuEntry();

	QHash<QString, pThemeFile> themes() const { return mThemes; }
	QHash<QString, pDesktopFile> directories() const { return mDirectories; }
	QHash<QString, pDesktopFile> applications() const { return mApplications; }
	
	QString FindIcon( const QString& icon, int size, const QString& theme );

protected:
	QHash<QString, pThemeFile> mThemes;
	QHash<QString, pDesktopFile> mDirectories;
	QHash<QString, pDesktopFile> mApplications;

	QStringList themesPath() const;
	QString directoriesPath() const;
	QString applicationsPath() const;

	void hashThemes();
	void hashDirectories();
	void hashApplications();
	
	bool DirectorySizeDistance( const pThemeFolder&, int iconsize );
	bool DirectoryMatchesSize( const pThemeFolder&, int iconsize );
	QString LookupIcon( const QString& iconname, int size, const QString& theme );
	QString LookupFallbackIcon( const QString& iconname );
	QString FindIconHelper( const QString& icon, int size, const QString& theme );

};

#endif // PDESKTOPMENUENTRY_H
