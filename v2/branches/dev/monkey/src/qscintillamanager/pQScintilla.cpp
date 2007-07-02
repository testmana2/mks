#include "pQScintilla.h"
#include "pSettings.h"

#include <QFileInfo>

const QString mPath = "/Scintilla";

pQScintilla::pQScintilla( QObject* o )
	: QObject( o )
{}

pQScintilla::~pQScintilla()
{
	qDeleteAll( mLexers );
	mLexers.clear();
}

QStringList pQScintilla::languages() const
{
	return QStringList() << "Bash" << "Batch" << "C#" << "C++" << "CMake" << "CSS"
		<< "D" << "Diff" << "HTML" << "IDL" << "Java" << "JavaScript" << "Lua"
		<< "Makefile" << "POV" << "Perl" << "Properties" << "Python" << "Ruby"
		<< "SQL" << "TeX" << "VHDL";
}

QsciLexer* pQScintilla::lexer( const QString& s )
{
	// get language
	const QString ln = s.toLower();

	// lexer
	QsciLexer* l = 0;

	// init lexer
	if ( ln == "bash" )
		l = new QsciLexerBash( this );
	else if ( ln == "batch" )
		l = new QsciLexerBatch( this );
	else if ( ln == "c#" )
		l = new QsciLexerCSharp( this );
	else if ( ln == "c++" )
		l = new QsciLexerCPP( this );
	else if ( ln == "cmake" )
		l = new QsciLexerCMake( this );
	else if ( ln == "css" )
		l = new QsciLexerCSS( this );
	else if ( ln == "d" )
		l = new QsciLexerD( this );
	else if ( ln == "diff" )
		l = new QsciLexerDiff( this );
	else if ( ln == "html" )
		l = new QsciLexerHTML( this );
	else if ( ln == "idl" )
		l = new QsciLexerIDL( this );
	else if ( ln == "java" )
		l = new QsciLexerJava( this );
	else if ( ln == "javascript" )
		l = new QsciLexerJavaScript( this );
	else if ( ln == "lua" )
		l = new QsciLexerLua( this );
	else if ( ln == "makefile" )
		l = new QsciLexerMakefile( this );
	else if ( ln == "pov" )
		l = new QsciLexerPOV( this );
	else if ( ln == "perl" )
		l = new QsciLexerPerl( this );
	else if ( ln == "properties" )
		l = new QsciLexerProperties( this );
	else if ( ln == "python" )
		l = new QsciLexerPython( this );
	else if ( ln == "ruby" )
		l = new QsciLexerRuby( this );
	else if ( ln == "sql" )
		l = new QsciLexerSQL( this );
	else if ( ln == "tex" )
		l = new QsciLexerTeX( this );
	else if ( ln == "vhdl" )
		l = new QsciLexerVHDL( this );

	// read lexer settings
	if ( l )
		l->readSettings( *pSettings::instance(), qPrintable( mPath ) );

	// return lexer
	return l;
}

QHash<QString,QsciLexer*> pQScintilla::lexers()
{
	// init lexers if needed
	if ( mLexers.isEmpty() )
		foreach ( QString s, languages() )
			mLexers[s] = lexer( s );

	// return lexers
	return mLexers;
}

void pQScintilla::readSettings()
{
	// read settings
	foreach ( QsciLexer* l, lexers() )
		l->readSettings( *pSettings::instance(), qPrintable( mPath ) );
}

void pQScintilla::writeSettings()
{
	// write settings
	foreach ( QsciLexer* l, lexers() )
		l->writeSettings( *pSettings::instance(), qPrintable( mPath ) );
}

void pQScintilla::resetLexer( QsciLexer* l )
{
	// cancel if no lexer
	if ( !l )
		return;

	// get language
	const QString s = l->language();

	// reset lexer
	pSettings::instance()->remove( QString( "%1/%2" ).arg( mPath ).arg( s ) );
	delete l;
	mLexers[s] = lexer( s );
}

bool pQScintilla::setProperty( const QString& s, QsciLexer* l, const QVariant& v )
{
	// cancel no property, no lexer or f variant is not valid
	if ( s.trimmed().isEmpty() || !l || !v.isValid() )
		return false;

	if ( v.type() == QVariant::Bool )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( bool, v.toBool() ) );
	else if ( v.userType() == QVariant::Int )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( QsciLexerPython::IndentationWarning, (QsciLexerPython::IndentationWarning)v.toInt() ) );

	// return default value
	return false;
}

QVariant pQScintilla::property( const QString& s, QsciLexer* l ) const
{
	// if no member or lexer return null variant
	if ( s.trimmed().isEmpty() || !l )
		return QVariant();

	const QString lng = QString( l->language() );

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

QHash<QString, QStringList> pQScintilla::defaultSuffixes() const
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

QHash<QString, QStringList> pQScintilla::suffixes() const
{
	// suffixes list
	QHash<QString, QStringList> l;

	// get settings
	pSettings* s = pSettings::instance();

	// get associations from settings
	s->beginGroup( mPath +"/Associations" );
	foreach ( QString k, s->childKeys() )
		l[s->value( k ).toString()] << k;
	s->endGroup();

	if ( l.isEmpty() )
		l = defaultSuffixes();

	return l;
}

QsciLexer* pQScintilla::lexerForFilename( const QString& s )
{
	// get suffixes
	QHash<QString, QStringList> l = suffixes();

	// file suffixe
	const QString sf = QFileInfo( s ).suffix().prepend( "*." );

	// basic setup need change for makefile and cmake
	foreach ( QString k, l.keys() )
		if ( l.value( k ).contains( sf, Qt::CaseInsensitive ) )
			return lexer( k );

	// return no lexer if not found
	return 0;
}

void pQScintilla::setAutoCompletionCaseSensitivity( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionCaseSensitivity", b );
}

bool pQScintilla::autoCompletionCaseSensitivity() const
{
	pSettings::instance()->value( mPath +"/AutoCompletionCaseSensitivity", true ).toBool();
}

void pQScintilla::setAutoCompletionReplaceWord( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionReplaceWord", b );
}

bool pQScintilla::autoCompletionReplaceWord() const
{
	pSettings::instance()->value( mPath +"/AutoCompletionReplaceWord", false ).toBool();
}

void pQScintilla::setAutoCompletionShowSingle( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionShowSingle", b );
}

bool pQScintilla::autoCompletionShowSingle() const
{
	pSettings::instance()->value( mPath +"/AutoCompletionShowSingle", false ).toBool();
}

void setAutoCompletionSource( QsciScintilla::AutoCompletionSource a )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionSource", a );
}

QsciScintilla::AutoCompletionSource pQScintilla::autoCompletionSource() const
{
	(QsciScintilla::AutoCompletionSource)pSettings::instance()->value( mPath +"/AutoCompletionSource", (int)QsciScintilla::AcsNone ).toInt();
}

void pQScintilla::setAutoCompletionThreshold( int i )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionThreshold", i );
}

int pQScintilla::autoCompletionThreshold() const
{
	pSettings::instance()->value( mPath +"/AutoCompletionThreshold", -1 ).toInt();
}

void pQScintilla::setAutoIndent( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoIndent", b );
}

bool pQScintilla::autoIndent() const
{
	pSettings::instance()->value( mPath +"/AutoIndent", false ).toBool();
}

void pQScintilla::setBraceMatching( QsciScintilla::BraceMatch b )
{
	pSettings::instance()->setValue( mPath +"/BraceMatching", b );
}

QsciScintilla::BraceMatch pQScintilla::braceMatching() const
{
	(QsciScintilla::BraceMatch)pSettings::instance()->value( mPath +"/BraceMatching", (int)QsciScintilla::NoBraceMatch ).toInt();
}

void pQScintilla::setBackspaceUnindents( bool b )
{
	pSettings::instance()->setValue( mPath +"/BackspaceUnindents", b );
}

bool pQScintilla::backspaceUnindents() const
{
	pSettings::instance()->value( mPath +"/BackspaceUnindents", false ).toBool();
}

void pQScintilla::setEolMode( QsciScintilla::EolMode e )
{
	pSettings::instance()->setValue( mPath +"/EolMode", e );
}

QsciScintilla::EolMode pQScintilla::eolMode() const
{
#if defined( Q_WS_WIN )
	int i = QsciScintilla::EolWindows;
#elseif defined( Q_WS_MAC )
	int i = QsciScintilla::EolMac;
#else
	int i = QsciScintilla::EolUnix;
#endif
	(QsciScintilla::EolMode)pSettings::instance()->value( mPath +"/EolMode", i ).toInt();
}

void pQScintilla::setEolVisibility( bool b )
{
	pSettings::instance()->setValue( mPath +"/EolVisibility", b );
}

bool pQScintilla::eolVisibility() const
{
	pSettings::instance()->value( mPath +"/EolVisibility", false ).toBool();
}

void pQScintilla::setFolding( QsciScintilla::FoldStyle f )
{
	pSettings::instance()->setValue( mPath +"/Folding", f );
}

QsciScintilla::FoldStyle pQScintilla::folding() const
{
	(QsciScintilla::FoldStyle)pSettings::instance()->value( mPath +"/Folding", (int)QsciScintilla::NoFoldStyle ).toInt();
}

void pQScintilla::setIndentationGuides( bool b )
{
	pSettings::instance()->setValue( mPath +"/IndentationGuides", b );
}

bool pQScintilla::indentationGuides() const
{
	pSettings::instance()->value( mPath +"/IndentationGuides", false ).toBool();
}

void pQScintilla::setIndentationsUseTabs( bool b )
{
	pSettings::instance()->setValue( mPath +"/IndentationsUseTabs", b );
}

bool pQScintilla::indentationsUseTabs() const
{
	pSettings::instance()->value( mPath +"/IndentationsUseTabs", true ).toBool();
}

void pQScintilla::setIndentationWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/IndentationWidth", i );
}

int pQScintilla::indentationWidth() const
{
	pSettings::instance()->value( mPath +"/IndentationWidth", 0 ).toInt();
}

void pQScintilla::setTabIndents( bool b )
{
	pSettings::instance()->setValue( mPath +"/TabIndents", b );
}

bool pQScintilla::tabIndents() const
{
	pSettings::instance()->value( mPath +"/TabIndents", true ).toBool();
}

void pQScintilla::setTabWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/TabWidth", i );
}

int pQScintilla::tabWidth() const
{
	pSettings::instance()->value( mPath +"/IndentationWidth", 8 ).toInt();
}

void pQScintilla::setUtf8( bool b )
{
	pSettings::instance()->setValue( mPath +"/Utf8", b );
}

bool pQScintilla::isUtf8() const
{
	pSettings::instance()->value( mPath +"/Utf8", false ).toBool();
}

void pQScintilla::setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility w )
{
	pSettings::instance()->setValue( mPath +"/WhitespaceVisibility", w );
}

QsciScintilla::WhitespaceVisibility pQScintilla::whitespaceVisibility() const
{
	(QsciScintilla::WhitespaceVisibility)pSettings::instance()->value( mPath +"/WhitespaceVisibility", (int)QsciScintilla::WsInvisible ).toInt();
}

void pQScintilla::setWrapMode( QsciScintilla::WrapMode w )
{
	pSettings::instance()->setValue( mPath +"/WrapMode", w );
}

QsciScintilla::WrapMode pQScintilla::wrapMode() const
{
	(QsciScintilla::WrapMode)pSettings::instance()->value( mPath +"/WrapMode", (int)QsciScintilla::WrapNone ).toInt();
}

