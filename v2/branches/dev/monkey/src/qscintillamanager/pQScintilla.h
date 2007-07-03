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
	//
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


	void setAutoIndent( bool ); // false
	bool autoIndent() const;
	void setBraceMatching( QsciScintilla::BraceMatch ); // NoBraceMatching
	QsciScintilla::BraceMatch braceMatching() const;
	void setBackspaceUnindents( bool ); // false
	bool backspaceUnindents() const;
	void setEolMode( QsciScintilla::EolMode ); // platform
	QsciScintilla::EolMode eolMode() const;
	void setEolVisibility( bool ); // false
	bool eolVisibility() const;
	void setFolding( QsciScintilla::FoldStyle ); // NoFoldStyle
	QsciScintilla::FoldStyle folding() const;
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
	void setUtf8( bool ); // false
	bool isUtf8() const;
	void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility ); // false
	QsciScintilla::WhitespaceVisibility whitespaceVisibility() const;
	void setWrapMode( QsciScintilla::WrapMode ); // WrapNone
	QsciScintilla::WrapMode wrapMode() const;

	void setCallTipsBackgroundColor (const QColor &col)
	void setCallTipsForegroundColor (const QColor &col)
	void setCallTipsHighlightColor (const QColor &col)
	void setCallTipsStyle (CallTipsStyle style)
	void setCallTipsVisible (int nr)
/*
	void setFoldMarginColors (const QColor &fore, const QColor &back)
	void setAutoCompletionFillupsEnabled (bool enabled)
	void setAutoCompletionFillups (const char *fillups)
	void setAutoCompletionWordSeparators (const QStringList &separators)
	
	void setDocument (const QsciDocument &document)
	void setEdgeColor (const QColor &col)
	void setEdgeColumn (int colnr)
	void setEdgeMode (EdgeMode mode)
	void setMarkerBackgroundColor (const QColor &col, int mnr=-1)
	void setMarkerForegroundColor (const QColor &col, int mnr=-1)
	void setMatchedBraceBackgroundColor (const QColor &col)
	void setMatchedBraceForegroundColor (const QColor &col)
	void setUnmatchedBraceBackgroundColor (const QColor &col)
	void setUnmatchedBraceForegroundColor (const QColor &col)
	void setWrapVisualFlags (WrapVisualFlag eflag, WrapVisualFlag sflag=WrapFlagNone, int sindent=0)
*/
/*
	void setMarginLineNumbers( int, bool );
	bool marginLineNumbers( int ) const;
	void setMarginMarkerMask( int, int );
	int marginMarkerMask( int ) const;
	void setMarginSensitivity( int, bool );
	bool marginSensitivity( int ) const;
	void setMarginWidth( int, int );
	int marginWidth( int ) const;
	void setCaretForegroundColor( const QColor& );
	void setCaretLineBackgroundColor( const QColor& );
	void setCaretLineVisible( bool );
	void setCaretWidth( int width );
	void setIndentationGuidesBackgroundColor( const QColor &col )
	void setIndentationGuidesForegroundColor( const QColor &col )
	void setMarginsBackgroundColor( const QColor &col ) // gray
	void setMarginsFont( const QFont &f )
	void setMarginsForegroundColor( const QColor &col ) // black
	void setSelectionBackgroundColor( const QColor &col )
	void setSelectionForegroundColor( const QColor &col )
*/

private:
	pQScintilla( QObject* = QApplication::instance() );
	~pQScintilla();

	QHash<QString,QsciLexer*> mLexers;

};

#endif // PQSCINTILLA_H
