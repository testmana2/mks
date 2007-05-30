#include "UISettings.h"
#include "Settings.h"
//
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
//
#include <QButtonGroup>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextCodec>
#include <QMetaProperty>
//
const QString SettingsPath = "Settings";
//
QPointer<UISettings> UISettings::mSelf = 0L;
//
UISettings* UISettings::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UISettings( p );
	return mSelf;
}
//
UISettings::UISettings( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// designer
	bgDesigner = new QButtonGroup( gbUIDesignerIntegration );
	bgDesigner->addButton( rbUseEmbeddedUIDesigner, Embedded );
	bgDesigner->addButton( rbRunQtDesigner, External );
	// externalchanges
	bgExternalChanges = new QButtonGroup( gbOnExternalChanges );
	bgExternalChanges->addButton( rbDoNothing, Nothing );
	bgExternalChanges->addButton( rbAlertUser, Alert );
	bgExternalChanges->addButton( rbReloadAutomatically, Reload );
	// api source
	bgAPISource = new QButtonGroup( gbAutoCompletionSource );
	bgAPISource->addButton( rbAutoCompletionDocument, QsciScintilla::AcsDocument );
	bgAPISource->addButton( rbAutoCompletionAPI, QsciScintilla::AcsAPIs );
	bgAPISource->addButton( rbAutoCompletionAll, QsciScintilla::AcsAll );
	// loads text codecs
	QStringList l;
	foreach ( QByteArray a, QTextCodec::availableCodecs() )
		l << a;
	l.sort();
	cbGeneralEncoding->addItems( l );
	// resize column
	twHighlighterAssociations->setColumnWidth( 0, 200 );
	// init lexers
	initLexers();
	// eol mode
	bgEOLMode = new QButtonGroup( gbStyleLineEndCharacters );
	bgEOLMode->addButton( rbStyleUnix, QsciScintilla::EolUnix );
	bgEOLMode->addButton( rbStyleMacintosh, QsciScintilla::EolMac );
	bgEOLMode->addButton( rbStyleWindowsDOS, QsciScintilla::EolWindows );
	// fold style
	cbStyleFoldingStyle->addItem( tr( "Disabled" ), QsciScintilla::NoFoldStyle );
	cbStyleFoldingStyle->addItem( tr( "Plain" ), QsciScintilla::PlainFoldStyle );
	cbStyleFoldingStyle->addItem( tr( "Circled" ), QsciScintilla::CircledFoldStyle );
	cbStyleFoldingStyle->addItem( tr( "Boxed" ), QsciScintilla::BoxedFoldStyle );
	cbStyleFoldingStyle->addItem( tr( "Circled Tree" ), QsciScintilla::CircledTreeFoldStyle );
	cbStyleFoldingStyle->addItem( tr( "Boxed Tree" ), QsciScintilla::BoxedTreeFoldStyle );
	// edge mode
	cbStyleEdgeMode->addItem( tr( "Disabled" ), QsciScintilla::EdgeNone );
	cbStyleEdgeMode->addItem( tr( "Draw Line" ), QsciScintilla::EdgeLine );
	cbStyleEdgeMode->addItem( tr( "Change Background Color" ), QsciScintilla::EdgeBackground );
	// read settings
	loadSettings();
	// connections
	foreach ( QToolButton* tb, wEditorColours->findChildren<QToolButton*>() )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
}
//
void UISettings::loadSettings()
{
	Settings* s = Settings::current();
	QString sp;
	// general
	cbLoadLastProject->setChecked( s->value( "Settings/General/LoadLastProject", true ).toBool() );	
	leDefaultProjectsDirectory->setText( s->value( "Settings/General/DefaultProjectsDirectory", "%HOME/Projects%" ).toString() );
	bgDesigner->button( s->value( "Settings/General/Designer", Embedded ).toInt() )->setChecked( true );
	// user interface
	bgExternalChanges->button( s->value( "Settings/UserInterface/ExternalChanges", Alert ).toInt() )->setChecked( true );
	// File Templates
	// Editor
	//	APIs
	for ( int i = 0; i < cbAPIsLanguages->count(); i++ )
		cbAPIsLanguages->setItemData( i, s->value( QString( "%1/Editor/APIs/%2" ).arg( SettingsPath, cbAPIsLanguages->itemText( i ) ) ).toStringList() );
	if ( cbAPIsLanguages->count() > 0 )
		cbAPIsLanguages->setCurrentIndex( 0 );
	//  Auto Completion
	sp = QString( "%1/Editor/AutoCompletion" ).arg( SettingsPath );
	gbAutoCompletion->setEnabled( s->value( sp +"/Enabled", true ).toBool() );
	cbAutoCompletionCaseSensitive->setChecked( s->value( sp +"/CaseSensitive", true ).toBool() );
	cbAutoCompletionShowSingle->setChecked( s->value( sp +"/ShowSingle", false ).toBool() );
	cbAutoCompletionReplaceWord->setChecked( s->value( sp +"/ReplaceWord", false ).toBool() );
	sAutoCompletionThreshold->setValue( s->value( sp +"/Threshold", 2 ).toInt() );
	bgAPISource->button( s->value( sp +"/APISource", QsciScintilla::AcsAPIs ).toInt() )->setChecked( true );
	//  Calltips
	sp = QString( "%1/Editor/Calltips" ).arg( SettingsPath );
	gbAutoCompletionCalltips->setEnabled( s->value( sp +"/Enabled", true ).toBool() );
	sAutoCompletionCalltips->setValue( s->value( sp +"/Visible", 4 ).toInt() );
	//  Colours
	sp = QString( "%1/Editor/Colours" ).arg( SettingsPath );
	tbColoursCurrentLineMarker->setIcon( colourizedPixmap( s->value( sp +"/CurrentLineMarker", Qt::yellow ).value<QColor>() ) );
	tbColoursErrorLineMarker->setIcon( colourizedPixmap( s->value( sp +"/ErrorLineMarker", Qt::red ).value<QColor>() ) );
	tbColoursMatchedBraces->setIcon( colourizedPixmap( s->value( sp +"/MatchedBraces", Qt::blue ).value<QColor>() ) );
	tbColoursMatchedBracesBackground->setIcon( colourizedPixmap( s->value( sp +"/MatchedBracesBackground", Qt::white ).value<QColor>() ) );
	tbColoursCalltipsBackground->setIcon( colourizedPixmap( s->value( sp +"/CalltipsBackground", Qt::white ).value<QColor>() ) );
	tbColoursCaretForeground->setIcon( colourizedPixmap( s->value( sp +"/CaretForeground", Qt::black ).value<QColor>() ) );
	tbColoursCaretLineBackground->setIcon( colourizedPixmap( s->value( sp +"/CaretLineBackground", Qt::white ).value<QColor>() ) );
	tbColoursUnmatchedBraces->setIcon( colourizedPixmap( s->value( sp +"/UnmatchedBraces", Qt::red ).value<QColor>() ) );
	tbColoursUnmatchedBracesBackground->setIcon( colourizedPixmap( s->value( sp +"/UnmatchedBracesBackground", Qt::white ).value<QColor>() ) );
	//  General
	sp = QString( "%1/Editor/General" ).arg( SettingsPath );
	sGeneralAutosaveInterval->setValue( s->value( sp +"/AutoSaveInterval", 0 ).toInt() );
	sGeneralTabWidth->setValue( s->value( sp +"/TabWidth", 4 ).toInt() );
	sGeneralIndentationWidth->setValue( s->value( sp +"/IndentationWidth", 1 ).toInt() );
	sGeneralLineNumbersWidth->setValue( s->value( sp +"/LineNumbersWidth", 4 ).toInt() );
	cbGeneralShowLineNumbersMargin->setChecked( s->value( sp +"/ShowLineNumbersMargin", true ).toBool() );
	cbGeneralShowFoldMargin->setChecked( s->value( sp +"/ShowFoldMargin", true ).toBool() );
	cbGeneralShowWhitespace->setChecked( s->value( sp +"/ShowWhitespace", false ).toBool() );
	cbGeneralShowEndOfLine->setChecked( s->value( sp +"/ShowEndOfLine", false ).toBool() );
	cbGeneralShowIndentationGuides->setChecked( s->value( sp +"/ShowIndentationGuides", true ).toBool() );
	cbGeneralHighlightBraces->setChecked( s->value( sp +"/HighlightBraces", true ).toBool() );
	cbGeneralAutomaticSyntaxCheck->setChecked( s->value( sp +"/AutomaticSyntaxCheck", true ).toBool() );
	cbGeneralWrapLongLines->setChecked( s->value( sp +"/WrapLongLines", true ).toBool() );
	cbGeneralUseTabsForIndentations->setChecked( s->value( sp +"/UseTabsForIndentations", false ).toBool() );
	cbGeneralConvertTabsUponOpen->setChecked( s->value( sp +"/ConvertTabsUponOpen", false ).toBool() );
	cbGeneralTabKeyIndents->setChecked( s->value( sp +"/TabKeyIndents", true ).toBool() );
	cbGeneralAutoIndentation->setChecked( s->value( sp +"/AutoIndentation", true ).toBool() );
	cbGeneralAutomaticEndOfLineConversion->setChecked( s->value( sp +"/AutomaticEndOfLineConversion", true ).toBool() );
	cbGeneralCreateBackupFileUponOpen->setChecked( s->value( sp +"/CreateBackupFileUponOpen", true ).toBool() );
	cbGeneralEncoding->setCurrentIndex( cbGeneralEncoding->findText( s->value( sp +"/DefaultEncoding", "UTF-8" ).toString() ) );
	//  Highlighter Association
	sp = QString( "%1/Editor/HighlighterAssociation" ).arg( SettingsPath );
	s->beginGroup( sp );
	foreach ( QString l, s->childKeys() )
	{
		QTreeWidgetItem* it = new QTreeWidgetItem( twHighlighterAssociations );
		it->setText( 0, l );
		it->setText( 1, s->value( l ).toString() );
	}
	s->endGroup();
	//  SyntaxHighlighting
	if ( cbSyntaxHighlightingLexerLanguage->count() )
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( cbSyntaxHighlightingLexerLanguage->itemText( 0 ) );
	//  Properties
	sp = QString( "%1/Editor/Properties" ).arg( SettingsPath );
	cbPropertiesCppFoldComments->setChecked( s->value( sp +"/CppFoldComments", true ).toBool() );
	cbPropertiesCppFoldAtElse->setChecked( s->value( sp +"/CppFoldAtElse", false ).toBool() );
	cbPropertiesCppFoldPreprocessorDirectives->setChecked( s->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
	cbPropertiesCppIndentOpeningBrace->setChecked( s->value( sp +"/CppIndentOpeningBrace", false ).toBool() );
	cbPropertiesCppIndentClosingBrace->setChecked( s->value( sp +"/CppIndentClosingBrace", false ).toBool() );
	cbPropertiesHTMLFoldPreprocessorDirectives->setChecked( s->value( sp +"/HTMLFoldPreprocessorDirectives", false ).toBool() );
	cbPropertiesHTMLCaseSensitivesTags->setChecked( s->value( sp +"/HTMLCaseSensitivesTags", false ).toBool() );
	cbPropertiesPythonFoldComments->setChecked( s->value( sp +"/PythonFoldComments", true ).toBool() );
	cbPropertiesPythonHighlightBadIndentation->setChecked( s->value( sp +"/PythonHighlightBadIndentation", true ).toBool() );
	cbPropertiesPythonFoldStrings->setChecked( s->value( sp +"/PythonFoldStrings", true ).toBool() );
	cbPropertiesPythonAutoIndentationAfter->setChecked( s->value( sp +"/PythonAutoIndentationAfter", false ).toBool() );
	cbPropertiesSQLFoldComments->setChecked( s->value( sp +"/SQLFoldComments", true ).toBool() );
	cbPropertiesSQLBackslashEscapes->setChecked( s->value( sp +"/SQLBackslashEscapes", false ).toBool() );
	cbPropertiesBashFoldComments->setChecked( s->value( sp +"/BashFoldComments", true ).toBool() );
	cbPropertiesCSSFoldComments->setChecked( s->value( sp +"/CSSFoldComments", true ).toBool() );
	cbPropertiesPerlFoldComments->setChecked( s->value( sp +"/PerlFoldComments", true ).toBool() );
	cbPropertiesRubyHighlightBadIndentation->setChecked( s->value( sp +"/RubyHighlightBadIndentation", true ).toBool() );
	cbPropertiesAllFoldCompact->setChecked( s->value( sp +"/AllFoldCompact", true ).toBool() );
	//  Style
	sp = QString( "%1/Editor/Style" ).arg( SettingsPath );
	bgEOLMode->button( s->value( sp +"/EOLMode", QsciScintilla::EolWindows ).toInt() )->setChecked( true );
	cbStyleFoldingStyle->setCurrentIndex( cbStyleFoldingStyle->findData( s->value( sp +"/FoldingStyle", QsciScintilla::NoFoldStyle ).toInt() ) );
	QFont f;
	if ( f.fromString( s->value( sp +"/LineNumbersFont" ).toString() ) )
		leStyleLineNumbersFont->setFont( f );
	if ( f.fromString( s->value( sp +"/MonospacedFont" ).toString() ) )
		leStyleMonospacedFont->setFont( f );
	cbStyleUseMonospacedAsDefault->setChecked( s->value( sp +"/UseMonospacedAsDefault", false ).toBool() );
	cbStyleCaretLineVisible->setChecked( s->value( sp +"/CaretLineVisible", false ).toBool() );
	cbStyleColourizeSelectedText->setChecked( s->value( sp +"/ColourizeSelectedText", false ).toBool() );
	sbStyleCaretWidth->setValue( s->value( sp +"/CaretWidth", 1 ).toInt() );
	cbStyleEdgeMode->setCurrentIndex( cbStyleEdgeMode->findData( s->value( sp +"/EdgeMode", QsciScintilla::EdgeNone ).toInt() ) );
	// Abbreviations
	// Tools Menu
	//QString m = QString( "%1/menus/%2applications.menu" ).arg( QString( qgetenv( "XDG_CONFIG_DIRS" ) ) ).arg( QString( qgetenv( "XDG_MENU_PREFIX" ) ) );
	//qWarning( qPrintable( m ) );
}
//
void UISettings::initLexers()
{
	QsciLexer* l;
	l = new QsciLexerBash( this );
	mLexers[l->language()] = l;
	l = new QsciLexerBatch( this );
	mLexers[l->language()] = l;
	l = new QsciLexerCMake( this );
	mLexers[l->language()] = l;
	l = new QsciLexerCPP( this );
	mLexers[l->language()] = l;
	l = new QsciLexerCSharp( this );
	mLexers[l->language()] = l;
	l = new QsciLexerCSS( this );
	mLexers[l->language()] = l;
	l = new QsciLexerD( this );
	mLexers[l->language()] = l;
	l = new QsciLexerDiff( this );
	mLexers[l->language()] = l;
	l = new QsciLexerHTML( this );
	mLexers[l->language()] = l;
	l = new QsciLexerIDL( this );
	mLexers[l->language()] = l;
	l = new QsciLexerJava( this );
	mLexers[l->language()] = l;
	l = new QsciLexerJavaScript( this );
	mLexers[l->language()] = l;
	l = new QsciLexerLua( this );
	mLexers[l->language()] = l;
	l = new QsciLexerMakefile( this );
	mLexers[l->language()] = l;
	l = new QsciLexerPerl( this );
	mLexers[l->language()] = l;
	l = new QsciLexerPOV( this );
	mLexers[l->language()] = l;
	l = new QsciLexerProperties( this );
	mLexers[l->language()] = l;
	l = new QsciLexerPython( this );
	mLexers[l->language()] = l;
	l = new QsciLexerRuby( this );
	mLexers[l->language()] = l;
	l = new QsciLexerSQL( this );
	mLexers[l->language()] = l;
	l = new QsciLexerTeX( this );
	mLexers[l->language()] = l;
	l = new QsciLexerVHDL( this );
	mLexers[l->language()] = l;
	// fill combobox
	QStringList ll;
	foreach ( QsciLexer* sl, mLexers )
	{
		// get language name
		ll << sl->language();
		// load its settings
		sl->readSettings( *Settings::current(), QString( "%1/Editor/Scintilla" ).arg( SettingsPath ).toLocal8Bit() );
	}
	ll.sort();
	cbAPIsLanguages->addItems( ll );
	cbHighlighterAssociationLexerLanguage->addItems( ll );
	cbSyntaxHighlightingLexerLanguage->addItems( ll );
}
//
void UISettings::saveSettings()
{
	Settings* s = Settings::current();
	QString sp;
	// general
	sp = QString( "%1/Editor/AutoCompletion" ).arg( SettingsPath );
	s->setValue( sp +"/LoadLastProject", cbLoadLastProject->isChecked() );
	s->setValue( sp +"/DefaultProjectsDirectory", leDefaultProjectsDirectory->text() );
	s->setValue( sp +"/Designer", bgDesigner->checkedId() );
	// user interface
	sp = QString( "%1/Editor/UserInterface" ).arg( SettingsPath );
	s->setValue( sp +"/ExternalChanges", bgExternalChanges->checkedId() );
	// File Templates
	// Editor
	//	APIs
	sp = QString( "%1/Editor/APIs/" ).arg( SettingsPath );
	for ( int i = 0; i < cbAPIsLanguages->count(); i++ )
		s->setValue( sp +cbAPIsLanguages->itemText( i ), cbAPIsLanguages->itemData( i ).toStringList() );
	//  Auto Completion
	sp = QString( "%1/Editor/AutoCompletion" ).arg( SettingsPath );
	s->setValue( sp +"/Enabled", gbAutoCompletion->isEnabled() );
	s->setValue( sp +"/CaseSensitive", cbAutoCompletionCaseSensitive->isChecked() );
	s->setValue( sp +"/ShowSingle", cbAutoCompletionShowSingle->isChecked() );
	s->setValue( sp +"/ReplaceWord", cbAutoCompletionReplaceWord->isChecked() );
	s->setValue( sp +"/Threshold", sAutoCompletionThreshold->value() );
	s->setValue( sp +"/APISource", bgAPISource->checkedId() );
	//  Calltips
	sp = QString( "%1/Editor/Calltips" ).arg( SettingsPath );
	s->setValue( sp +"/Enabled", gbAutoCompletionCalltips->isEnabled() );
	s->setValue( sp +"/Visible", sAutoCompletionCalltips->value() );
	//  Colours
	sp = QString( "%1/Editor/Colours" ).arg( SettingsPath );
	s->setValue( sp +"/CurrentLineMarker", iconBackgroundColor( tbColoursCurrentLineMarker->icon() ) );
	s->setValue( sp +"/ErrorLineMarker", iconBackgroundColor( tbColoursErrorLineMarker->icon() ) );
	s->setValue( sp +"/MatchedBraces", iconBackgroundColor( tbColoursMatchedBraces->icon() ) );
	s->setValue( sp +"/MatchedBracesBackground", iconBackgroundColor( tbColoursMatchedBracesBackground->icon() ) );
	s->setValue( sp +"/CalltipsBackground", iconBackgroundColor( tbColoursCalltipsBackground->icon() ) );
	s->setValue( sp +"/CaretForeground", iconBackgroundColor( tbColoursCaretForeground->icon() ) );
	s->setValue( sp +"/CaretLineBackground", iconBackgroundColor( tbColoursCaretLineBackground->icon() ) );
	s->setValue( sp +"/UnmatchedBraces", iconBackgroundColor( tbColoursUnmatchedBraces->icon() ) );
	s->setValue( sp +"/UnmatchedBracesBackground", iconBackgroundColor( tbColoursUnmatchedBracesBackground->icon() ) );
	//  General
	sp = QString( "%1/Editor/General" ).arg( SettingsPath );
	s->setValue( sp +"/AutoSaveInterval", sGeneralAutosaveInterval->value() );
	s->setValue( sp +"/TabWidth", sGeneralTabWidth->value() );
	s->setValue( sp +"/IndentationWidth", sGeneralIndentationWidth->value() );
	s->setValue( sp +"/LineNumbersWidth", sGeneralLineNumbersWidth->value() );
	s->setValue( sp +"/ShowLineNumbersMargin", cbGeneralShowLineNumbersMargin->isChecked() );
	s->setValue( sp +"/ShowFoldMargin", cbGeneralShowFoldMargin->isChecked() );
	s->setValue( sp +"/ShowWhitespace", cbGeneralShowWhitespace->isChecked() );
	s->setValue( sp +"/ShowEndOfLine", cbGeneralShowEndOfLine->isChecked() );
	s->setValue( sp +"/ShowIndentationGuides", cbGeneralShowIndentationGuides->isChecked() );
	s->setValue( sp +"/HighlightBraces", cbGeneralHighlightBraces->isChecked() );
	s->setValue( sp +"/AutomaticSyntaxCheck", cbGeneralAutomaticSyntaxCheck->isChecked() );
	s->setValue( sp +"/WrapLongLines", cbGeneralWrapLongLines->isChecked() );
	s->setValue( sp +"/UseTabsForIndentations", cbGeneralUseTabsForIndentations->isChecked() );
	s->setValue( sp +"/ConvertTabsUponOpen", cbGeneralConvertTabsUponOpen->isChecked() );
	s->setValue( sp +"/TabKeyIndents", cbGeneralTabKeyIndents->isChecked() );
	s->setValue( sp +"/AutoIndentation", cbGeneralAutoIndentation->isChecked() );
	s->setValue( sp +"/AutomaticEndOfLineConversion", cbGeneralAutomaticEndOfLineConversion->isChecked() );
	s->setValue( sp +"/CreateBackupFileUponOpen", cbGeneralCreateBackupFileUponOpen->isChecked() );
	s->setValue( sp +"/DefaultEncoding", cbGeneralEncoding->currentText() );
	//  Highlighter Association
	sp = QString( "%1/Editor/HighlighterAssociation/" ).arg( SettingsPath );
	for ( int i = 0; i < twHighlighterAssociations->topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* it = twHighlighterAssociations->topLevelItem( i );
		s->setValue( sp +it->text( 0 ), it->text( 1 ) );
	}
	//  SyntaxHighlighting // TODO: fix me
	//  Properties
	sp = QString( "%1/Editor/Properties" ).arg( SettingsPath );
	s->setValue( sp +"/CppFoldComments", cbPropertiesCppFoldComments->isChecked() );
	s->setValue( sp +"/CppFoldAtElse", cbPropertiesCppFoldAtElse->isChecked() );
	s->setValue( sp +"/CppFoldPreprocessorDirectives", cbPropertiesCppFoldPreprocessorDirectives->isChecked() );
	s->setValue( sp +"/CppIndentOpeningBrace", cbPropertiesCppIndentOpeningBrace->isChecked() );
	s->setValue( sp +"/CppIndentClosingBrace", cbPropertiesCppIndentClosingBrace->isChecked() );
	s->setValue( sp +"/HTMLFoldPreprocessorDirectives", cbPropertiesHTMLFoldPreprocessorDirectives->isChecked() );
	s->setValue( sp +"/HTMLCaseSensitivesTags", cbPropertiesHTMLCaseSensitivesTags->isChecked() );
	s->setValue( sp +"/PythonFoldComments", cbPropertiesPythonFoldComments->isChecked() );
	s->setValue( sp +"/PythonHighlightBadIndentation", cbPropertiesPythonHighlightBadIndentation->isChecked() );
	s->setValue( sp +"/PythonFoldStrings", cbPropertiesPythonFoldStrings->isChecked() );
	s->setValue( sp +"/PythonAutoIndentationAfter", cbPropertiesPythonAutoIndentationAfter->isChecked() );
	s->setValue( sp +"/SQLFoldComments", cbPropertiesSQLFoldComments->isChecked() );
	s->setValue( sp +"/SQLBackslashEscapes", cbPropertiesSQLBackslashEscapes->isChecked() );
	s->setValue( sp +"/BashFoldComments", cbPropertiesBashFoldComments->isChecked() );
	s->setValue( sp +"/CSSFoldComments", cbPropertiesCSSFoldComments->isChecked() );
	s->setValue( sp +"/PerlFoldComments", cbPropertiesPerlFoldComments->isChecked() );
	s->setValue( sp +"/RubyHighlightBadIndentation", cbPropertiesRubyHighlightBadIndentation->isChecked() );
	s->setValue( sp +"/AllFoldCompact", cbPropertiesAllFoldCompact->isChecked() );
	//  Style
	sp = QString( "%1/Editor/Style" ).arg( SettingsPath );
	s->setValue( sp +"/EOLMode", bgEOLMode->checkedId() );
	s->setValue( sp +"/FoldingStyle", cbStyleFoldingStyle->itemData( cbStyleFoldingStyle->currentIndex() ).toInt() );
	s->setValue( sp +"/LineNumbersFont", leStyleLineNumbersFont->font().toString() );
	s->setValue( sp +"/MonospacedFont", leStyleMonospacedFont->font().toString() );
	s->setValue( sp +"/UseMonospacedAsDefault", cbStyleUseMonospacedAsDefault->isChecked() );
	s->setValue( sp +"/CaretLineVisible", cbStyleCaretLineVisible->isChecked() );
	s->setValue( sp +"/ColourizeSelectedText", cbStyleColourizeSelectedText->isChecked() );
	s->setValue( sp +"/CaretWidth", sbStyleCaretWidth->value() );
	s->setValue( sp +"/EdgeMode", cbStyleEdgeMode->itemData( cbStyleEdgeMode->currentIndex() ).toInt() );
	// Abbreviations
	// Tools Menu
	//QString m = QString( "%1/menus/%2applications.menu" ).arg( QString( qgetenv( "XDG_CONFIG_DIRS" ) ) ).arg( QString( qgetenv( "XDG_MENU_PREFIX" ) ) );
	//qWarning( qPrintable( m ) );
}
//
QPixmap UISettings::colourizedPixmap( const QColor& c ) const
{
	QPixmap p( 48, 16 );
	p.fill( c );
	return p;
}
//
QColor UISettings::iconBackgroundColor( const QIcon& i ) const
{
	return QColor( i.pixmap( QSize( 5, 5 ) ).toImage().pixel( QPoint( 0, 0 ) ) );
}
//
void UISettings::cbAPIsLanguages_beforeChanged( int i )
{
	if ( i == cbAPIsLanguages->currentIndex() )
	{
		QStringList l;
		for ( int j = 0; j < lwAPIs->count(); j++ )
			l << lwAPIs->item( j )->text();
		cbAPIsLanguages->setItemData( i, l );
	}
}
//
void UISettings::on_cbAPIsLanguages_currentIndexChanged( int i )
{
	lwAPIs->clear();
	lwAPIs->addItems( cbAPIsLanguages->itemData( i ).toStringList() );
}
//
void UISettings::on_pbAPIsDelete_clicked()
{
	QListWidgetItem* it = lwAPIs->currentItem();
	if ( it )
	{
		delete it;
		cbAPIsLanguages_beforeChanged( cbAPIsLanguages->currentIndex() );
	}
}
//
void UISettings::on_pbAPIsAdd_clicked()
{
	QString s = leAPIs->text();
	if ( !s.isEmpty() && lwAPIs->findItems( s, Qt::MatchFixedString ).count() == 0 )
	{
		lwAPIs->addItem( s );
		cbAPIsLanguages_beforeChanged( cbAPIsLanguages->currentIndex() );
	}
}
//
void UISettings::on_pbAPIsBrowse_clicked()
{
	QString s = QFileDialog::getOpenFileName( window(), tr( "Select API file" ), QString::null, tr( "API Files (*.api);;All Files (*)" ) );
	if ( !s.isNull() )
		leAPIs->setText( s );
}
//
void UISettings::tbColours_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	QColor c = QColorDialog::getColor( iconBackgroundColor( tb->icon() ), window() );
	if ( c.isValid() )
		tb->setIcon( colourizedPixmap( c ) );
}
//
void UISettings::on_twHighlighterAssociations_itemClicked( QTreeWidgetItem* it, int )
{
	if ( it )
	{
		leHighlighterAssociationFilenamePattern->setText( it->text( 0 ) );
		cbHighlighterAssociationLexerLanguage->setCurrentIndex( cbHighlighterAssociationLexerLanguage->findText( it->text( 1 ) ) );
	}
}
//
void UISettings::on_pbHighlighterAssociationAddChange_clicked()
{
	QString f = leHighlighterAssociationFilenamePattern->text();
	QString l = cbHighlighterAssociationLexerLanguage->currentText();
	if ( f.isEmpty() || l.isEmpty() )
		return;
	QTreeWidgetItem* it = twHighlighterAssociations->currentItem();
	if ( !it || it->text( 0 ) != f )
	{
		// check if item with same parameters already exists
		QList<QTreeWidgetItem*> l = twHighlighterAssociations->findItems( f, Qt::MatchFixedString );
		if ( l.count() )
			it = l.at( 0 );
		else
			it = new QTreeWidgetItem( twHighlighterAssociations );
	}
	it->setText( 0, f );
	it->setText( 1, l );
	twHighlighterAssociations->setCurrentItem( 0 );
	twHighlighterAssociations->selectionModel()->clear();
	leHighlighterAssociationFilenamePattern->clear();
	cbHighlighterAssociationLexerLanguage->setCurrentIndex( -1 );
}
//
void UISettings::on_pbHighlighterAssociationDelete_clicked()
{
	QTreeWidgetItem* it = twHighlighterAssociations->currentItem();
	if ( it )
	{
		delete it;
		twHighlighterAssociations->setCurrentItem( 0 );
		twHighlighterAssociations->selectionModel()->clear();
		leHighlighterAssociationFilenamePattern->clear();
		cbHighlighterAssociationLexerLanguage->setCurrentIndex( -1 );
	}
}
//
void UISettings::on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( const QString& s )
{
	QsciLexer* l = mLexers[s];
	lwSyntaxHighlightingStyleElements->clear();
	for ( int i = 0; i < 128; i++ )
	{
		QString n = l->description( i );
		if ( !n.isEmpty() )
		{
			QListWidgetItem* it = new QListWidgetItem( lwSyntaxHighlightingStyleElements );
			it->setText( n );
			it->setForeground( l->color( i ) );
			it->setBackground( l->paper( i ) );
			it->setFont( l->font( i ) );
			it->setData( Id, i );
			it->setData( EolFill, l->eolFill( i ) );
		}
	}
}
//
void UISettings::on_lwSyntaxHighlightingStyleElements_itemClicked( QListWidgetItem* it )
{
	if ( it )
	{
		//QsciLexer* l = mLexers[it->text()];
		//int i = it->data( Id ).toInt();
		QPalette p( leSyntaxHighlightingSampleText->palette() );
		p.setColor( QPalette::Base, it->background().color() );
		p.setColor( QPalette::Text, it->foreground().color() );
		leSyntaxHighlightingSampleText->setPalette( p );
		leSyntaxHighlightingSampleText->setFont( it->font() );
		cbSyntaxHighlightingFillToEndOfLine->setChecked( it->data( EolFill ).toBool() );
	}
}
//
void UISettings::on_pbSyntaxHighlightingForegroundColour_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		QColor c = QColorDialog::getColor( it->foreground().color(), window() );
		if ( c.isValid() )
		{
			it->setForeground( c );
			mLexers[cbSyntaxHighlightingLexerLanguage->currentText()]->setColor( c, it->data( Id ).toInt() );
			on_lwSyntaxHighlightingStyleElements_itemClicked( it );
		}
	}
}
//
void UISettings::on_pbSyntaxHighlightingBackgroundColour_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		QColor c = QColorDialog::getColor( it->background().color(), window() );
		if ( c.isValid() )
		{
			it->setBackground( c );
			mLexers[cbSyntaxHighlightingLexerLanguage->currentText()]->setPaper( c, it->data( Id ).toInt() );
			on_lwSyntaxHighlightingStyleElements_itemClicked( it );
		}
	}
}
//
void UISettings::on_pbSyntaxHighlightingFont_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		bool b;
		QFont f = QFontDialog::getFont( &b, it->font(), window() );
		if ( b )
		{
			it->setFont( f );
			mLexers[cbSyntaxHighlightingLexerLanguage->currentText()]->setFont( f, it->data( Id ).toInt());
			on_lwSyntaxHighlightingStyleElements_itemClicked( it );
		}
	}
}
//
void UISettings::on_cbSyntaxHighlightingFillToEndOfLine_clicked( bool b )
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		it->setData( EolFill, b );
		mLexers[cbSyntaxHighlightingLexerLanguage->currentText()]->setEolFill( b, it->data( Id ).toInt() );
	}
}
//
void UISettings::on_pbSyntaxHighlightingAllBackgroundColours_clicked()
{
	QsciLexer* l = mLexers[cbSyntaxHighlightingLexerLanguage->currentText()];
	QColor c = QColorDialog::getColor( l->defaultPaper(), window() );
	if ( c.isValid() )
	{
		l->setDefaultPaper( c );
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( l->language() );
	}
}
//
void UISettings::on_pbSyntaxHighlightingAllFonts_clicked()
{
	QsciLexer* l = mLexers[cbSyntaxHighlightingLexerLanguage->currentText()];
	bool b;
	QFont f = QFontDialog::getFont( &b, l->defaultFont(), window() );
	if ( b )
	{
		l->setDefaultFont( f );
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( l->language() );
	}
}
//
void UISettings::on_pbStyleLineNumbersFont_clicked()
{
	bool b;
	QFont f = QFontDialog::getFont( &b, leStyleLineNumbersFont->font(), window() );
	if ( b )
		leStyleLineNumbersFont->setFont( f );
}
//
void UISettings::on_pbStyleMonospacedFont_clicked()
{
	bool b;
	QFont f = QFontDialog::getFont( &b, leStyleMonospacedFont->font(), window() );
	if ( b )
		leStyleMonospacedFont->setFont( f );
}
//
void UISettings::accept()
{
	saveSettings();
	QDialog::accept();
}
