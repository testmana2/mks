#include "pQScintilla.h"
#include "pSettings.h"

pQScintilla::pQScintilla( QObject* o )
	: QObject( o )
{
	// init lexers
	mLexers[ "Bash" ] = new QsciLexerBash( this );
	mLexers[ "Batch" ] = new QsciLexerBatch( this );
	mLexers[ "C#" ] = new QsciLexerCSharp( this );
	mLexers[ "C++" ] = new QsciLexerCPP( this );
	mLexers[ "CMake" ] = new QsciLexerCMake( this );
	mLexers[ "CSS" ] = new QsciLexerCSS( this );
	mLexers[ "D" ] = new QsciLexerD( this );
	mLexers[ "Diff" ] = new QsciLexerDiff( this );
	mLexers[ "HTML" ] = new QsciLexerHTML( this );
	mLexers[ "IDL" ] = new QsciLexerIDL( this );
	mLexers[ "Java" ] = new QsciLexerJava( this );
	mLexers[ "JavaScript" ] = new QsciLexerJavaScript( this );
	mLexers[ "Lua" ] = new QsciLexerLua( this );
	mLexers[ "Makefile" ] = new QsciLexerMakefile( this );
	mLexers[ "POV" ] = new QsciLexerPOV( this );
	mLexers[ "Perl" ] = new QsciLexerPerl( this );
	mLexers[ "Properties" ] = new QsciLexerProperties( this );
	mLexers[ "Python" ] = new QsciLexerPython( this );
	mLexers[ "Ruby" ] = new QsciLexerRuby( this );
	mLexers[ "SQL" ] = new QsciLexerSQL( this );
	mLexers[ "TeX" ] = new QsciLexerTeX( this );
	mLexers[ "VHDL" ] = new QsciLexerVHDL( this );
}

pQScintilla::~pQScintilla()
{
	qDeleteAll( mLexers );
	mLexers.clear();
}

QStringList pQScintilla::languages() const
{
	return mLexers.keys();
}

QHash<QString,QsciLexer*> pQScintilla::lexers() const
{
	return mLexers;
}

void pQScintilla::readSettings()
{
	// write settings
	foreach ( QsciLexer* l, mLexers )
		l->readSettings( *pSettings::instance() );
}

void pQScintilla::writeSettings()
{
	// read settings
	foreach ( QsciLexer* l, mLexers )
		l->writeSettings( *pSettings::instance() );
}

bool pQScintilla::setProperty( const QString& s, QsciLexer* l, const QVariant& v )
{
	// cancel f variant is not valid
	if ( !v.isValid() )
		return false;

	if ( v.type() == QVariant::Bool )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( bool, v.toBool() ) );
	else if ( v.type() == QVariant::Int )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( int, v.toInt() ) );

	// return default value
	return false;
}

QVariant pQScintilla::property( const QString& s, QsciLexer* l ) const
{
	// if no member or lexer return null variant
	if ( s.trimmed().isEmpty() || !l )
		return QVariant();

	if ( s == "foldComments" )
	{
		if ( l->language() == "Bash" )
			return qobject_cast<QsciLexerBash*>( l )->foldComments();
		else if ( l->language() == "CSS" )
			return qobject_cast<QsciLexerCSS*>( l )->foldComments();
		else if ( l->language() == "D" )
			return qobject_cast<QsciLexerD*>( l )->foldComments();
		else if ( l->language() == "Perl" )
			return qobject_cast<QsciLexerPerl*>( l )->foldComments();
		else if ( l->language() == "POV" )
			return qobject_cast<QsciLexerPOV*>( l )->foldComments();
		else if ( l->language() == "Python" )
			return qobject_cast<QsciLexerPython*>( l )->foldComments();
		else if ( l->language() == "SQL" )
			return qobject_cast<QsciLexerSQL*>( l )->foldComments();
		else if ( l->language() == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldComments();
		else if ( l->language() == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldComments();
		else if ( l->language() == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldComments();
		else if ( l->language() == "CSharp" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldComments();
		else if ( l->language() == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldComments();
	}
	else if ( s == "foldCompact" )
	{
		if ( l->language() == "Bash" )
			return qobject_cast<QsciLexerBash*>( l )->foldCompact();
		else if ( l->language() == "CSS" )
			return qobject_cast<QsciLexerCSS*>( l )->foldCompact();
		else if ( l->language() == "D" )
			return qobject_cast<QsciLexerD*>( l )->foldCompact();
		else if ( l->language() == "HTML" )
			return qobject_cast<QsciLexerHTML*>( l )->foldCompact();
		else if ( l->language() == "Lua" )
			return qobject_cast<QsciLexerLua*>( l )->foldCompact();
		else if ( l->language() == "Perl" )
			return qobject_cast<QsciLexerPerl*>( l )->foldCompact();
		else if ( l->language() == "POV" )
			return qobject_cast<QsciLexerPOV*>( l )->foldCompact();
		else if ( l->language() == "Properties" )
			return qobject_cast<QsciLexerProperties*>( l )->foldCompact();
		else if ( l->language() == "SQL" )
			return qobject_cast<QsciLexerSQL*>( l )->foldCompact();
		else if ( l->language() == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldCompact();
		else if ( l->language() == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldCompact();
		else if ( l->language() == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldCompact();
		else if ( l->language() == "CSharp" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldCompact();
		else if ( l->language() == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldCompact();
	}
	else if ( s == "foldQuotes" )
	{
		if ( l->language() == "Python" )
			return qobject_cast<QsciLexerPython*>( l )->foldQuotes();
	}
	else if ( s == "foldDirectives" )
	{
		if ( l->language() == "POV" )
			return qobject_cast<QsciLexerPOV*>( l )->foldDirectives();
	}
	else if ( s == "foldAtBegin" )
	{
		if ( l->language() == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldAtBegin();
	}
	else if ( s == "foldAtParenthesis" )
	{
		if ( l->language() == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldAtParenthesis();
	}
	else if ( s == "foldAtElse" )
	{
		if ( l->language() == "CMake" )
			return qobject_cast<QsciLexerCMake*>( l )->foldAtElse();
		else if ( l->language() == "D" )
			return qobject_cast<QsciLexerD*>( l )->foldAtElse();
		else if ( l->language() == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( l )->foldAtElse();
		else if ( l->language() == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldAtElse();
		else if ( l->language() == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldAtElse();
		else if ( l->language() == "CSharp" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldAtElse();
		else if ( l->language() == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldAtElse();
	}
	else if ( s == "foldPreprocessor" )
	{
		if ( l->language() == "HTML" )
			return qobject_cast<QsciLexerHTML*>( l )->foldPreprocessor();
		else if ( l->language() == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->foldPreprocessor();
		else if ( l->language() == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->foldPreprocessor();
		else if ( l->language() == "CSharp" )
			return qobject_cast<QsciLexerCSharp*>( l )->foldPreprocessor();
		else if ( l->language() == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->foldPreprocessor();
	}
	else if ( s == "stylePreprocessor" )
	{
		if ( l->language() == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( l )->stylePreprocessor();
		else if ( l->language() == "Java" )
			return qobject_cast<QsciLexerJava*>( l )->stylePreprocessor();
		else if ( l->language() == "CSharp" )
			return qobject_cast<QsciLexerCSharp*>( l )->stylePreprocessor();
		else if ( l->language() == "C++" )
			return qobject_cast<QsciLexerCPP*>( l )->stylePreprocessor();
	}
	else if ( s == "caseSensitiveTags" )
	{
		if ( l->language() == "HTML" )
			return qobject_cast<QsciLexerHTML*>( l )->caseSensitiveTags();
	}
	else if ( s == "backslashEscapes" )
	{
		if ( l->language() == "SQL" )
			return qobject_cast<QsciLexerSQL*>( l )->backslashEscapes();
	}
	else if ( s == "indentationWarning" )
	{
		if ( l->language() == "Python" )
			return qobject_cast<QsciLexerPython*>( l )->indentationWarning();
	}

	// default return value
	return QVariant();
}
