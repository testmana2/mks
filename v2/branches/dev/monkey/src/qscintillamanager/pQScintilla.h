#ifndef PQSCINTILLA_H
#define PQSCINTILLA_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include "qsciscintilla.h"
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

#include <QObject>
#include <QApplication>
#include <QHash>

class pEditor;

struct Q_MONKEY_EXPORT pAbbreviation
{
	pAbbreviation() {}
	pAbbreviation( const QString& t, const QString& d, const QString& l, const QString& c )
		: Template( t ), Description( d ), Language( l ), Code( c ) {}

	QString Template;
	QString Description;
	QString Language;
	QString Code;
};

class Q_MONKEY_EXPORT pQScintilla : public QObject, public QSingleton<pQScintilla>
{
	Q_OBJECT
	friend class QSingleton<pQScintilla>;

public:
	enum UIDesignerMode { uidmEmbedded = 0, uidmExternal };
	enum ExternalChangesMode { ecmNothing = 0, ecmAlert, ecmReload };

	QsciAPIs* apisForLexer( QsciLexer* );
	QHash<QString,QsciLexer*> lexersSettings();
	QsciLexer* lexerForLanguage( const QString& );
	void resetLexer( QsciLexer* );
	void writeLexersSettings();
	bool setLexerProperty( const QString&, QsciLexer*, const QVariant& );
	QVariant lexerProperty( const QString&, QsciLexer* ) const;
	void setEditorProperties( pEditor* );
	const QList<pAbbreviation> defaultAbbreviations();
	const QList<pAbbreviation> abbreviations();
	
	QStringList languages() const;
	QHash<QString, QStringList> defaultSuffixes() const;
	QHash<QString, QStringList> suffixes() const;
	QString filters() const;
	QsciLexer* lexerForFilename( const QString& );
	void applyProperties();
	void expandAbbreviation( pEditor* );
	/***** GENERAL *****/
	void setRestoreProjectsOnStartup( bool ); // true
	bool restoreProjectsOnStartup() const;
	void setDefaultProjectsDirectory( const QString& ); // %HOME%/.Monkey Studio/Projects
	QString defaultProjectsDirectory() const;
	void setUIDesignerMode( pQScintilla::UIDesignerMode ); // dmExternal
	pQScintilla::UIDesignerMode uiDesignerMode() const;
	void setExternalChanges( pQScintilla::ExternalChangesMode ); // ecmAlert
	pQScintilla::ExternalChangesMode externalchanges() const;
	void setSaveSessionOnClose( bool ); // true
	bool saveSessionOnClose() const;
	void setRestoreSessionOnStartup( bool ); // true
	bool restoreSessionOnStartup() const;
	/* TEMPLATES */
	/******	EDITOR ******/
	// General
	void setAutoSyntaxCheck( bool ); // false
	bool autoSyntaxCheck() const;
	void setConvertTabsUponOpen( bool ); // false
	bool convertTabsUponOpen() const;
	void setCreateBackupUponOpen( bool ); // false
	bool createBackupUponOpen() const;
	void setAutoEolConversion( bool ); // false
	bool autoEolConversion() const;
	void setDefaultEncoding( const QString& ); // UTF-8
	QString defaultEncoding() const;
	void setSelectionBackgroundColor( const QColor& ); // #444444FF
	QColor selectionBackgroundColor() const;
	void setSelectionForegroundColor( const QColor& ); // #444444FF
	QColor selectionForegroundColor() const;
	void setDefaultDocumentColours( bool ); // false
	bool defaultDocumentColours() const;
	void setDefaultDocumentPen( const QColor& ); // black
	QColor defaultDocumentPen() const;
	void setDefaultDocumentPaper( const QColor& ); // white
	QColor defaultDocumentPaper() const;
	// Auto Completion
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
	// CallTips
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
	void setBraceMatching( QsciScintilla::BraceMatch ); // NoBraceMatch
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
	void setLineNumbersMarginEnabled( bool ); // false
	bool lineNumbersMarginEnabled() const;
	void setLineNumbersMarginWidth( int ); // 4
	int lineNumbersMarginWidth() const;
	void setLineNumbersMarginAutoWidth( bool ); // false
	bool lineNumbersMarginAutoWidth() const;
	void setFolding( QsciScintilla::FoldStyle ); // NoFoldStyle
	QsciScintilla::FoldStyle folding() const;
	void setFoldMarginBackgroundColor( const QColor& ); // gray
	QColor foldMarginBackgroundColor() const;
	void setFoldMarginForegroundColor( const QColor& ); // black
	QColor foldMarginForegroundColor() const;
	void setMarginsEnabled( bool ); // false
	bool marginsEnabled() const;
	void setMarginsBackgroundColor( const QColor& ); // gray
	QColor marginsBackgroundColor() const;
	void setMarginsForegroundColor( const QColor& ); // black
	QColor marginsForegroundColor() const;
	void setMarginsFont( const QFont& );
	QFont marginsFont() const;
	// Special Characters
	void setEolMode( QsciScintilla::EolMode ); // platform
	QsciScintilla::EolMode eolMode() const;
	void setEolVisibility( bool ); // false
	bool eolVisibility() const;
	void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility ); // false
	QsciScintilla::WhitespaceVisibility whitespaceVisibility() const;
	void setWrapMode( QsciScintilla::WrapMode ); // WrapNone
	QsciScintilla::WrapMode wrapMode() const;
	void setWrapVisualFlagsEnabled( bool ); // false
	bool wrapVisualFlagsEnabled() const;
	void setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag ); // WrapFlagNone
	QsciScintilla::WrapVisualFlag startWrapVisualFlag() const;
	void setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag ); // WrapFlagNone
	QsciScintilla::WrapVisualFlag endWrapVisualFlag() const;
	void setWrappedLineIndentWidth( int );
	int wrappedLineIndentWidth() const;

private:
	pQScintilla( QObject* = QApplication::instance() );
	~pQScintilla();

	QHash<QString,QsciLexer*> mLexersSettings;
	QHash<QString,QsciLexer*> mGlobalsLexers;
	QHash<QString,QsciAPIs*> mGlobalsAPIs;
	QList<pAbbreviation> mAbbreviations;

};

#endif // PQSCINTILLA_H
