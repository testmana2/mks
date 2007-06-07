/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : lundi 30 janvier 2006 - 17h48
 * AUTHOR       : Nox P@sNox ( pasnox@hotmail.com )
 * FILENAME     : Settings.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "Settings.h"

#include <QDir>
#include <QMainWindow>
#include <QTextCodec>

#include <QHash>
#include <QFileInfo>
#include <QFile>


#include "qsciscintilla.h"
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

Settings::Settings( const QString& pName, const QString& pVersion )
	: QSettings( QDir::convertSeparators( QString( "%1/.%2/%3.ini" ).arg( QDir::homePath(), pName, pName ) ), QSettings::IniFormat )
{
	mProgramName = pName;
	mProgramVersion = pVersion;
	beginGroup( mProgramVersion );
}

Settings::~Settings()
{
	endGroup();
}

QString Settings::programName() const
{
	return mProgramName;
}

QString Settings::programVersion() const
{
	return mProgramVersion;
}

void Settings::restoreState( QMainWindow* w )
{
	if ( !value( "MainWindow/Maximized", true ).toBool() )
		w->setGeometry( value( "MainWindow/Geometry" ).toRect() );
	else
		w->showMaximized();
	w->restoreState( value( "MainWindow/State" ).toByteArray() );
}

void Settings::saveState( QMainWindow* w )
{
	setValue( "MainWindow/Maximized", w->isMaximized() );
	setValue( "MainWindow/Geometry", w->geometry() );
	setValue( "MainWindow/State", w->saveState() );
}

void Settings::setDefaultSettings()
{
	//
}

QTextCodec* Settings::defaultCodec()
{
	return QTextCodec::codecForName( qPrintable( instance()->value( "Settings/Editor/General/DefaultEncoding", "UTF-8" ).toString() ) );
}

QsciLexer* Settings::lexerForLanguage( const QString& s )
{
	Settings* ss = instance();
	QString l = s.toLower();
	const char* c = qPrintable( QString( "Settings/Editor/Scintilla" ) );
	QString sp = "Settings/Editor/Properties";
	if ( l == "bash" )
	{
		QsciLexerBash* sl = new QsciLexerBash( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/BashFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		return sl;
	}
	else if ( l == "batch" )
	{
		QsciLexerBatch* sl = new QsciLexerBatch( current() );
		sl->readSettings( *ss, c );
		return sl;
	}
	else if ( l == "cmake" )
	{
		QsciLexerCMake* sl = new QsciLexerCMake( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/CMakeFoldAtElse", true ).toBool() );
		return sl;
	}
	else if ( l == "c++" )
	{
		QsciLexerCPP* sl = new QsciLexerCPP( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/CppFoldAtElse", false ).toBool() );
		sl->setFoldComments( ss->value( sp +"/CppFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldPreprocessor( ss->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
		sl->setStylePreprocessor( ss->value( sp +"/CppStylePreprocessorDirectives", false ).toBool() );
		int i = 0;
		if ( ss->value( sp +"/CppIndentOpeningBrace", false ).toBool() )
			i |= QsciScintilla::AiOpening;
		if ( ss->value( sp +"/CppIndentClosingBrace", false ).toBool() )
			i |= QsciScintilla::AiClosing;
		sl->setAutoIndentStyle( i );
		return sl;
	}
	else if ( l == "c#" )
	{
		QsciLexerCSharp* sl = new QsciLexerCSharp( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/CppFoldAtElse", false ).toBool() );
		sl->setFoldComments( ss->value( sp +"/CppFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldPreprocessor( ss->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
		sl->setStylePreprocessor( ss->value( sp +"/CppStylePreprocessorDirectives", false ).toBool() );
		return sl;
	}
	else if ( l == "css" )
	{
		QsciLexerCSS* sl = new QsciLexerCSS( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/CSSFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		return sl;
	}
	else if ( l == "d" )
	{
		QsciLexerD* sl = new QsciLexerD( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/DFoldAtElse", false ).toBool() );
		sl->setFoldComments( ss->value( sp +"/DFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		return sl;
	}
	else if ( l == "diff" )
	{
		QsciLexerDiff* sl = new QsciLexerDiff( current() );
		sl->readSettings( *ss, c );
		return sl;
	}
	else if ( l == "html" )
	{
		QsciLexerHTML* sl = new QsciLexerHTML( current() );
		sl->readSettings( *ss, c );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldPreprocessor( ss->value( sp +"/HTMLFoldPreprocessorDirectives", false ).toBool() );
		sl->setCaseSensitiveTags( ss->value( sp +"/HTMLCaseSensitivesTags", false ).toBool() );
		return sl;
	}
	else if ( l == "idl" )
	{
		QsciLexerIDL* sl = new QsciLexerIDL( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/CppFoldAtElse", false ).toBool() );
		sl->setFoldComments( ss->value( sp +"/CppFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldPreprocessor( ss->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
		sl->setStylePreprocessor( ss->value( sp +"/CppStylePreprocessorDirectives", false ).toBool() );
		return sl;
	}
	else if ( l == "java" )
	{
		QsciLexerJava* sl = new QsciLexerJava( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/CppFoldAtElse", false ).toBool() );
		sl->setFoldComments( ss->value( sp +"/CppFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldPreprocessor( ss->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
		sl->setStylePreprocessor( ss->value( sp +"/CppStylePreprocessorDirectives", false ).toBool() );
		return sl;
	}
	else if ( l == "javascript" )
	{
		QsciLexerJavaScript* sl = new QsciLexerJavaScript( current() );
		sl->readSettings( *ss, c );
		sl->setFoldAtElse( ss->value( sp +"/CppFoldAtElse", false ).toBool() );
		sl->setFoldComments( ss->value( sp +"/CppFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldPreprocessor( ss->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
		sl->setStylePreprocessor( ss->value( sp +"/CppStylePreprocessorDirectives", false ).toBool() );
		return sl;
	}
	else if ( l == "lua" )
	{
		QsciLexerLua* sl = new QsciLexerLua( current() );
		sl->readSettings( *ss, c );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		return sl;
	}
	else if ( l == "makefile" )
	{
		QsciLexerMakefile* sl = new QsciLexerMakefile( current() );
		sl->readSettings( *ss, c );
		return sl;
	}
	else if ( l == "perl" )
	{
		QsciLexerPerl* sl = new QsciLexerPerl( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/PerlFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		return sl;
	}
	else if ( l == "pov" )
	{
		QsciLexerPOV* sl = new QsciLexerPOV( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/POVFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldDirectives( ss->value( sp +"/POVFoldPreprocessorDirectives", false ).toBool() );
		return sl;
	}
	else if ( l == "properties" )
	{
		QsciLexerProperties* sl = new QsciLexerProperties( current() );
		sl->readSettings( *ss, c );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		return sl;
	}
	else if ( l == "python" )
	{
		QsciLexerPython* sl = new QsciLexerPython( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/PythonFoldComments", true ).toBool() );
		sl->setFoldQuotes( ss->value( sp +"/PythonFoldStrings", true ).toBool() );
		sl->setIndentationWarning( (QsciLexerPython::IndentationWarning)ss->value( sp +"/PythonIndentationwarning", QsciLexerPython::NoWarning ).toInt() );
		return sl;
	}
	else if ( l == "ruby" )
	{
		QsciLexerRuby* sl = new QsciLexerRuby( current() );
		sl->readSettings( *ss, c );
		return sl;
	}
	else if ( l == "sql" )
	{
		QsciLexerSQL* sl = new QsciLexerSQL( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/SQLFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setBackslashEscapes( ss->value( sp +"/SQLBackslashEscapes", true ).toBool() );
		return sl;
	}
	else if ( l == "tex" )
	{
		QsciLexerTeX* sl = new QsciLexerTeX( current() );
		sl->readSettings( *ss, c );
		return sl;
	}
	else if ( l == "vhdl" )
	{
		QsciLexerVHDL* sl = new QsciLexerVHDL( current() );
		sl->readSettings( *ss, c );
		sl->setFoldComments( ss->value( sp +"/VHDLFoldComments", true ).toBool() );
		sl->setFoldCompact( ss->value( sp +"/AllFoldCompact", true ).toBool() );
		sl->setFoldAtElse( ss->value( sp +"/VHDLFoldAtElse", false ).toBool() );
		sl->setFoldAtBegin( ss->value( sp +"/VHDLFoldAtBegin", false ).toBool() );
		sl->setFoldAtParenthesis( ss->value( sp +"/VHDLFoldAtParenthesis", false ).toBool() );
		return sl;
	}
	return 0;
}

QsciLexer* Settings::lexerForFilename( const QString& s )
{
	return lexerForLanguage( languageForFilename( s ) );
}

QString Settings::languageForFilename( const QString& s )
{
	QStringList l = lexersFilters();
	QFileInfo f( s );
	QString ss = f.suffix();
	if ( ss.isEmpty() )
		ss = f.fileName();
	int i;
	if ( ( i = l.indexOf( QRegExp( QString( "\\*\\.%1" ).arg( ss ), Qt::CaseInsensitive ) ) ) != -1 ||
	( i = l.indexOf( QRegExp( QString( "\\*%1" ).arg( ss ), Qt::CaseInsensitive ) ) ) != -1 ||
	( i = l.indexOf( QRegExp( QString( "%1\\*" ).arg( ss ), Qt::CaseInsensitive ) ) ) != -1 )
		return current()->value( "Settings/Editor/HighlighterAssociation/" +l.at( i ) ).toString();
	return QString::null;
}

QStringList Settings::lexersFilters()
{
	Settings* s = instance();
	s->beginGroup( "Settings/Editor/HighlighterAssociation" );
	QStringList l = s->childKeys();
	s->endGroup();
	return l;
}

QString Settings::dialogLexersFilters()
{
	// temp hash
	QHash<QString, QStringList> h;
	QString f;
	// get all lexers and filters
	Settings* s = instance();
	s->beginGroup( "Settings/Editor/HighlighterAssociation" );
	foreach ( QString l, s->childKeys() )
	{
		f = s->value( l ).toString();
		if ( h.contains( f ) )
			h[f] << l;
		else
			h[f] = QStringList( l );
	}
	s->endGroup();
	// create dialog filter
	f.clear();
	foreach ( QString l, h.keys() )
		f.append( QString( "%1 Files ( %2 );;" ).arg( l, h.value( l ).join( " " ) ) );
	// remove trailing ;;
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	return f;
}
