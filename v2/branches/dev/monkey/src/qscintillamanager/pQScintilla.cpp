#include "pQScintilla.h"
#include "pSettings.h"
#include "pEditor.h"
#include "pAbstractChild.h"
#include "pWorkspace.h"

#include <QFileInfo>

const QString mPath = "/Scintilla";

pQScintilla::pQScintilla( QObject* o )
	: QObject( o )
{}

pQScintilla::~pQScintilla()
{}

QHash<QString,QsciLexer*> pQScintilla::lexersSettings()
{
	// init lexers if needed
	if ( mLexersSettings.isEmpty() )
		foreach ( QString s, languages() )
			mLexersSettings[s] = lexerForLanguage( s );

	// return lexers
	return mLexersSettings;
}

QsciLexer* pQScintilla::lexerForLanguage( const QString& s )
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

	// read settings
	if ( l )
		l->readSettings( *pSettings::instance(), qPrintable( mPath ) );

	// return lexer
	return l;
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
	mLexersSettings[s] = lexerForLanguage( s );
}

void pQScintilla::writeLexersSettings()
{
	// write settings
	foreach ( QsciLexer* l, lexersSettings() )
		l->writeSettings( *pSettings::instance(), qPrintable( mPath ) );
}

bool pQScintilla::setLexerProperty( const QString& s, QsciLexer* l, const QVariant& v )
{
	// cancel no property, no lexer or f variant is not valid
	if ( s.trimmed().isEmpty() || !l || !v.isValid() )
		return false;

	if ( v.type() == QVariant::Bool )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( bool, v.toBool() ) );
	else if ( v.type() == QVariant::Int )
		return QMetaObject::invokeMethod( l, qPrintable( s ), Q_ARG( QsciLexerPython::IndentationWarning, (QsciLexerPython::IndentationWarning)v.toInt() ) );

	// return default value
	return false;
}

QVariant pQScintilla::lexerProperty( const QString& s, QsciLexer* l ) const
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

void pQScintilla::setProperties( pEditor* e )
{
	if ( !e )
		return;

	// apply settings from UISettings
	// General
	e->setSelectionBackgroundColor( selectionBackgroundColor() );
	e->setSelectionForegroundColor( selectionForegroundColor() );
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

const QList<pAbbreviation*> pQScintilla::defaultAbbreviations()
{
	QList<pAbbreviation*> l;
	pAbbreviation* a;

	// C++
	// classd
	a = new pAbbreviation();
	a->Template = "classd";
	a->Description = "class declaration";
	a->Language = "C++";
	a->Code = "class |\n{\npublic:\n};";
	l << a;
	// forb
	a = new pAbbreviation();
	a->Template = "forb";
	a->Description = "for statement";
	a->Language = "C++";
	a->Code = "for( |; ; )\n{\n}";
	l << a;
	// ifb
	a = new pAbbreviation();
	a->Template = "ifb";
	a->Description = "if statement";
	a->Language = "C++";
	a->Code = "if( | )\n{\n}";
	l << a;
	// ife
	a = new pAbbreviation();
	a->Template = "ife";
	a->Description = "if else statement";
	a->Language = "C++";
	a->Code = "if( | )\n{\n}\nelse\n{\n}";
	l << a;
	// pr
	a = new pAbbreviation();
	a->Template = "pr";
	a->Description = "private";
	a->Language = "C++";
	a->Code = "private|";
	l << a;
	// pro
	a = new pAbbreviation();
	a->Template = "pro";
	a->Description = "protected";
	a->Language = "C++";
	a->Code = "protected|";
	l << a;
	// pu
	a = new pAbbreviation();
	a->Template = "pu";
	a->Description = "public";
	a->Language = "C++";
	a->Code = "public|";
	l << a;
	// structd
	a = new pAbbreviation();
	a->Template = "structd";
	a->Description = "struct declaration";
	a->Language = "C++";
	a->Code = "struct |\n{\n};";
	l << a;
	// switchb
	a = new pAbbreviation();
	a->Template = "switchb";
	a->Description = "switch statement";
	a->Language = "C++";
	a->Code = "switch( | )\n{\n}";
	l << a;
	// whileb
	a = new pAbbreviation();
	a->Template = "whileb";
	a->Description = "while statement";
	a->Language = "C++";
	a->Code = "while( | )\n{\n}";
	l << a;

	// return abbreviations
	return l;
}

const QList<pAbbreviation*> pQScintilla::abbreviations()
{
	// get settings
	pSettings* s = pSettings::instance();

	// get abbreviation if needed
	if ( mAbbreviations.isEmpty() )
	{
		// read abbreviations from settings
		int size = s->beginReadArray( "Abbreviations" );
		for ( int i = 0; i < size; i++ )
		{
			s->setArrayIndex( i );
			pAbbreviation* a = new pAbbreviation();
			a->Template = s->value( "Template" ).toString();
			a->Description = s->value( "Description" ).toString();
			a->Language = s->value( "Language" ).toString();
			a->Code = s->value( "Code" ).toString();
			mAbbreviations << a;
		}
		s->endArray();

		// get default abbreviations if needed
		if ( mAbbreviations.isEmpty() )
			mAbbreviations << defaultAbbreviations();
	}

	// return abbreviations
	return mAbbreviations;
}

QStringList pQScintilla::languages() const
{
	return QStringList() << "Bash" << "Batch" << "C#" << "C++" << "CMake" << "CSS"
		<< "D" << "Diff" << "HTML" << "IDL" << "Java" << "JavaScript" << "Lua"
		<< "Makefile" << "POV" << "Perl" << "Properties" << "Python" << "Ruby"
		<< "SQL" << "TeX" << "VHDL";
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
	s->beginGroup( mPath +"/LexersAssociations" );
	foreach ( QString k, s->childKeys() )
		l[s->value( k ).toString()] << k;
	s->endGroup();

	if ( l.isEmpty() )
		l = defaultSuffixes();

	return l;
}

QString pQScintilla::filters() const
{
	QString f;

	// get suffixes
	QHash<QString, QStringList> sl = suffixes();
	
	foreach ( QString k, sl.keys() )
		f += QString( "%1 Files ( %2 );;" ).arg( k ).arg( sl.value( k ).join( " " ) );

	// remove trailing ;;
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );

	// return filters list
	return f;
}

QsciLexer* pQScintilla::lexerForFilename( const QString& s )
{
	// get suffixes
	QHash<QString, QStringList> l = suffixes();

	// file suffixe
	const QString sf = QFileInfo( s ).suffix().prepend( "*." );

	// basic setup need change for makefile and cmake
	foreach ( QString k, l.keys() )
	{
		if ( l.value( k ).contains( sf, Qt::CaseInsensitive ) )
		{
			// return existing global lexer
			if ( mGlobalsLexers.keys().contains( k ) )
				return mGlobalsLexers[k];
			// create global lexer, store and return it
			mGlobalsLexers[k] = lexerForLanguage( k );
			return mGlobalsLexers[k];
		}
	}

	// return no lexer if not found
	return 0;
}

void pQScintilla::applyProperties()
{
	// apply scintilla properties
	foreach ( pAbstractChild* c, pWorkspace::instance()->children() )
		foreach ( pEditor* p, c->findChildren<pEditor*>() )
			setProperties( p );

	// apply lexers properties
	foreach ( QsciLexer* l, mGlobalsLexers )
		l->readSettings( *pSettings::instance(), qPrintable( mPath ) );

	// clear mAbbreviations so a call to abbreviations() will refill it
	qDeleteAll( mAbbreviations );
	mAbbreviations.clear();
}

void pQScintilla::expandAbbreviation( pEditor* e )
{
	if ( !e || !e->lexer() )
		return;

	// get current cursor position
	const QPoint p = e->cursorPosition();

	// get word template
	QString t = e->text( p.y() ).left( p.x() );

	// calculate the index
	int i = t.lastIndexOf( " " );
	if ( i == -1 )
		i = t.lastIndexOf( "\t" );

	// get true word template
	t = t.mid( i ).trimmed();

	// get language
	const QString lng = e->lexer()->language();

	// look for abbreviation and lexer to replace
	foreach ( pAbbreviation* a, abbreviations() )
	{
		// if template is found for language
		if ( a->Language == lng && a->Template == t )
		{
			// select word template from document
			e->setSelection( p.y(), i +1, p.y(), i +1 +t.length() );

			// remove word template from document
			e->removeSelectedText();

			// for calculate final cursor position if it found a |
			QPoint op;
			int k;

			// get code lines
			QStringList l = a->Code.split( "\n" );
			int j = 0;
			// iterating code lines
			foreach ( QString s, l )
			{
				// looking for cursor position
				k = s.indexOf( "|" );

				// calculate cursor position
				if ( k != -1 )
				{
					op.ry() = p.y() +j;
					op.rx() = k +i +1;
					s.replace( "|", "" );
				}

				// if no last line
				if ( j < l.count() -1 )
				{
					// insert code line and an end of line
					e->insert( s +"\n" );

					// set cursor on next line
					e->setCursorPosition( p.y() +j +1, 0 );
				}
				// insert codel ine
				else
					e->insert( s );

				// process indentation for code line if line is not first one
				if ( j > 0 )
					e->setIndentation( p.y() +j, e->indentation( p.y() ) +e->indentation( p.y() +j ) );

				// increment j for calculate correct line
				j++;
			}

			// set new cursor position is needed
			if ( !op.isNull() )
				e->setCursorPosition( op.y(), op.x() );

			// finish
			return;
		}
	}
}

void pQScintilla::setRestoreProjectsOnStartup( bool b )
{
	pSettings::instance()->setValue( mPath +"/RestoreProjectsOnStartup", b );
}

bool pQScintilla::restoreProjectsOnStartup() const
{
	return pSettings::instance()->value( mPath +"/RestoreProjectsOnStartup", true ).toBool();
}

void pQScintilla::setDefaultProjectsDirectory( const QString& s )
{
	pSettings::instance()->setValue( mPath +"/DefaultProjectsDirectory", s );
}

QString pQScintilla::defaultProjectsDirectory() const
{
	return pSettings::instance()->value( mPath +"/DefaultProjectsDirectory", "%HOME%/.Monkey Studio/Projects" ).toString();
}

void pQScintilla::setUIDesignerMode( pQScintilla::UIDesignerMode d )
{
	pSettings::instance()->setValue( mPath +"/UIDesignerMode", d );
}

pQScintilla::UIDesignerMode pQScintilla::uiDesignerMode() const
{
	return (pQScintilla::UIDesignerMode)pSettings::instance()->value( mPath +"/UIDesignerMode", pQScintilla::uidmExternal ).toInt();
}

void pQScintilla::setExternalChanges( pQScintilla::ExternalChangesMode e )
{
	pSettings::instance()->setValue( mPath +"/ExternalChanges", e );
}

pQScintilla::ExternalChangesMode pQScintilla::externalchanges() const
{
	return (pQScintilla::ExternalChangesMode)pSettings::instance()->value( mPath +"/ExternalChanges", pQScintilla::ecmAlert ).toInt();
}

void pQScintilla::setSaveSessionOnClose( bool b )
{
	pSettings::instance()->setValue( mPath +"/SaveSessionOnClose", b );
}

bool pQScintilla::saveSessionOnClose() const
{
	return pSettings::instance()->value( mPath +"/SaveSessionOnClose", true ).toBool();
}

void pQScintilla::setRestoreSessionOnStartup( bool b )
{
	pSettings::instance()->setValue( mPath +"/RestoreSessionOnStartup", b );
}

bool pQScintilla::restoreSessionOnStartup() const
{
	return pSettings::instance()->value( mPath +"/RestoreSessionOnStartup", true ).toBool();
}

void pQScintilla::setAutoSyntaxCheck( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoSyntaxCheck", b );
}

bool pQScintilla::autoSyntaxCheck() const
{
	return pSettings::instance()->value( mPath +"/AutoSyntaxCheck", false ).toBool();
}

void pQScintilla::setConvertTabsUponOpen( bool b )
{
	pSettings::instance()->setValue( mPath +"/ConvertTabsUponOpen", b );
}

bool pQScintilla::convertTabsUponOpen() const
{
	return pSettings::instance()->value( mPath +"/ConvertTabsUponOpen", false ).toBool();
}

void pQScintilla::setCreateBackupUponOpen( bool b )
{
	pSettings::instance()->setValue( mPath +"/CreateBackupUponOpen", b );
}

bool pQScintilla::createBackupUponOpen() const
{
	return pSettings::instance()->value( mPath +"/CreateBackupUponOpen", false ).toBool();
}

void pQScintilla::setAutoEolConversion( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoEolConversion", b );
}

bool pQScintilla::autoEolConversion() const
{
	return pSettings::instance()->value( mPath +"/AutoEolConversion", false ).toBool();
}

void pQScintilla::setDefaultEncoding( const QString& s )
{
	pSettings::instance()->setValue( mPath +"/DefaultEncoding", s );
}

QString pQScintilla::defaultEncoding() const
{
	return pSettings::instance()->value( mPath +"/DefaultEncoding", "UTF-8" ).toString();
}

void pQScintilla::setSelectionBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/SelectionBackgroundColor", c );
}

QColor pQScintilla::selectionBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/SelectionBackgroundColor", QColor( "#444444" ) ).value<QColor>();
}

void pQScintilla::setSelectionForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/SelectionForegroundColor", c );
}

QColor pQScintilla::selectionForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/SelectionForegroundColor", QColor( Qt::transparent ) ).value<QColor>();
}

void pQScintilla::setDefaultDocumentColours( bool b )
{
	pSettings::instance()->setValue( mPath +"/DefaultDocumentColours", b );
}

bool pQScintilla::defaultDocumentColours() const
{
	return pSettings::instance()->value( mPath +"/DefaultDocumentColours", false ).toBool();
}

void pQScintilla::setDefaultDocumentPen( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/DefaultDocumentPen", c );
}

QColor pQScintilla::defaultDocumentPen() const
{
	return pSettings::instance()->value( mPath +"/DefaultDocumentPen", QColor( Qt::black ) ).value<QColor>();
}

void pQScintilla::setDefaultDocumentPaper( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/DefaultDocumentPaper", c );
}

QColor pQScintilla::defaultDocumentPaper() const
{
	return pSettings::instance()->value( mPath +"/DefaultDocumentPaper", QColor( Qt::white ) ).value<QColor>();
}

void pQScintilla::setAutoCompletionCaseSensitivity( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionCaseSensitivity", b );
}

bool pQScintilla::autoCompletionCaseSensitivity() const
{
	return pSettings::instance()->value( mPath +"/AutoCompletionCaseSensitivity", true ).toBool();
}

void pQScintilla::setAutoCompletionReplaceWord( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionReplaceWord", b );
}

bool pQScintilla::autoCompletionReplaceWord() const
{
	return pSettings::instance()->value( mPath +"/AutoCompletionReplaceWord", false ).toBool();
}

void pQScintilla::setAutoCompletionShowSingle( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionShowSingle", b );
}

bool pQScintilla::autoCompletionShowSingle() const
{
	return pSettings::instance()->value( mPath +"/AutoCompletionShowSingle", false ).toBool();
}

void pQScintilla::setAutoCompletionSource( QsciScintilla::AutoCompletionSource a )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionSource", a );
}

QsciScintilla::AutoCompletionSource pQScintilla::autoCompletionSource() const
{
	return (QsciScintilla::AutoCompletionSource)pSettings::instance()->value( mPath +"/AutoCompletionSource", (int)QsciScintilla::AcsNone ).toInt();
}

void pQScintilla::setAutoCompletionThreshold( int i )
{
	pSettings::instance()->setValue( mPath +"/AutoCompletionThreshold", i );
}

int pQScintilla::autoCompletionThreshold() const
{
	return pSettings::instance()->value( mPath +"/AutoCompletionThreshold", -1 ).toInt();
}

void pQScintilla::setCallTipsBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/CallTipsBackgroundColor", c );
}

QColor pQScintilla::callTipsBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/CallTipsBackgroundColor", Qt::white ).value<QColor>();
}

void pQScintilla::setCallTipsForegroundColor( const QColor& c ) // mid-gray
{
	pSettings::instance()->setValue( mPath +"/CallTipsForegroundColor", c );
}

QColor pQScintilla::callTipsForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/CallTipsForegroundColor", Qt::lightGray ).value<QColor>();
}

void pQScintilla::setCallTipsHighlightColor( const QColor& c ) // dark blue
{
	pSettings::instance()->setValue( mPath +"/CallTipsHighlightColor", c );
}

QColor pQScintilla::callTipsHighlightColor() const
{
	return pSettings::instance()->value( mPath +"/CallTipsHighlightColor", Qt::darkBlue ).value<QColor>();
}

void pQScintilla::setCallTipsStyle( QsciScintilla::CallTipsStyle s )
{
	pSettings::instance()->setValue( mPath +"/CallTipsStyle", s );
}

QsciScintilla::CallTipsStyle pQScintilla::callTipsStyle() const
{
	return (QsciScintilla::CallTipsStyle)pSettings::instance()->value( mPath +"/CallTipsStyle", (int)QsciScintilla::CallTipsNoContext ).toInt();
}

void pQScintilla::setCallTipsVisible( int i )
{
	pSettings::instance()->setValue( mPath +"/CallTipsVisible", i );
}

int pQScintilla::callTipsVisible() const
{
	return pSettings::instance()->value( mPath +"/CallTipsVisible", -1 ).toInt();
}

void pQScintilla::setAutoIndent( bool b )
{
	pSettings::instance()->setValue( mPath +"/AutoIndent", b );
}

bool pQScintilla::autoIndent() const
{
	return pSettings::instance()->value( mPath +"/AutoIndent", false ).toBool();
}

void pQScintilla::setBackspaceUnindents( bool b )
{
	pSettings::instance()->setValue( mPath +"/BackspaceUnindents", b );
}

bool pQScintilla::backspaceUnindents() const
{
	return pSettings::instance()->value( mPath +"/BackspaceUnindents", false ).toBool();
}

void pQScintilla::setIndentationGuides( bool b )
{
	pSettings::instance()->setValue( mPath +"/IndentationGuides", b );
}

bool pQScintilla::indentationGuides() const
{
	return pSettings::instance()->value( mPath +"/IndentationGuides", false ).toBool();
}

void pQScintilla::setIndentationsUseTabs( bool b )
{
	pSettings::instance()->setValue( mPath +"/IndentationsUseTabs", b );
}

bool pQScintilla::indentationsUseTabs() const
{
	return pSettings::instance()->value( mPath +"/IndentationsUseTabs", true ).toBool();
}

void pQScintilla::setIndentationWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/IndentationWidth", i );
}

int pQScintilla::indentationWidth() const
{
	return pSettings::instance()->value( mPath +"/IndentationWidth", 0 ).toInt();
}

void pQScintilla::setTabIndents( bool b )
{
	pSettings::instance()->setValue( mPath +"/TabIndents", b );
}

bool pQScintilla::tabIndents() const
{
	return pSettings::instance()->value( mPath +"/TabIndents", true ).toBool();
}

void pQScintilla::setTabWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/TabWidth", i );
}

int pQScintilla::tabWidth() const
{
	return pSettings::instance()->value( mPath +"/TabWidth", 8 ).toInt();
}

void pQScintilla::setIndentationGuidesBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/IndentationGuidesBackgroundColor", c );
}

QColor pQScintilla::indentationGuidesBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/IndentationGuidesBackgroundColor", Qt::white ).value<QColor>();
}

void pQScintilla::setIndentationGuidesForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/IndentationGuidesForegroundColor", c );
}

QColor pQScintilla::indentationGuidesForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/IndentationGuidesForegroundColor", Qt::black ).value<QColor>();
}

void pQScintilla::setBraceMatching( QsciScintilla::BraceMatch b )
{
	pSettings::instance()->setValue( mPath +"/BraceMatching", b );
}

QsciScintilla::BraceMatch pQScintilla::braceMatching() const
{
	return (QsciScintilla::BraceMatch)pSettings::instance()->value( mPath +"/BraceMatching", (int)QsciScintilla::NoBraceMatch ).toInt();
}

void pQScintilla::setMatchedBraceBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/MatchedBraceBackgroundColor", c );
}

QColor pQScintilla::matchedBraceBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/MatchedBraceBackgroundColor", Qt::white ).value<QColor>();
}

void pQScintilla::setMatchedBraceForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/MatchedBraceForegroundColor", c );
}

QColor pQScintilla::matchedBraceForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/MatchedBraceForegroundColor", Qt::red ).value<QColor>();
}

void pQScintilla::setUnmatchedBraceBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/UnmatchedBraceBackgroundColor", c );
}

QColor pQScintilla::unmatchedBraceBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/UnmatchedBraceBackgroundColor", Qt::white ).value<QColor>();
}

void pQScintilla::setUnmatchedBraceForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/UnmatchedBraceForegroundColor", c );
}

QColor pQScintilla::unmatchedBraceForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/UnmatchedBraceForegroundColor", Qt::blue ).value<QColor>();
}

void pQScintilla::setEdgeMode( QsciScintilla::EdgeMode m )
{
	pSettings::instance()->setValue( mPath +"/EdgeMode", m );
}

QsciScintilla::EdgeMode pQScintilla::edgeMode() const
{
	return (QsciScintilla::EdgeMode)pSettings::instance()->value( mPath +"/EdgeMode", (int)QsciScintilla::EdgeNone ).toInt();
}

void pQScintilla::setEdgeColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/EdgeColor", c );
}

QColor pQScintilla::edgeColor() const
{
	return pSettings::instance()->value( mPath +"/EdgeColor", Qt::red ).value<QColor>();
}

void pQScintilla::setEdgeColumn( int i )
{
	pSettings::instance()->setValue( mPath +"/EdgeColumn", i );
}

int pQScintilla::edgeColumn() const
{
	return pSettings::instance()->value( mPath +"/EdgeColumn", 80 ).toInt();
}


void pQScintilla::setCaretLineVisible( bool b )
{
	pSettings::instance()->setValue( mPath +"/CaretLineVisible", b );
}

bool pQScintilla::caretLineVisible() const
{
	return pSettings::instance()->value( mPath +"/CaretLineVisible", false ).toBool();
}

void pQScintilla::setCaretLineBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/CaretLineBackgroundColor", c );
}

QColor pQScintilla::caretLineBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/CaretLineBackgroundColor", QColor( Qt::transparent ) ).value<QColor>();
}

void pQScintilla::setCaretForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/CaretForegroundColor", c );
}

QColor pQScintilla::caretForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/CaretForegroundColor", Qt::black ).value<QColor>();
}

void pQScintilla::setCaretWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/CaretWidth", i );
}

int pQScintilla::caretWidth() const
{
	return pSettings::instance()->value( mPath +"/CaretWidth", 1 ).toInt();
}

void pQScintilla::setLineNumbersMarginEnabled( bool b )
{
	pSettings::instance()->setValue( mPath +"/LineNumbersMarginEnabled", b );
}

bool pQScintilla::lineNumbersMarginEnabled() const
{
	return pSettings::instance()->value( mPath +"/LineNumbersMarginEnabled", false ).toBool();
}

void pQScintilla::setLineNumbersMarginWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/LineNumbersMarginWidth", i );
}

int pQScintilla::lineNumbersMarginWidth() const
{
	return pSettings::instance()->value( mPath +"/LineNumbersMarginWidth", 4 ).toInt();
}

void pQScintilla::setLineNumbersMarginAutoWidth( bool b )
{
	pSettings::instance()->setValue( mPath +"/LineNumbersMarginAutoWidth", b );
}

bool pQScintilla::lineNumbersMarginAutoWidth() const
{
	return pSettings::instance()->value( mPath +"/LineNumbersMarginAutoWidth", false ).toBool();
}

void pQScintilla::setFolding( QsciScintilla::FoldStyle f )
{
	pSettings::instance()->setValue( mPath +"/Folding", f );
}

QsciScintilla::FoldStyle pQScintilla::folding() const
{
	return (QsciScintilla::FoldStyle)pSettings::instance()->value( mPath +"/Folding", (int)QsciScintilla::NoFoldStyle ).toInt();
}

void pQScintilla::setFoldMarginBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/FoldMarginBackgroundColor", c );
}

QColor pQScintilla::foldMarginBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/FoldMarginBackgroundColor", Qt::lightGray ).value<QColor>();
}

void pQScintilla::setFoldMarginForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/FoldMarginForegroundColor", c );
}

QColor pQScintilla::foldMarginForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/FoldMarginForegroundColor", Qt::black ).value<QColor>();
}

void pQScintilla::setMarginsEnabled( bool b )
{
	pSettings::instance()->setValue( mPath +"/MarginsEnabled", b );
}

bool pQScintilla::marginsEnabled() const
{
	return pSettings::instance()->value( mPath +"/MarginsEnabled", false ).toBool();
}

void pQScintilla::setMarginsBackgroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/MarginsBackgroundColor", c );
}

QColor pQScintilla::marginsBackgroundColor() const
{
	return pSettings::instance()->value( mPath +"/MarginsBackgroundColor", Qt::lightGray ).value<QColor>();
}

void pQScintilla::setMarginsForegroundColor( const QColor& c )
{
	pSettings::instance()->setValue( mPath +"/MarginsForegroundColor", c );
}

QColor pQScintilla::marginsForegroundColor() const
{
	return pSettings::instance()->value( mPath +"/MarginsForegroundColor", Qt::black ).value<QColor>();
}

void pQScintilla::setMarginsFont( const QFont& f )
{
	pSettings::instance()->setValue( mPath +"/MarginsFont", f.toString() );
}

QFont pQScintilla::marginsFont() const
{
	QFont f;
	f.fromString( pSettings::instance()->value( mPath +"/MarginsFont", f.toString() ).toString() );
	return f;
}

void pQScintilla::setEolMode( QsciScintilla::EolMode e )
{
	pSettings::instance()->setValue( mPath +"/EolMode", e );
}

QsciScintilla::EolMode pQScintilla::eolMode() const
{
#if defined( Q_WS_WIN )
	int i = QsciScintilla::EolWindows;
#elif defined( Q_WS_MAC )
	int i = QsciScintilla::EolMac;
#else
	int i = QsciScintilla::EolUnix;
#endif
	return (QsciScintilla::EolMode)pSettings::instance()->value( mPath +"/EolMode", i ).toInt();
}

void pQScintilla::setEolVisibility( bool b )
{
	pSettings::instance()->setValue( mPath +"/EolVisibility", b );
}

bool pQScintilla::eolVisibility() const
{
	return pSettings::instance()->value( mPath +"/EolVisibility", false ).toBool();
}

void pQScintilla::setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility w )
{
	pSettings::instance()->setValue( mPath +"/WhitespaceVisibility", w );
}

QsciScintilla::WhitespaceVisibility pQScintilla::whitespaceVisibility() const
{
	return (QsciScintilla::WhitespaceVisibility)pSettings::instance()->value( mPath +"/WhitespaceVisibility", (int)QsciScintilla::WsInvisible ).toInt();
}

void pQScintilla::setWrapMode( QsciScintilla::WrapMode w )
{
	pSettings::instance()->setValue( mPath +"/WrapMode", w );
}

QsciScintilla::WrapMode pQScintilla::wrapMode() const
{
	return (QsciScintilla::WrapMode)pSettings::instance()->value( mPath +"/WrapMode", (int)QsciScintilla::WrapNone ).toInt();
}

void pQScintilla::setWrapVisualFlagsEnabled( bool b )
{
	pSettings::instance()->setValue( mPath +"/WrapVisualFlagsEnabled", b );
}

bool pQScintilla::wrapVisualFlagsEnabled() const
{
	return pSettings::instance()->value( mPath +"/WrapVisualFlagsEnabled", false ).toBool();
}

void pQScintilla::setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag f )
{
	pSettings::instance()->setValue( mPath +"/StartWrapVisualFlag", f );
}

QsciScintilla::WrapVisualFlag pQScintilla::startWrapVisualFlag() const
{
	return (QsciScintilla::WrapVisualFlag)pSettings::instance()->value( mPath +"/StartWrapVisualFlag", (int)QsciScintilla::WrapFlagNone ).toInt();
}

void pQScintilla::setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag f )
{
	pSettings::instance()->setValue( mPath +"/EndWrapVisualFlag", f );
}

QsciScintilla::WrapVisualFlag pQScintilla::endWrapVisualFlag() const
{
	return (QsciScintilla::WrapVisualFlag)pSettings::instance()->value( mPath +"/EndWrapVisualFlag", (int)QsciScintilla::WrapFlagNone ).toInt();
}

void pQScintilla::setWrappedLineIndentWidth( int i )
{
	pSettings::instance()->setValue( mPath +"/WrappedLineIndentWidth", i );
}

int pQScintilla::wrappedLineIndentWidth() const
{
	return pSettings::instance()->value( mPath +"/WrappedLineIndentWidth", 0 ).toInt();
}
