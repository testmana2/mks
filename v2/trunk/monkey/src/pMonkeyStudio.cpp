/*****************************************************
* AUTHOR   : Nox P@sNox <pasnox@gmail.com>
* PROJECT  : 
* FILENAME : pMonkeyStudio.cpp
* DATE     : 2007/07/29
* TIME     : 13:13
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "PluginsManager.h"
#include "ProjectPlugin.h"

#include "pWorkspace.h"
#include "pAbstractChild.h"
#include "pEditor.h"

#include <QTextCodec>
#include <QImageReader>
#include <QFileDialog>
#include <QFileInfo>
#include <QHash>

#include "qsciapis.h"
#include "qscilexerbash.h"
#include "qscilexerbatch.h"
#include "qscilexercmake.h"
#include "qscilexercpp.h"
#include "qscilexercsharp.h"
#include "qscilexercss.h"
#include "qscilexerd.h"
#include "qscilexerdiff.h"
#include "qscilexerhtml.h"
#include "qscilexeridl.h"
#include "qscilexerjava.h"
#include "qscilexerjavascript.h"
#include "qscilexerlua.h"
#include "qscilexermakefile.h"
#include "qscilexerperl.h"
#include "qscilexerpov.h"
#include "qscilexerproperties.h"
#include "qscilexerpython.h"
#include "qscilexerruby.h"
#include "qscilexersql.h"
#include "qscilexertex.h"
#include "qscilexervhdl.h"

QHash<QString,QsciLexer*> mGlobalsLexers;
QHash<QString,QsciAPIs*> mGlobalsAPIs;

void pMonkeyStudio::warning( const QString& c, const QString& s, QWidget* w )
{
	QMessageBox::warning( w, c, s );
}

void pMonkeyStudio::information( const QString& c, const QString& s, QWidget* w )
{
	QMessageBox::information( w, c, s );
}

bool pMonkeyStudio::question( const QString& c, const QString& s, QWidget* w, QMessageBox::StandardButtons b, QMessageBox::StandardButton d )
{
	if ( QMessageBox::question( w, c, s, b, d ) == QMessageBox::Yes )
		return true;
	return false;
}

bool pMonkeyStudio::isSameFile( const QString& f, const QString& o )
{
	// get file info
	QFileInfo fif( f );
	QFileInfo fio( o );
	
	// check files exists
	if ( fif.exists() != fio.exists() )
		return false;
	
	// check simlink
	if ( fif.isSymLink() )
		fif.setFile( fif.symLinkTarget() );
	if ( fio.isSymLink() )
		fio.setFile( fio.symLinkTarget() );
	
	// check canonical file path
	return fif.canonicalFilePath() == fio.canonicalFilePath();
}

const QStringList pMonkeyStudio::availableTextCodecs()
{
	static QStringList l;
	if ( l.isEmpty() )
	{
		foreach ( QByteArray a, QTextCodec::availableCodecs() )
			l << a;
		l.sort();
	}
	return l;
}

const QStringList pMonkeyStudio::availableImageFormats()
{
	static QStringList l;
	if ( l.isEmpty() )
	{
		foreach ( QByteArray a, QImageReader::supportedImageFormats() )
			l << a;
		l.sort();
	}
	return l;
}

const QStringList pMonkeyStudio::availableLanguages()
{
	static QStringList l = QStringList() << "Bash" << "Batch" << "C#" << "C++" << "CMake" << "CSS"
		<< "D" << "Diff" << "HTML" << "IDL" << "Java" << "JavaScript" << "Lua"
		<< "Makefile" << "POV" << "Perl" << "Properties" << "Python" << "Ruby"
		<< "SQL" << "TeX" << "VHDL";
	return l;
}

QStringList pMonkeyStudio::defaultOperators()
{ return QStringList() << "=" << "-=" << "+=" << "*=" << "~=" << ":" << "|"; }

const QStringList pMonkeyStudio::availableOperators()
{
	QStringList l = pSettings::instance()->value( settingsPath() +"/Operators" ).toStringList();
	return l.isEmpty() ? defaultOperators() : l;
}

void pMonkeyStudio::setAvailableOperators( const QStringList& l )
{ pSettings::instance()->setValue( settingsPath() +"/Operators", l ); }

const QFileInfoList pMonkeyStudio::getFiles( QDir d, const QStringList& l, bool b )
{
	QFileInfoList ll;
	foreach ( QFileInfo f, d.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
	{
		if ( f.isFile() && ( l.isEmpty() || ( !l.isEmpty() && l.contains( f.suffix(), Qt::CaseInsensitive ) ) ) )
			ll << f;
		else if ( f.isDir() && b )
		{
			d.cd( f.filePath() );
			ll << getFiles( d, l );
			d.cdUp();
		}
	}
	return ll;
}

const QFileInfoList pMonkeyStudio::getFiles( QDir d, const QString& s, bool b )
{ return getFiles( d, s.isEmpty() ? QStringList() : QStringList( s ), b ); }

QFileDialog* getOpenDialog( QFileDialog::FileMode fm, const QString& c, const QString& fn, const QString& f, QWidget* w, QFileDialog::AcceptMode m =  QFileDialog::AcceptOpen )
{
	// create dialg
	QFileDialog* d = new QFileDialog( w, c, fn, f );
	// set file accept mode
	d->setAcceptMode( m );
	// set file mode
	d->setFileMode( fm );

	// set filename for compatibility with qt 4.2.x
	/*
	QFileInfo fi( f );
	d.setDirectory( fi.path() );
	d.selectFile( fi.fileName() );
	*/
	// return dialog
	return d;
}

const QStringList pMonkeyStudio::getImageFileNames( const QString& c, const QString& f, QWidget* w )
{
	// get image filters
	QStringList l;
	foreach ( QString s, availableImageFormats() )
		l << QObject::tr( "%1 Files (*.%2)" ).arg( s.toUpper() ).arg( s );
	// add all format as one filter at begining
	if ( !l.isEmpty() )
		l.prepend( QObject::tr( "All Image Files (%1)" ).arg( QStringList( availableImageFormats() ).replaceInStrings( QRegExp( "^(.*)$" ), "*.\\1" ).join( " " ) ) );
	// create dialog
	QFileDialog* d = getOpenDialog( QFileDialog::ExistingFiles, c.isEmpty() ? QObject::tr( "Select image(s)" ) : c, f, l.join( ";;" ), w );
	// choose last used filter if available
	if ( !l.isEmpty() )
		d->selectFilter( pSettings::instance()->value( "Recents/ImageFilter" ).toString() );
	// execute dialog
	if ( d->exec() )
	{
		// remember last filter if available
		if ( !l.isEmpty() )
			pSettings::instance()->setValue( "Recents/ImageFilter", d->selectedFilter() );
		// remember selected files
		l = d->selectedFiles();
		// delete dialog
		delete d;
		// return selected files
		return l;
	}
	// delete dialog
	delete d;
	// return empty list
	return QStringList();
}

const QString pMonkeyStudio::getImageFileName( const QString& c, const QString& f, QWidget* w )
{
	return getImageFileNames( c, f, w ).value( 0 );
}

const QStringList pMonkeyStudio::getOpenFileNames( const QString& c, const QString& fn, const QString& f, QWidget* w )
{
	// create dialg
	QFileDialog* d = getOpenDialog( QFileDialog::ExistingFiles, c.isEmpty() ? QObject::tr( "Select file(s)" ) : c, fn, f, w );
	// choose last used filter if available
	if ( !f.isEmpty() )
		d->selectFilter( pSettings::instance()->value( "Recents/FileFilter" ).toString() );
	// execute dialog
	if ( d->exec() )
	{
		// remember last filter if available
		if ( !f.isEmpty() )
			pSettings::instance()->setValue( "Recents/FileFilter", d->selectedFilter() );
		// remember selected files
		QStringList l = d->selectedFiles();
		// delete dialog
		delete d;
		// return selected files
		return l;
	}
	// delete dialog
	delete d;
	// return empty list
	return QStringList();
}

const QString pMonkeyStudio::getOpenFileName( const QString& c, const QString& fn, const QString& f, QWidget* w )
{
	// create dialg
	QFileDialog* d = getOpenDialog( QFileDialog::ExistingFile, c.isEmpty() ? QObject::tr( "Select file" ) : c, fn, f, w );
	// choose last used filter if available
	if ( !f.isEmpty() )
		d->selectFilter( pSettings::instance()->value( "Recents/FileFilter" ).toString() );
	// execute dialog
	if ( d->exec() )
	{
		// remember last filter if available
		if ( !f.isEmpty() )
			pSettings::instance()->setValue( "Recents/FileFilter", d->selectedFilter() );
		// remember selected files
		QStringList l = d->selectedFiles();
		// delete dialog
		delete d;
		// return selected files
		return l.value( 0 );
	}
	// delete dialog
	delete d;
	// return empty list
	return QString();
}

const QString pMonkeyStudio::getSaveFileName( const QString& c, const QString& fn, const QString& f, QWidget* w )
{
	// create dialg
	QFileDialog* d = getOpenDialog( QFileDialog::AnyFile, c.isEmpty() ? QObject::tr( "Choose a filename" ) : c, fn, f, w, QFileDialog::AcceptSave );
	// choose last used filter if available
	if ( !f.isEmpty() )
		d->selectFilter( pSettings::instance()->value( "Recents/FileFilter" ).toString() );
	// execute dialog
	if ( d->exec() )
	{
		// remember last filter if available
		if ( !f.isEmpty() )
			pSettings::instance()->setValue( "Recents/FileFilter", d->selectedFilter() );
		// remember selected files
		QStringList l = d->selectedFiles();
		// delete dialog
		delete d;
		// return selected files
		return l.value( 0 );
	}
	// delete dialog
	delete d;
	// return empty list
	return QString();
}

const QString pMonkeyStudio::getExistingDirectory( const QString& c, const QString& f, QWidget* w )
{
	return QFileDialog::getExistingDirectory( w, c.isEmpty() ? QObject::tr( "Select a folder" ) : c, f );
}

const QString pMonkeyStudio::tokenizeHome( const QString& s )
{
	return QString( s ).replace( QDir::homePath(), "$HOME$" ); 
}

const QString pMonkeyStudio::unTokenizeHome( const QString& s )
{
	return QString( s ).replace( "$HOME$", QDir::homePath() );
}

const QHash<QString, QStringList> pMonkeyStudio::defaultSuffixes()
{
	// suffixes list
	QHash<QString, QStringList> l;
	// Bash
	l["Bash"] << "*.sh";
	// Batch
	l["Batch"] << "*.bat";
	l["Batch"] << "*.cmd";
	// C#
	l["C#"] << "*.cs";
	// C++
	l["C++"] << "*.c";
	l["C++"] << "*.cc";
	l["C++"] << "*.cpp";
	l["C++"] << "*.cxx";
	l["C++"] << "*.c++";
	l["C++"] << "*.h";
	l["C++"] << "*.hh";
	l["C++"] << "*.hpp";
	l["C++"] << "*.hxx";
	l["C++"] << "*.h++";
	// CMake
	l["CMake"] << "*.cmake";
	l["CMake"] << "CMake.txt";
	// CSS
	l["CSS"] << "*.css";
	// D
	l["D"] << "*.d";
	// Diff
	l["Diff"] << "*.diff";
	l["Diff"] << "*.patch";
	// HTML
	l["HTML"] << "*.asp";
	l["HTML"] << "*.xml";
	l["HTML"] << "*.xsd";
	l["HTML"] << "*.xsl";
	l["HTML"] << "*.xslt";
	l["HTML"] << "*.docbook";
	l["HTML"] << "*.dtd";
	l["HTML"] << "*.htm";
	l["HTML"] << "*.html";
	l["HTML"] << "*.php";
	l["HTML"] << "*.php3";
	l["HTML"] << "*.php4";
	l["HTML"] << "*.php5";
	l["HTML"] << "*.phtml";
	l["HTML"] << "*.rdf";
	l["HTML"] << "*.svg";
	l["HTML"] << "*.shtml";
	// IDL
	l["IDL"] << "*.idl";
	// Java
	l["Java"] << "*.java";
	// JavaScript
	l["JavaScript"] << "*.js";
	// Lua
	l["Lua"] << "*.lua";
	// Makefile
	l["Makefile"] << "*.mak";
	l["Makefile"] << "*makefile";
	l["Makefile"] << "Makefile*";
	// POV
	l["POV"] << "*.pov";
	// Perl
	l["Perl"] << "*.ph";
	l["Perl"] << "*.pl";
	l["Perl"] << "*.pm";
	// Properties
	l["Properties"] << "*.cfg";
	l["Properties"] << "*.cnf";
	l["Properties"] << "*.inf";
	l["Properties"] << "*.ini";
	l["Properties"] << "*.properties";
	l["Properties"] << "*.rc";
	l["Properties"] << "*.reg";
	// Python
	l["Python"] << "*.ptl";
	l["Python"] << "*.py";
	l["Python"] << "*.pyw";
	l["Python"] << "*.pyx";
	// Ruby
	l["Ruby"] << "*.rb";
	l["Ruby"] << "*.rbw";
	// SQL
	l["SQL"] << "*.sql";
	// TeX
	l["TeX"] << "*.aux";
	l["TeX"] << "*.idx";
	l["TeX"] << "*.sty";
	l["TeX"] << "*.toc";
	// VHDL
	l["VHDL"] << "*.vhdl";
	// return list
	return l;
}

const QHash<QString, QStringList> pMonkeyStudio::availableSuffixes()
{
	// suffixes list
	QHash<QString, QStringList> l;
	// get settings
	pSettings* s = pSettings::instance();
	// get associations from settings
	s->beginGroup( "LexersAssociations" );
	foreach ( QString k, s->childKeys() )
		l[s->value( k ).toString()] << k;
	s->endGroup();
	// if empty get default suffixes
	if ( l.isEmpty() )
		l = defaultSuffixes();
	// return list
	return l;
}

const QHash<QString, QStringList> pMonkeyStudio::availableProjectsSuffixes()
{
	// temporary hash
	QHash<QString, QStringList> mSuffixes;
	// get all hash
	foreach ( BasePlugin* bp, PluginsManager::instance()->plugins() )
		if ( bp->isEnabled() && bp->infos().Type == BasePlugin::iProject )
			mSuffixes.unite( qobject_cast<ProjectPlugin*>( bp )->suffixes() );
	// return suffixes
	return mSuffixes;
}

const QString pMonkeyStudio::availableLanguagesFilters()
{
	QString f;
	// get suffixes
	QHash<QString, QStringList> sl = availableSuffixes();
	//
	foreach ( QString k, sl.keys() )
		f += QString( "%1 Files ( %2 );;" ).arg( k ).arg( sl.value( k ).join( " " ) );
	// remove trailing ;;
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	// return filters list
	return f;
}

const QString pMonkeyStudio::availableProjectsFilters()
{
	QString f;
	// get suffixes
	QHash<QString, QStringList> sl = availableProjectsSuffixes();
	//
	foreach ( QString k, sl.keys() )
		f += QString( "%1 ( %2 );;" ).arg( k ).arg( sl.value( k ).join( " " ) );
	// remove trailing ;;
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	// return filters list
	return f;
}

const QString pMonkeyStudio::settingsPath()
{
	return "/Settings";
}

const QString pMonkeyStudio::scintillaSettingsPath()
{
	return "/Scintilla";
}

void pMonkeyStudio::prepareAPIs()
{
	// check lexers & apis
	if ( mGlobalsLexers.isEmpty() || mGlobalsAPIs.isEmpty() )
	{
		// free hashes
		qDeleteAll( mGlobalsLexers );
		mGlobalsLexers.clear();
		qDeleteAll( mGlobalsAPIs );
		mGlobalsAPIs.clear();
	}
	//
	foreach ( QString ln, availableLanguages() )
	{
		QsciLexer* l = lexerForLanguage( ln );
		QsciAPIs* a = apisForLexer( l );
		// clear raw api
		a->clear();
		// load prepared files
		foreach ( QString f, pSettings::instance()->value( QString( "SourceAPIs/" ).append( ln ) ).toStringList() )
		{
			if ( !a->load( f ) )
				warning( QObject::tr( "Loaging Api File..." ), QObject::tr( "An error occured when loading api file:\n%1" ).arg( f ) );
		}
		// start prepare for apis
		a->prepare();
	}
}

QsciAPIs* pMonkeyStudio::apisForLexer( QsciLexer* l )
{
	// cancel if no lexer
	if ( !l )
		return 0;
	// check if apis already exists
	if ( !mGlobalsAPIs.contains( l->language() ) )
	{
		// create apis
		QsciAPIs* a = new QsciAPIs( l );
		// store global apis
		mGlobalsAPIs[l->language()] = a;
	}
	// return apis
	return mGlobalsAPIs.value( l->language() );
}

QsciLexer* pMonkeyStudio::lexerForFilename( const QString& s ) // FIXME use QDir::match
{
	// get suffixes
	QHash<QString, QStringList> l = availableSuffixes();
	// file suffixe
	const QString sf = QFileInfo( s ).suffix().prepend( "*." );
	const QString f = QFileInfo( s ).fileName();
	bool b = false;
	// checking suffix
	foreach ( QString k, l.keys() )
	{
		// check normal extension: ie *.ext
		b = l.value( k ).contains( sf, Qt::CaseInsensitive );
		// if not found try regexp expression: ie *makefile or Makefile*
		if ( !b )
			foreach ( QString sx, l.value( k ) )
				if ( ( b = QRegExp( sx, Qt::CaseInsensitive, QRegExp::Wildcard ).exactMatch( f ) ) )
					break;
		// if suffixe match, return lexer
		if ( b )
			return lexerForLanguage( k );
	}
	// return no lexer if not found
	return 0;
}

QsciLexer* pMonkeyStudio::lexerForLanguage( const QString& s )
{
	if ( mGlobalsLexers.keys().contains( s ) )
		return mGlobalsLexers.value( s );
	// get language
	const QString ln = s.toLower();
	// lexer
	QsciLexer* l = 0;
	// init lexer
	if ( ln == "bash" )
		l = new QsciLexerBash( QApplication::instance() );
	else if ( ln == "batch" )
		l = new QsciLexerBatch( QApplication::instance() );
	else if ( ln == "c#" )
		l = new QsciLexerCSharp( QApplication::instance() );
	else if ( ln == "c++" )
		l = new QsciLexerCPP( QApplication::instance() );
	else if ( ln == "cmake" )
		l = new QsciLexerCMake( QApplication::instance() );
	else if ( ln == "css" )
		l = new QsciLexerCSS( QApplication::instance() );
	else if ( ln == "d" )
		l = new QsciLexerD( QApplication::instance() );
	else if ( ln == "diff" )
		l = new QsciLexerDiff( QApplication::instance() );
	else if ( ln == "html" )
		l = new QsciLexerHTML( QApplication::instance() );
	else if ( ln == "idl" )
		l = new QsciLexerIDL( QApplication::instance() );
	else if ( ln == "java" )
		l = new QsciLexerJava( QApplication::instance() );
	else if ( ln == "javascript" )
		l = new QsciLexerJavaScript( QApplication::instance() );
	else if ( ln == "lua" )
		l = new QsciLexerLua( QApplication::instance() );
	else if ( ln == "makefile" )
		l = new QsciLexerMakefile( QApplication::instance() );
	else if ( ln == "pov" )
		l = new QsciLexerPOV( QApplication::instance() );
	else if ( ln == "perl" )
		l = new QsciLexerPerl( QApplication::instance() );
	else if ( ln == "properties" )
		l = new QsciLexerProperties( QApplication::instance() );
	else if ( ln == "python" )
		l = new QsciLexerPython( QApplication::instance() );
	else if ( ln == "ruby" )
		l = new QsciLexerRuby( QApplication::instance() );
	else if ( ln == "sql" )
		l = new QsciLexerSQL( QApplication::instance() );
	else if ( ln == "tex" )
		l = new QsciLexerTeX( QApplication::instance() );
	else if ( ln == "vhdl" )
		l = new QsciLexerVHDL( QApplication::instance() );
	// init lexer settings
	if ( l )
	{
		// add lexer to global lexers hash
		mGlobalsLexers[l->language()] = l;
		// read settings
		l->readSettings( *pSettings::instance(), qPrintable( scintillaSettingsPath() ) );
		// set apis
		l->setAPIs( apisForLexer( l ) );
	}
	// return lexer
	return l;
}

bool pMonkeyStudio::setLexerProperty( const QString& s, QsciLexer* l, const QVariant& v )
{
	// cancel no property, no lexer or if variant is not valid
	if ( !l || s.trimmed().isEmpty() || !v.isValid() )
		return false;
	// if bool
	if ( v.type() == QVariant::Bool )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( bool, v.toBool() ) );
	// if int
	else if ( v.type() == QVariant::Int )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( QsciLexerPython::IndentationWarning, (QsciLexerPython::IndentationWarning)v.toInt() ) );
	// return default value
	return false;
}

const QVariant pMonkeyStudio::lexerProperty( const QString& s, QsciLexer* l )
{
	// if no member or lexer return null variant
	if ( !l || s.isEmpty() )
		return QVariant();
	// get language
	const QString lng = QString( l->language() );
	// checking property
	if ( s == "foldComments" )
	{
		if ( lng == "Bash" )
			return qobject_cast<QsciLexerBash*>( l )->foldComments();
		else if ( lng == "CSS" )
			return qobject_cast<QsciLexerCSS*>( l )->foldComments();
		else if ( lng == "D" )
			return qobject_cast<QsciLexerD*>( l )->foldComments();
		else if ( lng == "Perl" )
			return qobject_cast<QsciLexerPerl*>( l )->foldComments();
		else if ( lng == "POV" )
			return qobject_cast<QsciLexerPOV*>( l )->foldComments();
		else if ( lng == "Python" )
			return qobject_cast<QsciLexerPython*>( l )->foldComments();
		else if ( lng == "SQL" )
			return qobject_cast<QsciLexerSQL*>( l )->foldComments();
		else if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldComments();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldComments();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldComments();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldComments();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldComments();
	}
	else if ( s == "foldCompact" )
	{
		if ( lng == "Bash" )
			return qobject_cast<QsciLexerBash*>( l )->foldCompact();
		else if ( lng == "CSS" )
			return qobject_cast<QsciLexerCSS*>( l )->foldCompact();
		else if ( lng == "D" )
			return qobject_cast<QsciLexerD*>( l )->foldCompact();
		else if ( lng == "HTML" )
			return qobject_cast<QsciLexerHTML*>( l )->foldCompact();
		else if ( lng == "Lua" )
			return qobject_cast<QsciLexerLua*>( l )->foldCompact();
		else if ( lng == "Perl" )
			return qobject_cast<QsciLexerPerl*>( l )->foldCompact();
		else if ( lng == "POV" )
			return qobject_cast<QsciLexerPOV*>( l )->foldCompact();
		else if ( lng == "Properties" )
			return qobject_cast<QsciLexerProperties*>( l )->foldCompact();
		else if ( lng == "SQL" )
			return qobject_cast<QsciLexerSQL*>( l )->foldCompact();
		else if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldCompact();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldCompact();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldCompact();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldCompact();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldCompact();
	}
	else if ( s == "foldQuotes" )
	{
		if ( lng == "Python" )
			return qobject_cast<QsciLexerPython*>( l )->foldQuotes();
	}
	else if ( s == "foldDirectives" )
	{
		if ( lng == "POV" )
			return qobject_cast<QsciLexerPOV*>( l )->foldDirectives();
	}
	else if ( s == "foldAtBegin" )
	{
		if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldAtBegin();
	}
	else if ( s == "foldAtParenthesis" )
	{
		if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldAtParenthesis();
	}
	else if ( s == "foldAtElse" )
	{
		if ( lng == "CMake" )
			return qobject_cast<QsciLexerCMake*>( l )->foldAtElse();
		else if ( lng == "D" )
			return qobject_cast<QsciLexerD*>( l )->foldAtElse();
		else if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldAtElse();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldAtElse();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldAtElse();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldAtElse();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldAtElse();
	}
	else if ( s == "foldPreprocessor" )
	{
		if ( lng == "HTML" )
			return qobject_cast<QsciLexerHTML*>( l )->foldPreprocessor();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldPreprocessor();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldPreprocessor();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldPreprocessor();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldPreprocessor();
	}
	else if ( s == "stylePreprocessor" )
	{
		if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->stylePreprocessor();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->stylePreprocessor();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( l )->stylePreprocessor();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->stylePreprocessor();
	}
	else if ( s == "caseSensitiveTags" )
	{
		if ( lng == "HTML" )
			return qobject_cast<QsciLexerHTML*>( l )->caseSensitiveTags();
	}
	else if ( s == "backslashEscapes" )
	{
		if ( lng == "SQL" )
			return qobject_cast<QsciLexerSQL*>( l )->backslashEscapes();
	}
	else if ( s == "indentationWarning" )
	{
		if ( lng == "Python" )
			return qobject_cast<QsciLexerPython*>( l )->indentationWarning();
	}
	// default return value
	return QVariant();
}

void pMonkeyStudio::resetLexer( QsciLexer* l )
{
	// cancel if no lexer
	if ( !l )
		return;
	// reset lexer settings
	pSettings::instance()->remove( QString( "%1/%2" ).arg( scintillaSettingsPath() ).arg( l->language() ) );
	// read default settings
	l->readSettings( *pSettings::instance(), qPrintable( scintillaSettingsPath() ) );
	// emit changes
	l->refreshProperties();
}

void pMonkeyStudio::applyProperties()
{
	// apply editor properties
	foreach ( pAbstractChild* c, pWorkspace::instance()->children() )
		foreach ( pEditor* e, c->findChildren<pEditor*>() )
			setEditorProperties( e );
	// apply lexers properties
	foreach ( QsciLexer* l, mGlobalsLexers.values() )
	{
		// refresh properties
		l->readSettings( *pSettings::instance(), qPrintable( scintillaSettingsPath() ) );
		// refresh default pen/paper if needed
		if ( defaultDocumentColours() )
		{
			l->setDefaultColor( defaultDocumentPen() );
			l->setDefaultPaper( defaultDocumentPaper() );
		}
		// reload apis
		if ( l->apis() )
			foreach ( QString s, pSettings::instance()->value( QString( "SourceAPIs/" ).append( l->language() ) ).toStringList() )
				l->apis()->loadPrepared( s );
	}
}

void pMonkeyStudio::setEditorProperties( pEditor* e )
{
	if ( !e )
		return;
	// apply settings from UISettings
	// General
	e->setSelectionBackgroundColor( selectionBackgroundColor() );
	e->setSelectionForegroundColor( selectionForegroundColor() );
	if ( defaultDocumentColours() )
	{
		// set scintilla default colors
		e->setColor( defaultDocumentPen() );
		e->setPaper( defaultDocumentPaper() );
	}
	// Auto Completion
	e->setAutoCompletionCaseSensitivity( autoCompletionCaseSensitivity() );
	e->setAutoCompletionReplaceWord( autoCompletionReplaceWord() );
	e->setAutoCompletionShowSingle( autoCompletionShowSingle() );
	e->setAutoCompletionSource( autoCompletionSource() );
	e->setAutoCompletionThreshold( autoCompletionThreshold() );
	// CallTips
	e->setCallTipsBackgroundColor( callTipsBackgroundColor() );
	e->setCallTipsForegroundColor( callTipsForegroundColor() );
	e->setCallTipsHighlightColor( callTipsHighlightColor() );
	e->setCallTipsStyle( callTipsStyle() );
	e->setCallTipsVisible( callTipsVisible() );
	// Indentation
	e->setAutoIndent( autoIndent() );
	e->setBackspaceUnindents( backspaceUnindents() );
	e->setIndentationGuides( indentationGuides() );
	e->setIndentationsUseTabs( indentationsUseTabs() );
	e->setIndentationWidth( indentationWidth() );
	e->setTabIndents( tabIndents() );
	e->setTabWidth( tabWidth() );
	e->setIndentationGuidesBackgroundColor( indentationGuidesBackgroundColor() );
	e->setIndentationGuidesForegroundColor( indentationGuidesForegroundColor() );
	// Brace Matching
	e->setBraceMatching( braceMatching() );
	e->setMatchedBraceBackgroundColor( matchedBraceBackgroundColor() );
	e->setMatchedBraceForegroundColor( matchedBraceForegroundColor() );
	e->setUnmatchedBraceBackgroundColor( unmatchedBraceBackgroundColor() );
	e->setUnmatchedBraceForegroundColor( unmatchedBraceForegroundColor() );
	// Edge Mode
	e->setEdgeMode( edgeMode() );
	e->setEdgeColor( edgeColor() );
	e->setEdgeColumn( edgeColumn() );
	// Caret
	e->setCaretLineVisible( caretLineVisible() );
	e->setCaretLineBackgroundColor( caretLineBackgroundColor() );
	e->setCaretForegroundColor( caretForegroundColor() );
	e->setCaretWidth( caretWidth() );
	// Margins
	if ( marginsEnabled() )
	{
		e->setMarginsBackgroundColor( marginsBackgroundColor() );
		e->setMarginsForegroundColor( marginsForegroundColor() );
		e->setMarginsFont( marginsFont() );
	}
	e->setLineNumbersMarginEnabled( lineNumbersMarginEnabled() );
	e->setLineNumbersMarginWidth( lineNumbersMarginWidth() );
	e->setLineNumbersMarginAutoWidth( lineNumbersMarginAutoWidth() );
	e->setFolding( folding() );
	e->setFoldMarginColors( foldMarginForegroundColor(), foldMarginBackgroundColor() );
	// Special Characters
	e->setEolMode( eolMode() );
	e->setEolVisibility( eolVisibility() );
	e->setWhitespaceVisibility( whitespaceVisibility() );
	e->setWrapMode( wrapMode() );
	e->setWrapVisualFlags( endWrapVisualFlag(), startWrapVisualFlag(), wrappedLineIndentWidth() );
}

void pMonkeyStudio::setRestoreProjectsOnStartup( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/RestoreProjectsOnStartup", b );
}

const bool pMonkeyStudio::restoreProjectsOnStartup()
{
	return pSettings::instance()->value( settingsPath() +"/RestoreProjectsOnStartup", true ).toBool();
}

void pMonkeyStudio::setDefaultProjectsDirectory( const QString& s )
{
	pSettings::instance()->setValue( settingsPath() +"/DefaultProjectsDirectory", s );
}

const QString pMonkeyStudio::defaultProjectsDirectory()
{
	return pSettings::instance()->value( settingsPath() +"/DefaultProjectsDirectory", "$HOME$/.Monkey Studio/Projects" ).toString();
}

void pMonkeyStudio::setUIDesignerMode( pMonkeyStudio::UIDesignerMode d )
{
	pSettings::instance()->setValue( settingsPath() +"/UIDesignerMode", d );
}

const pMonkeyStudio::UIDesignerMode pMonkeyStudio::uiDesignerMode()
{
	return (pMonkeyStudio::UIDesignerMode)pSettings::instance()->value( settingsPath() +"/UIDesignerMode", pMonkeyStudio::uidmExternal ).toInt();
}

void pMonkeyStudio::setExternalChanges( pMonkeyStudio::ExternalChangesMode e )
{
	pSettings::instance()->setValue( settingsPath() +"/ExternalChanges", e );
}

const pMonkeyStudio::ExternalChangesMode pMonkeyStudio::externalchanges()
{
	return (pMonkeyStudio::ExternalChangesMode)pSettings::instance()->value( settingsPath() +"/ExternalChanges", pMonkeyStudio::ecmAlert ).toInt();
}

void pMonkeyStudio::setSaveSessionOnClose( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/SaveSessionOnClose", b );
}

const bool pMonkeyStudio::saveSessionOnClose()
{
	return pSettings::instance()->value( settingsPath() +"/SaveSessionOnClose", true ).toBool();
}

void pMonkeyStudio::setRestoreSessionOnStartup( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/RestoreSessionOnStartup", b );
}

const bool pMonkeyStudio::restoreSessionOnStartup()
{
	return pSettings::instance()->value( settingsPath() +"/RestoreSessionOnStartup", true ).toBool();
}

void pMonkeyStudio::setAutoSyntaxCheck( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoSyntaxCheck", b );
}

const bool pMonkeyStudio::autoSyntaxCheck()
{
	return pSettings::instance()->value( settingsPath() +"/AutoSyntaxCheck", false ).toBool();
}

void pMonkeyStudio::setConvertTabsUponOpen( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/ConvertTabsUponOpen", b );
}

const bool pMonkeyStudio::convertTabsUponOpen()
{
	return pSettings::instance()->value( settingsPath() +"/ConvertTabsUponOpen", false ).toBool();
}

void pMonkeyStudio::setCreateBackupUponOpen( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/CreateBackupUponOpen", b );
}

const bool pMonkeyStudio::createBackupUponOpen()
{
	return pSettings::instance()->value( settingsPath() +"/CreateBackupUponOpen", false ).toBool();
}

void pMonkeyStudio::setAutoEolConversion( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoEolConversion", b );
}

const bool pMonkeyStudio::autoEolConversion()
{
	return pSettings::instance()->value( settingsPath() +"/AutoEolConversion", true ).toBool();
}

void pMonkeyStudio::setDefaultEncoding( const QString& s )
{
	pSettings::instance()->setValue( settingsPath() +"/DefaultEncoding", s );
}

const QString pMonkeyStudio::defaultEncoding()
{
	return pSettings::instance()->value( settingsPath() +"/DefaultEncoding", "UTF-8" ).toString();
}

void pMonkeyStudio::setSelectionBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/SelectionBackgroundColor", c );
}

const QColor pMonkeyStudio::selectionBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/SelectionBackgroundColor", QColor( "#bdff9b" ) ).value<QColor>();
}

void pMonkeyStudio::setSelectionForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/SelectionForegroundColor", c );
}

const QColor pMonkeyStudio::selectionForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/SelectionForegroundColor", QColor( "#000000" ) ).value<QColor>();
}

void pMonkeyStudio::setDefaultDocumentColours( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/DefaultDocumentColours", b );
}

const bool pMonkeyStudio::defaultDocumentColours()
{
	return pSettings::instance()->value( settingsPath() +"/DefaultDocumentColours", false ).toBool();
}

void pMonkeyStudio::setDefaultDocumentPen( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/DefaultDocumentPen", c );
}

const QColor pMonkeyStudio::defaultDocumentPen()
{
	return pSettings::instance()->value( settingsPath() +"/DefaultDocumentPen", QColor( Qt::black ) ).value<QColor>();
}

void pMonkeyStudio::setDefaultDocumentPaper( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/DefaultDocumentPaper", c );
}

const QColor pMonkeyStudio::defaultDocumentPaper()
{
	return pSettings::instance()->value( settingsPath() +"/DefaultDocumentPaper", QColor( Qt::white ) ).value<QColor>();
}

void pMonkeyStudio::setAutoCompletionCaseSensitivity( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoCompletionCaseSensitivity", b );
}

const bool pMonkeyStudio::autoCompletionCaseSensitivity()
{
	return pSettings::instance()->value( settingsPath() +"/AutoCompletionCaseSensitivity", true ).toBool();
}

void pMonkeyStudio::setAutoCompletionReplaceWord( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoCompletionReplaceWord", b );
}

const bool pMonkeyStudio::autoCompletionReplaceWord()
{
	return pSettings::instance()->value( settingsPath() +"/AutoCompletionReplaceWord", true ).toBool();
}

void pMonkeyStudio::setAutoCompletionShowSingle( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoCompletionShowSingle", b );
}

const bool pMonkeyStudio::autoCompletionShowSingle()
{
	return pSettings::instance()->value( settingsPath() +"/AutoCompletionShowSingle", false ).toBool();
}

void pMonkeyStudio::setAutoCompletionSource( QsciScintilla::AutoCompletionSource a )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoCompletionSource", a );
}

const QsciScintilla::AutoCompletionSource pMonkeyStudio::autoCompletionSource()
{
	return (QsciScintilla::AutoCompletionSource)pSettings::instance()->value( settingsPath() +"/AutoCompletionSource", (int)QsciScintilla::AcsAll ).toInt();
}

void pMonkeyStudio::setAutoCompletionThreshold( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoCompletionThreshold", i );
}

const int pMonkeyStudio::autoCompletionThreshold()
{
	return pSettings::instance()->value( settingsPath() +"/AutoCompletionThreshold", 3 ).toInt();
}

void pMonkeyStudio::setCallTipsBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/CallTipsBackgroundColor", c );
}

const QColor pMonkeyStudio::callTipsBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/CallTipsBackgroundColor", QColor( "#ffff9b" ) ).value<QColor>();
}

void pMonkeyStudio::setCallTipsForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/CallTipsForegroundColor", c );
}

const QColor pMonkeyStudio::callTipsForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/CallTipsForegroundColor", QColor( "#000000" ) ).value<QColor>();
}

void pMonkeyStudio::setCallTipsHighlightColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/CallTipsHighlightColor", c );
}

const QColor pMonkeyStudio::callTipsHighlightColor()
{
	return pSettings::instance()->value( settingsPath() +"/CallTipsHighlightColor", QColor( "#ff0000" ) ).value<QColor>();
}

void pMonkeyStudio::setCallTipsStyle( QsciScintilla::CallTipsStyle s )
{
	pSettings::instance()->setValue( settingsPath() +"/CallTipsStyle", s );
}

const QsciScintilla::CallTipsStyle pMonkeyStudio::callTipsStyle()
{
	return (QsciScintilla::CallTipsStyle)pSettings::instance()->value( settingsPath() +"/CallTipsStyle", (int)QsciScintilla::CallTipsContext ).toInt();
}

void pMonkeyStudio::setCallTipsVisible( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/CallTipsVisible", i );
}

const int pMonkeyStudio::callTipsVisible()
{
	return pSettings::instance()->value( settingsPath() +"/CallTipsVisible", -1 ).toInt();
}

void pMonkeyStudio::setAutoIndent( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/AutoIndent", b );
}

const bool pMonkeyStudio::autoIndent()
{
	return pSettings::instance()->value( settingsPath() +"/AutoIndent", true ).toBool();
}

void pMonkeyStudio::setBackspaceUnindents( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/BackspaceUnindents", b );
}

const bool pMonkeyStudio::backspaceUnindents()
{
	return pSettings::instance()->value( settingsPath() +"/BackspaceUnindents", true ).toBool();
}

void pMonkeyStudio::setIndentationGuides( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/IndentationGuides", b );
}

const bool pMonkeyStudio::indentationGuides()
{
	return pSettings::instance()->value( settingsPath() +"/IndentationGuides", true ).toBool();
}

void pMonkeyStudio::setIndentationsUseTabs( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/IndentationsUseTabs", b );
}

const bool pMonkeyStudio::indentationsUseTabs()
{
	return pSettings::instance()->value( settingsPath() +"/IndentationsUseTabs", true ).toBool();
}

void pMonkeyStudio::setIndentationWidth( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/IndentationWidth", i );
}

const int pMonkeyStudio::indentationWidth()
{
	return pSettings::instance()->value( settingsPath() +"/IndentationWidth", 4 ).toInt();
}

void pMonkeyStudio::setTabIndents( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/TabIndents", b );
}

const bool pMonkeyStudio::tabIndents()
{
	return pSettings::instance()->value( settingsPath() +"/TabIndents", true ).toBool();
}

void pMonkeyStudio::setTabWidth( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/TabWidth", i );
}

const int pMonkeyStudio::tabWidth()
{
	return pSettings::instance()->value( settingsPath() +"/TabWidth", 4 ).toInt();
}

void pMonkeyStudio::setIndentationGuidesBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/IndentationGuidesBackgroundColor", c );
}

const QColor pMonkeyStudio::indentationGuidesBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/IndentationGuidesBackgroundColor", QColor( "#0000ff" ) ).value<QColor>();
}

void pMonkeyStudio::setIndentationGuidesForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/IndentationGuidesForegroundColor", c );
}

const QColor pMonkeyStudio::indentationGuidesForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/IndentationGuidesForegroundColor", QColor( "#0000ff" ) ).value<QColor>();
}

void pMonkeyStudio::setBraceMatching( QsciScintilla::BraceMatch b )
{
	pSettings::instance()->setValue( settingsPath() +"/BraceMatching", b );
}

const QsciScintilla::BraceMatch pMonkeyStudio::braceMatching()
{
	return (QsciScintilla::BraceMatch)pSettings::instance()->value( settingsPath() +"/BraceMatching", (int)QsciScintilla::SloppyBraceMatch ).toInt();
}

void pMonkeyStudio::setMatchedBraceBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/MatchedBraceBackgroundColor", c );
}

const QColor pMonkeyStudio::matchedBraceBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/MatchedBraceBackgroundColor", QColor( "#ffff7f" ) ).value<QColor>();
}

void pMonkeyStudio::setMatchedBraceForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/MatchedBraceForegroundColor", c );
}

const QColor pMonkeyStudio::matchedBraceForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/MatchedBraceForegroundColor", QColor( "#ff0000" ) ).value<QColor>();
}

void pMonkeyStudio::setUnmatchedBraceBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/UnmatchedBraceBackgroundColor", c );
}

const QColor pMonkeyStudio::unmatchedBraceBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/UnmatchedBraceBackgroundColor", QColor( "#55aa00" ) ).value<QColor>();
}

void pMonkeyStudio::setUnmatchedBraceForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/UnmatchedBraceForegroundColor", c );
}

const QColor pMonkeyStudio::unmatchedBraceForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/UnmatchedBraceForegroundColor", QColor( "#ff0000" ) ).value<QColor>();
}

void pMonkeyStudio::setEdgeMode( QsciScintilla::EdgeMode m )
{
	pSettings::instance()->setValue( settingsPath() +"/EdgeMode", m );
}

const QsciScintilla::EdgeMode pMonkeyStudio::edgeMode()
{
	return (QsciScintilla::EdgeMode)pSettings::instance()->value( settingsPath() +"/EdgeMode", (int)QsciScintilla::EdgeNone ).toInt();
}

void pMonkeyStudio::setEdgeColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/EdgeColor", c );
}

const QColor pMonkeyStudio::edgeColor()
{
	return pSettings::instance()->value( settingsPath() +"/EdgeColor", QColor( "#ff0000" ) ).value<QColor>();
}

void pMonkeyStudio::setEdgeColumn( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/EdgeColumn", i );
}

const int pMonkeyStudio::edgeColumn()
{
	return pSettings::instance()->value( settingsPath() +"/EdgeColumn", 80 ).toInt();
}

void pMonkeyStudio::setCaretLineVisible( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/CaretLineVisible", b );
}

const bool pMonkeyStudio::caretLineVisible()
{
	return pSettings::instance()->value( settingsPath() +"/CaretLineVisible", true ).toBool();
}

void pMonkeyStudio::setCaretLineBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/CaretLineBackgroundColor", c );
}

const QColor pMonkeyStudio::caretLineBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/CaretLineBackgroundColor", QColor( "#aaaaff" ) ).value<QColor>();
}

void pMonkeyStudio::setCaretForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/CaretForegroundColor", c );
}

const QColor pMonkeyStudio::caretForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/CaretForegroundColor", QColor( "#000000" ) ).value<QColor>();
}

void pMonkeyStudio::setCaretWidth( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/CaretWidth", i );
}

const int pMonkeyStudio::caretWidth()
{
	return pSettings::instance()->value( settingsPath() +"/CaretWidth", 1 ).toInt();
}

void pMonkeyStudio::setLineNumbersMarginEnabled( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/LineNumbersMarginEnabled", b );
}

const bool pMonkeyStudio::lineNumbersMarginEnabled()
{
	return pSettings::instance()->value( settingsPath() +"/LineNumbersMarginEnabled", true ).toBool();
}

void pMonkeyStudio::setLineNumbersMarginWidth( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/LineNumbersMarginWidth", i );
}

const int pMonkeyStudio::lineNumbersMarginWidth()
{
	return pSettings::instance()->value( settingsPath() +"/LineNumbersMarginWidth", 4 ).toInt();
}

void pMonkeyStudio::setLineNumbersMarginAutoWidth( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/LineNumbersMarginAutoWidth", b );
}

const bool pMonkeyStudio::lineNumbersMarginAutoWidth()
{
	return pSettings::instance()->value( settingsPath() +"/LineNumbersMarginAutoWidth", true ).toBool();
}

void pMonkeyStudio::setFolding( QsciScintilla::FoldStyle f )
{
	pSettings::instance()->setValue( settingsPath() +"/Folding", f );
}

const QsciScintilla::FoldStyle pMonkeyStudio::folding()
{
	return (QsciScintilla::FoldStyle)pSettings::instance()->value( settingsPath() +"/Folding", (int)QsciScintilla::BoxedTreeFoldStyle ).toInt();
}

void pMonkeyStudio::setFoldMarginBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/FoldMarginBackgroundColor", c );
}

const QColor pMonkeyStudio::foldMarginBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/FoldMarginBackgroundColor", QColor( "#c0c0c0" ) ).value<QColor>();
}

void pMonkeyStudio::setFoldMarginForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/FoldMarginForegroundColor", c );
}

const QColor pMonkeyStudio::foldMarginForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/FoldMarginForegroundColor", QColor( "#000000" ) ).value<QColor>();
}

void pMonkeyStudio::setMarginsEnabled( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/MarginsEnabled", b );
}

const bool pMonkeyStudio::marginsEnabled()
{
	return pSettings::instance()->value( settingsPath() +"/MarginsEnabled", false ).toBool();
}

void pMonkeyStudio::setMarginsBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/MarginsBackgroundColor", c );
}

const QColor pMonkeyStudio::marginsBackgroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/MarginsBackgroundColor", QColor( "#c0c0c0" ) ).value<QColor>();
}

void pMonkeyStudio::setMarginsForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( settingsPath() +"/MarginsForegroundColor", c );
}

const QColor pMonkeyStudio::marginsForegroundColor()
{
	return pSettings::instance()->value( settingsPath() +"/MarginsForegroundColor", QColor( "#000000" ) ).value<QColor>();
}

void pMonkeyStudio::setMarginsFont( const QFont& f )
{
	pSettings::instance()->setValue( settingsPath() +"/MarginsFont", f.toString() );
}

const QFont pMonkeyStudio::marginsFont()
{
	QFont f;
	f.fromString( pSettings::instance()->value( settingsPath() +"/MarginsFont", f.toString() ).toString() );
	return f;
}

void pMonkeyStudio::setEolMode( QsciScintilla::EolMode e )
{
	pSettings::instance()->setValue( settingsPath() +"/EolMode", e );
}

const QsciScintilla::EolMode pMonkeyStudio::eolMode()
{
#if defined( Q_WS_WIN )
	int i = QsciScintilla::EolWindows;
#elif defined( Q_WS_MAC )
	int i = QsciScintilla::EolMac;
#else
	int i = QsciScintilla::EolUnix;
#endif
	return (QsciScintilla::EolMode)pSettings::instance()->value( settingsPath() +"/EolMode", i ).toInt();
}

void pMonkeyStudio::setEolVisibility( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/EolVisibility", b );
}

const bool pMonkeyStudio::eolVisibility()
{
	return pSettings::instance()->value( settingsPath() +"/EolVisibility", false ).toBool();
}

void pMonkeyStudio::setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility w )
{
	pSettings::instance()->setValue( settingsPath() +"/WhitespaceVisibility", w );
}

const QsciScintilla::WhitespaceVisibility pMonkeyStudio::whitespaceVisibility()
{
	return (QsciScintilla::WhitespaceVisibility)pSettings::instance()->value( settingsPath() +"/WhitespaceVisibility", (int)QsciScintilla::WsInvisible ).toInt();
}

void pMonkeyStudio::setWrapMode( QsciScintilla::WrapMode w )
{
	pSettings::instance()->setValue( settingsPath() +"/WrapMode", w );
}

const QsciScintilla::WrapMode pMonkeyStudio::wrapMode()
{
	return (QsciScintilla::WrapMode)pSettings::instance()->value( settingsPath() +"/WrapMode", (int)QsciScintilla::WrapNone ).toInt();
}

void pMonkeyStudio::setWrapVisualFlagsEnabled( bool b )
{
	pSettings::instance()->setValue( settingsPath() +"/WrapVisualFlagsEnabled", b );
}

const bool pMonkeyStudio::wrapVisualFlagsEnabled()
{
	return pSettings::instance()->value( settingsPath() +"/WrapVisualFlagsEnabled", false ).toBool();
}

void pMonkeyStudio::setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag f )
{
	pSettings::instance()->setValue( settingsPath() +"/StartWrapVisualFlag", f );
}

const QsciScintilla::WrapVisualFlag pMonkeyStudio::startWrapVisualFlag()
{
	return (QsciScintilla::WrapVisualFlag)pSettings::instance()->value( settingsPath() +"/StartWrapVisualFlag", (int)QsciScintilla::WrapFlagNone ).toInt();
}

void pMonkeyStudio::setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag f )
{
	pSettings::instance()->setValue( settingsPath() +"/EndWrapVisualFlag", f );
}

const QsciScintilla::WrapVisualFlag pMonkeyStudio::endWrapVisualFlag()
{
	return (QsciScintilla::WrapVisualFlag)pSettings::instance()->value( settingsPath() +"/EndWrapVisualFlag", (int)QsciScintilla::WrapFlagNone ).toInt();
}

void pMonkeyStudio::setWrappedLineIndentWidth( int i )
{
	pSettings::instance()->setValue( settingsPath() +"/WrappedLineIndentWidth", i );
}

const int pMonkeyStudio::wrappedLineIndentWidth()
{
	return pSettings::instance()->value( settingsPath() +"/WrappedLineIndentWidth", 0 ).toInt();
}
