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

void pQScintilla::setProperty( const QString& s, QsciLexer* l, const QVariant v )
{
	l->metaObject()->invokeMethod( l, s, Q_ARG( bool, v ) );
}

QVariant pQScintilla::property( const QString& s, QsciLexer* l ) const
{
	// found lexer properties
	const QMetaObject* mo = l->metaObject();
	int i;
	bool b;

	// fold comments
	i = mo->indexOfSlot( "setFoldComments(bool)" );
	cbFoldComments->setVisible( i != -1 );
	if ( cbFoldComments->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldComments", Q_RETURN_ARG( bool, b ) );
		cbFoldComments->setChecked( ( qobject_cast<QsciLexerCPP*>( l ) )->foldComments() );
		// cpp, bash, css, d, perl, pov, python, sql, vhdl
	}

	// fold compact
	i = mo->indexOfSlot( "setFoldCompact(bool)" );
	cbFoldCompact->setVisible( i != -1 );
	if ( cbFoldCompact->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldCompact", Q_RETURN_ARG( bool, b ) );
		cbFoldCompact->setChecked( ( qobject_cast<QsciLexerCPP*>( l ) )->foldCompact() );
		// cpp, bash, css, d, html, lua, perl,pov, properties, sql, vhdl
	}

	// fold quotes
	i = mo->indexOfSlot( "setFoldQuotes(bool)" );
	cbFoldQuotes->setVisible( i != -1 );
	if ( cbFoldQuotes->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldQuotes", Q_RETURN_ARG( bool, b ) );
		cbFoldQuotes->setChecked( ( qobject_cast<QsciLexerPython*>( l ) )->foldQuotes() );
	}

	// fold directives
	i = mo->indexOfSlot( "setFoldDirectives(bool)" );
	cbFoldDirectives->setVisible( i != -1 );
	if ( cbFoldDirectives->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldDirectives", Q_RETURN_ARG( bool, b ) );
		cbFoldDirectives->setChecked( ( qobject_cast<QsciLexerPOV*>( l ) )->foldDirectives() );
	}

	// fold at begin
	i = mo->indexOfSlot( "setFoldAtBegin(bool)" );
	cbFoldAtBegin->setVisible( i != -1 );
	if ( cbFoldAtBegin->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldAtBegin", Q_RETURN_ARG( bool, b ) );
		cbFoldAtBegin->setChecked( ( qobject_cast<QsciLexerVHDL*>( l ) )->foldAtBegin() );
	}

	// fold at parenthesis
	i = mo->indexOfSlot( "setFoldAtParenthesis(bool)" );
	cbFoldAtParenthesis->setVisible( i != -1 );
	if ( cbFoldAtParenthesis->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldAtParenthesis", Q_RETURN_ARG( bool, b ) );
		cbFoldAtParenthesis->setChecked( ( qobject_cast<QsciLexerVHDL*>( l ) )->foldAtParenthesis() );
	}

	// fold at else
	i = mo->indexOfSlot( "setFoldAtElse(bool)" );
	cbFoldAtElse->setVisible( i != -1 );
	if ( cbFoldAtElse->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldAtElse", Q_RETURN_ARG( bool, b ) );
		cbFoldAtElse->setChecked( ( qobject_cast<QsciLexerCPP*>( l ) )->foldAtElse() );
		// cpp, vhdl, cmake, d
	}

	// fold preprocessor
	i = mo->indexOfSlot( "setFoldPreprocessor(bool)" );
	cbFoldPreprocessor->setVisible( i != -1 );
	if ( cbFoldPreprocessor->isVisible() )
	{
		QMetaObject::invokeMethod( l, "foldPreprocessor", Q_RETURN_ARG( bool, b ) );
		cbFoldPreprocessor->setChecked( ( qobject_cast<QsciLexerCPP*>( l ) )->foldPreprocessor() );
		// html, cpp
	}

	// style preprocessor
	i = mo->indexOfSlot( "setStylePreprocessor(bool)" );
	cbStylePreprocessor->setVisible( i != -1 );
	if ( cbStylePreprocessor->isVisible() )
	{
		QMetaObject::invokeMethod( l, "stylePreprocessor", Q_RETURN_ARG( bool, b ) );
		cbStylePreprocessor->setChecked( ( qobject_cast<QsciLexerCPP*>( l ) )->stylePreprocessor() );
	}

	// indent opening brace
	cbIndentOpeningBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiOpening );

	// indent closing brace
	cbIndentClosingBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiClosing );

	// case sensitive tags
	i = mo->indexOfSlot( "setCaseSensitiveTags(bool)" );
	cbCaseSensitiveTags->setVisible( i != -1 );
	if ( cbCaseSensitiveTags->isVisible() )
	{
		QMetaObject::invokeMethod( l, "caseSensitiveTags", Q_RETURN_ARG( bool, b ) );
		cbCaseSensitiveTags->setChecked( ( qobject_cast<QsciLexerHTML*>( l ) )->caseSensitiveTags() );
	}

	// backslash escapes
	i = mo->indexOfSlot( "setBackslashEscapes(bool)" );
	cbBackslashEscapes->setVisible( i != -1 );
	if ( cbBackslashEscapes->isVisible() )
	{
		QMetaObject::invokeMethod( l, "backslashEscapes", Q_RETURN_ARG( bool, b ) );
		cbBackslashEscapes->setChecked( ( qobject_cast<QsciLexerSQL*>( l ) )->backslashEscapes() );
	}

	// indentation warning
	i = mo->indexOfSlot( "setIndentationWarning(IndentationWarning)" );
	lIndentationWarning->setVisible( i != -1 );
	cbIndentationWarning->setVisible( lIndentationWarning->isVisible() );
	if ( lIndentationWarning->isVisible() )
	{
		QsciLexerPython* p = qobject_cast<QsciLexerPython*>( l );
		cbIndentationWarning->setCurrentIndex( cbIndentationWarning->findData( p->indentationWarning() ) );
	}
}
