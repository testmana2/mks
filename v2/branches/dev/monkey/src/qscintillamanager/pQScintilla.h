#ifndef PQSCINTILLA_H
#define PQSCINTILLA_H

#include "MonkeyExport.h"
#include "QSingleton.h"

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

#include <QObject>
#include <QApplication>
#include <QHash>

class Q_MONKEY_EXPORT pQScintilla : public QObject, public QSingleton<pQScintilla>
{
	Q_OBJECT
	friend class QSingleton<pQScintilla>;

public:
	QHash<QString,QsciLexer*> lexers();
	QStringList languages() const;
	QsciLexer* lexer( const QString& );
	void readSettings();
	void writeSettings();
	void resetLexer( QsciLexer* );
	bool setProperty( const QString&, QsciLexer*, const QVariant& );
	QVariant property( const QString&, QsciLexer* ) const;
	QHash<QString, QStringList> defaultSuffixes() const;
	QHash<QString, QStringList> suffixes() const;
	QsciLexer* lexerForFilename( const QString& );
	// Auto Completion
	void setAutocompletionEnabled( bool ); // false
	bool autocompletionEnabled() const;
	void setAutoCompletionCaseSensitivity( bool ); // true
	bool autoCompletionCaseSensitivity() const;
	void setAutoCompletionReplaceWord( bool ); // false
	bool autoCompletionReplaceWord() const;
	void setAutoCompletionShowSingle( bool ); // false
	bool autoCompletionShowSingle() const;
	void setAutoCompletionSource( QsciScintilla::AutoCompletionSource ); // AcsNone
	QsciScintilla::AutoCompletionSource autoCompletionSource() const;
	void setAutoCompletionThreshold( int ); // -1
	int autoCompletionThreshold() const;
	// Calltips
	void setCallTipsEnabled( bool ); // false
	bool callTipsEnabled() const;
	void setCallTipsBackgroundColor( const QColor& ); // white
	QColor callTipsBackgroundColor() const;
	void setCallTipsForegroundColor( const QColor& ); // mid-gray
	QColor callTipsForegroundColor() const;
	void setCallTipsHighlightColor( const QColor& ); // dark blue
	QColor callTipsHighlightColor() const;
	void setCallTipsStyle( QsciScintilla::CallTipsStyle ); // CallTipsNoContext
	QsciScintilla::CallTipsStyle callTipsStyle() const;
	void setCallTipsVisible( int ); // -1
	int callTipsVisible() const;
	// Indentation
	void setAutoIndent( bool ); // false
	bool autoIndent() const;
	void setBackspaceUnindents( bool ); // false
	bool backspaceUnindents() const;
	void setIndentationGuides( bool ); // false
	bool indentationGuides() const;
	void setIndentationsUseTabs( bool ); // true
	bool indentationsUseTabs() const;
	void setIndentationWidth( int ); // 0
	int indentationWidth() const;
	void setTabIndents( bool ); // true
	bool tabIndents() const;
	void setTabWidth( int ); // 8
	int tabWidth() const;
	void setIndentationGuidesBackgroundColor( const QColor& ); // ?
	QColor indentationGuidesBackgroundColor() const;
	void setIndentationGuidesForegroundColor( const QColor& ); // ?
	QColor indentationGuidesForegroundColor() const;
	// Brace Matching
/*
  NoBraceMatch    Brace matching is disabled. 
  StrictBraceMatch    Brace matching is enabled for a brace immediately before the current position. 
  SloppyBraceMatch
*/
	void setBraceMatchingEnabled( bool ); // false
	bool braceMatchingEnabled() const;
	void setBraceMatching( QsciScintilla::BraceMatch ); // NoBraceMatching
	QsciScintilla::BraceMatch braceMatching() const;
	void setMatchedBraceBackgroundColor( const QColor& ); // white
	QColor matchedBraceBackgroundColor() const;
	void setMatchedBraceForegroundColor( const QColor& ); // red
	QColor matchedBraceForegroundColor() const;
	void setUnmatchedBraceBackgroundColor( const QColor& ); // white
	QColor unmatchedBraceBackgroundColor() const;
	void setUnmatchedBraceForegroundColor( const QColor& ); // blue
	QColor unmatchedBraceForegroundColor() const;
	// Edge Mode
	void setEdgeModeEnabled( bool ); // false
	bool edgeModeEnabled() const;
	void setEdgeMode( QsciScintilla::EdgeMode ); // EdgeNone
	QsciScintilla::EdgeMode edgeMode() const;
	void setEdgeColor( const QColor& ); // red
	QColor edgeColor() const;
	void setEdgeColumn( int ); // 80
	int edgeColumn() const;
	// Caret
	void setCaretLineVisible( bool ); // false
	bool caretLineVisible() const;
	void setCaretLineBackgroundColor( const QColor& ); // transparent
	QColor caretLineBackgroundColor() const;
	void setCaretForegroundColor( const QColor& ); // black
	QColor caretForegroundColor() const;
	void setCaretWidth( int ); // 1
	int caretWidth() const;
	// Margins
	void setFoldMarginEnabled( bool ); // false
	bool foldMarginEnabled() const;
	void setFolding( QsciScintilla::FoldStyle ); // NoFoldStyle
	QsciScintilla::FoldStyle folding() const;
	void setFoldMarginBackgroundColor( const QColor& ); // black
	QColor foldMarginBackgroundColor() const;
	void setFoldMarginForegroundColor( const QColor& ); // mid gray
	QColor foldMarginForegroundColor() const;
	void setGlobalMarginsEnabled( bool ); // false
	bool globalMarginsEnabled() const;
	void setMarginsBackgroundColor( const QColor& ); // gray
	QColor marginsBackgroundColor() const;
	void setMarginsForegroundColor( const QColor& ); // black
	QColor marginsForegroundColor() const;
	void setMarginsFont( const QFont& );
	QFont marginsFont() const;



	void setUtf8( bool ); // false
	bool isUtf8() const;
	void setEolMode( QsciScintilla::EolMode ); // platform
	QsciScintilla::EolMode eolMode() const;
	void setEolVisibility( bool ); // false
	bool eolVisibility() const;
	void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility ); // false
	QsciScintilla::WhitespaceVisibility whitespaceVisibility() const;
	void setWrapMode( QsciScintilla::WrapMode ); // WrapNone
	QsciScintilla::WrapMode wrapMode() const;
	//void setWrapVisualFlags (WrapVisualFlag eflag, WrapVisualFlag sflag=WrapFlagNone, int sindent=0)
/*
	void setSelectionBackgroundColor( const QColor &col )
	void setSelectionForegroundColor( const QColor &col )
*/
/*
	void setAutoCompletionFillupsEnabled (bool enabled) -
	void setAutoCompletionFillups (const char *fillups) -
	void setAutoCompletionWordSeparators (const QStringList &separators) -
	void setMarginLineNumbers( int, bool );-
	bool marginLineNumbers( int ) const;-
	void setMarginSensitivity( int, bool );-
	bool marginSensitivity( int ) const;-
	void setMarginWidth( int, int );-
	int marginWidth( int ) const;-
	void setMarginMarkerMask( int, int );-
	int marginMarkerMask( int ) const;-
	void setMarkerBackgroundColor (const QColor &col, int mnr=-1)-
	void setMarkerForegroundColor (const QColor &col, int mnr=-1)-
*/

private:
	pQScintilla( QObject* = QApplication::instance() );
	~pQScintilla();

	QHash<QString,QsciLexer*> mLexers;

};

#endif // PQSCINTILLA_H
