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
#include "pTools.h"
#include "pSettings.h"

#include <QDir>
#include <QHash>
#include <QDateTime>

const QList<pTemplate> pTemplatesManager::defaultTemplates()
{
	return QList<pTemplate>()
	// C++
	<< pTemplate( "C++", pTemplate::ttFiles, "Header", "Simple Header", unTokenize( "$TEMPLATE_PATH$/C++/misc.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/header.h" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Source", "Simple Source", unTokenize( "$TEMPLATE_PATH$/C++/misc.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/source.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Main", "Simple Main", unTokenize( "$TEMPLATE_PATH$/C++/console.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/main.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "QObject Header", "Simple QObject Header", unTokenize( "$TEMPLATE_PATH$/C++/qobject.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/qobject_header.h" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "QObject Source", "Simple QObject Source", unTokenize( "$TEMPLATE_PATH$/C++/qobject.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/qobject_source.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "QWidget Header", "Simple QWidget Header", unTokenize( "$TEMPLATE_PATH$/C++/qwidget.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/qwidget_header.h" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "QWidget Source", "Simple QWidget Source", unTokenize( "$TEMPLATE_PATH$/C++/qwidget.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/qwidget_source.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Qt Console Main", "Simple Qt Console Main", unTokenize( "$TEMPLATE_PATH$/C++/qt_console.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/qt_console_main.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Qt GUI Main", "Simple Qt GUI Main", unTokenize( "$TEMPLATE_PATH$/C++/qt_gui.png" ), QStringList( unTokenize( "$TEMPLATE_PATH$/C++/qt_gui_main.cpp" ) ) );
}

const QList<pTemplate> pTemplatesManager::templates()
{
	// get settings
	pSettings* s = pSettings::instance();
	
	QList<pTemplate> mTemplates;

	int size = s->beginReadArray( "Templates" );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		mTemplates << pTemplate( s->value( "Language" ).toString(), (pTemplate::TemplateType)s->value( "Type" ).toInt(), s->value( "Name" ).toString(), s->value( "Description" ).toString(), s->value( "Icon" ).toString(), s->value( "FileNames" ).toStringList() );
	}
	s->endArray();

	// untokenize, need doing it separatly because qsettings is not thread safe
	for ( int i = 0; i < mTemplates.count(); i++ )
	{
		pTemplate& t = mTemplates[i];
		t.Icon = unTokenize( t.Icon );
		t.FileNames = unTokenize( t.FileNames );
	}

	// get default abbreviations if needed
	if ( mTemplates.isEmpty() )
		mTemplates << defaultTemplates();

	// return list
	return mTemplates;
}

void pTemplatesManager::setTemplatesPath( const QString& s )
{
	pSettings::instance()->setValue( "Templates/DefaultDirectory", pTools::tokenizeHome( s ) );
}

const QString pTemplatesManager::templatesPath()
{
	return pTools::unTokenizeHome( pSettings::instance()->value( "Templates/DefaultDirectory", "$HOME$/.Monkey Studio/Templates" ).toString() );
}

void pTemplatesManager::setTemplatesHeader( const QString& s )
{
	pSettings::instance()->setValue( "Templates/Header", s );
}

const QString pTemplatesManager::templatesHeader()
{
	QString s = pSettings::instance()->value( "Templates/Header", QString() ).toString();

	if ( s.isEmpty() )
		s = "/****************************************************************************\n"
			"**\n"
			"** 		Created using $editor_version_string$\n"
			"** Author    : $author$\n"
			"** Project   : $name$\n"
			"** FileName  : $filename$\n"
			"** Date      : $date$\n"
			"** License   : $license$\n"
			"** Comment   : $comment$\n"
			"**\n"
			"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
			"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
			"**\n"
			"****************************************************************************/\n";

	return s;
}

const QString pTemplatesManager::tokenize( QString s )
{
	return s.replace( templatesPath(), "$TEMPLATE_PATH$" );
}

const QString pTemplatesManager::unTokenize( QString s )
{
	return s.replace( "$TEMPLATE_PATH$", templatesPath() );
}

const QStringList pTemplatesManager::tokenize( QStringList l )
{
	for ( int i = 0; i < l.count(); i++ )
		l[i] = tokenize( l.at( i ) );
	return l;
}

const QStringList pTemplatesManager::unTokenize( QStringList l )
{
	for ( int i = 0; i < l.count(); i++ )
		l[i] = unTokenize( l.at( i ) );
	return l;
}

QString processFunction( const QString& v, const QString& f )
{
	// get lower function name
	QString s = f.toLower();

	// interpret
	if ( s == "upper" )
		return v.toUpper();
	else if ( s == "lower" )
		return v.toLower();
	else if ( s == "trimmed" )
		return v.trimmed();
	else if ( s == "simplified" )
		return v.simplified();

	// default value
	return v;
}

const QString pTemplatesManager::processContent( pTemplateContent tc )
{
	QHash<QString, QString> values;
	int p = 0;
	QString s;
	QRegExp r( "(\\$[^$\\s]+\\$)" );

	// search and interpret values
	while ( ( p = r.indexIn( tc.Content, p ) ) != -1 )
	{
		// got keyword
		s = r.capturedTexts().value( 1 ).toLower();

		// process value if needed
		if ( !values.contains( s ) )
		{
			// slip to check function needed
			QStringList l = QString( s ).replace( "$", "" ).split( "." );

			// set value
			QString v = l.at( 0 );

			// monkeystudio_version
			if ( v == "editor_version" )
				l[0] = PROGRAM_VERSION;
			// monkeystudio_version_string
			else if ( v == "editor_version_string" )
				l[0] = QString( "%1 v%2" ).arg( PROGRAM_NAME ).arg( PROGRAM_VERSION );
			// filename
			else if ( v == "filename" )
				l[0] = tc.FileName;
			// basename
			else if ( v == "basename" )
				l[0] = QFileInfo( tc.FileName ).baseName();
			// name
			else if ( v == "name" )
				l[0] = tc.Name;
			// date
			else if ( v == "date" )
				l[0] = QDateTime::currentDateTime().toString();
			// license
			else if ( v == "license" )
				l[0] = tc.License;
			// comment
			else if ( v == "comment" )
				l[0] = tc.Comment;
			// author
			else if ( v == "author" )
				l[0] = tc.Author;
			// project
			else if ( v == "project" )
				l[0] = tc.Project;
			else
				l[0] = "";

			// process function
			if ( l.count() > 1 )
				for ( int i = 0; i < l.count(); i++ )
					l[0] = processFunction( l.at( 0 ), l.at( i ) );

			// save value
			values[s] = l.at( 0 );
		}

		// next occurence
		p += r.matchedLength();
	}
	
	// replace occurences
	foreach ( QString k, values.keys() )
		tc.Content.replace( k, values[k], Qt::CaseInsensitive );

	// return value
	return tc.Content;
}
