#include "UISettings.h"
#include "pSettings.h"
#include "pQScintilla.h"

#include <QButtonGroup>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextCodec>

const QString SettingsPath = "Settings";

UISettings::UISettings( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	twMenu->topLevelItem( 2 )->setExpanded( true );
	twMenu->setCurrentItem( twMenu->topLevelItem( 0 ) );
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

	// calltips style
	bgCallTipsStyle = new QButtonGroup( gbAutoCompletionCalltips );
	bgCallTipsStyle->addButton( rbAutoCompletionNoContext, QsciScintilla::CallTipsNoContext );
	bgCallTipsStyle->addButton( rbAutoCompletionNoAutoCompletionContext, QsciScintilla::CallTipsNoAutoCompletionContext );
	bgCallTipsStyle->addButton( rbAutoCompletionContext, QsciScintilla::CallTipsContext );

	// loads text codecs
	QStringList l;
	foreach ( QByteArray a, QTextCodec::availableCodecs() )
		l << a;
	l.sort();
	cbGeneralEncoding->addItems( l );

	// resize column
	twHighlighterAssociations->setColumnWidth( 0, 200 );

	// fill lexers combo
	l = pQScintilla::instance()->languages();
	l.sort();
	cbAPIsLanguages->addItems( l );
	cbHighlighterAssociationLexerLanguage->addItems( l );
	cbSyntaxHighlightingLexerLanguage->addItems( l );

	// python indentation warning
	cbIndentationWarning->addItem( tr( "No warning" ), QsciLexerPython::NoWarning );
	cbIndentationWarning->addItem( tr( "Inconsistent" ), QsciLexerPython::Inconsistent );
	cbIndentationWarning->addItem( tr( "Tabs after spaces" ), QsciLexerPython::TabsAfterSpaces );
	cbIndentationWarning->addItem( tr( "Spaces" ), QsciLexerPython::Spaces );
	cbIndentationWarning->addItem( tr( "Tabs" ), QsciLexerPython::Tabs );

	// eol mode
	bgEOLMode = new QButtonGroup( gbStyleLineEndCharacters );
	bgEOLMode->addButton( rbStyleUnix, QsciScintilla::EolUnix );
	bgEOLMode->addButton( rbStyleMacintosh, QsciScintilla::EolMac );
	bgEOLMode->addButton( rbStyleWindowsDOS, QsciScintilla::EolWindows );

	// fold style
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
	foreach ( QToolButton* tb, pEditorColours->findChildren<QToolButton*>() )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbStyleEdgeModeBackgroundColour, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	foreach ( QCheckBox* cb, gbSyntaxHighlightingStyleElement->findChildren<QCheckBox*>() )
		if ( cb != cbSyntaxHighlightingFillToEndOfLine )
			connect( cb, SIGNAL( toggled( bool ) ), this, SLOT( cbProperties_toggled( bool ) ) );
}

void UISettings::loadSettings()
{
	pSettings* s = pSettings::instance();
	QString sp;

	// general
	sp = QString( "%1/Editor/General" ).arg( SettingsPath );
	cbLoadLastProject->setChecked( s->value( sp +"/LoadLastProject", true ).toBool() );	
	leDefaultProjectsDirectory->setText( s->value( sp +"/DefaultProjectsDirectory", "%HOME%/Projects" ).toString() );
	bgDesigner->button( s->value( sp +"/Designer", Embedded ).toInt() )->setChecked( true );

	// user interface
	sp = QString( "%1/Editor/UserInterface" ).arg( SettingsPath );
	bgExternalChanges->button( s->value( sp +"/ExternalChanges", Alert ).toInt() )->setChecked( true );

	// File Templates

	// Editor
	//  Auto Completion
	sp = QString( "%1/Editor/AutoCompletion" ).arg( SettingsPath );
	gbAutoCompletion->setChecked( s->value( sp +"/Enabled", true ).toBool() );
	cbAutoCompletionCaseSensitive->setChecked( s->value( sp +"/CaseSensitive", true ).toBool() );
	cbAutoCompletionShowSingle->setChecked( s->value( sp +"/ShowSingle", false ).toBool() );
	cbAutoCompletionReplaceWord->setChecked( s->value( sp +"/ReplaceWord", false ).toBool() );
	sAutoCompletionThreshold->setValue( s->value( sp +"/Threshold", 0 ).toInt() );
	bgAPISource->button( s->value( sp +"/APISource", QsciScintilla::AcsAPIs ).toInt() )->setChecked( true );
	//  Calltips
	sp = QString( "%1/Editor/Calltips" ).arg( SettingsPath );
	gbAutoCompletionCalltips->setChecked( s->value( sp +"/Enabled", true ).toBool() );
	sAutoCompletionCalltips->setValue( s->value( sp +"/Visible", -1 ).toInt() );
	bgCallTipsStyle->button( s->value( sp +"/Style", QsciScintilla::CallTipsNoContext ).toInt() )->setChecked( true );
	//  APIs
	for ( int i = 0; i < cbAPIsLanguages->count(); i++ )
		cbAPIsLanguages->setItemData( i, s->value( QString( "%1/Editor/APIs/%2" ).arg( SettingsPath, cbAPIsLanguages->itemText( i ) ) ).toStringList() );
	if ( cbAPIsLanguages->count() > 0 )
		cbAPIsLanguages->setCurrentIndex( 0 );

	//  Colours
	sp = QString( "%1/Editor/Colours" ).arg( SettingsPath );
	tbColoursCurrentLineMarker->setIcon( colourizedPixmap( s->value( sp +"/CurrentLineMarker", Qt::yellow ).value<QColor>() ) );
	tbColoursErrorLineMarker->setIcon( colourizedPixmap( s->value( sp +"/ErrorLineMarker", Qt::red ).value<QColor>() ) );
	tbColoursMatchedBraces->setIcon( colourizedPixmap( s->value( sp +"/MatchedBraces", Qt::red ).value<QColor>() ) );
	tbColoursMatchedBracesBackground->setIcon( colourizedPixmap( s->value( sp +"/MatchedBracesBackground", Qt::white ).value<QColor>() ) );
	tbColoursCalltipsBackground->setIcon( colourizedPixmap( s->value( sp +"/CalltipsBackground", Qt::white ).value<QColor>() ) );
	tbColoursCalltipsForeground->setIcon( colourizedPixmap( s->value( sp +"/CalltipsForeground", Qt::lightGray ).value<QColor>() ) );
	tbColoursCalltipsHighlight->setIcon( colourizedPixmap( s->value( sp +"/CalltipsHighlight", Qt::darkBlue ).value<QColor>() ) );
	tbColoursCaretForeground->setIcon( colourizedPixmap( s->value( sp +"/CaretForeground", Qt::black ).value<QColor>() ) );
	tbColoursCaretLineBackground->setIcon( colourizedPixmap( s->value( sp +"/CaretLineBackground", Qt::white ).value<QColor>() ) );
	tbColoursUnmatchedBraces->setIcon( colourizedPixmap( s->value( sp +"/UnmatchedBraces", Qt::blue ).value<QColor>() ) );
	tbColoursUnmatchedBracesBackground->setIcon( colourizedPixmap( s->value( sp +"/UnmatchedBracesBackground", Qt::white ).value<QColor>() ) );

	//  General
	sp = QString( "%1/Editor/General" ).arg( SettingsPath );
	sGeneralAutosaveInterval->setValue( s->value( sp +"/AutoSaveInterval", 0 ).toInt() );
	sGeneralTabWidth->setValue( s->value( sp +"/TabWidth", 4 ).toInt() );
	sGeneralIndentationWidth->setValue( s->value( sp +"/IndentationWidth", 1 ).toInt() );
	sGeneralLineNumbersWidth->setValue( s->value( sp +"/LineNumbersWidth", 4 ).toInt() );
	//  Options
	cbGeneralShowLineNumbersMargin->setChecked( s->value( sp +"/ShowLineNumbersMargin", true ).toBool() );
	cbGeneralShowFoldMargin->setChecked( s->value( sp +"/ShowFoldMargin", true ).toBool() );
	cbGeneralShowWhitespace->setChecked( s->value( sp +"/ShowWhitespace", false ).toBool() );
	cbGeneralShowEndOfLine->setChecked( s->value( sp +"/ShowEndOfLine", false ).toBool() );
	cbGeneralShowIndentationGuides->setChecked( s->value( sp +"/ShowIndentationGuides", true ).toBool() );
	cbGeneralCreateBackupFileUponOpen->setChecked( s->value( sp +"/CreateBackupFileUponOpen", true ).toBool() );
	cbStyleCaretLineVisible->setChecked( s->value( sp +"/CaretLineVisible", false ).toBool() );
	cbGeneralAutomaticSyntaxCheck->setChecked( s->value( sp +"/AutomaticSyntaxCheck", true ).toBool() );
	cbGeneralUseTabsForIndentations->setChecked( s->value( sp +"/UseTabsForIndentations", false ).toBool() );
	cbGeneralConvertTabsUponOpen->setChecked( s->value( sp +"/ConvertTabsUponOpen", false ).toBool() );
	cbGeneralTabKeyIndents->setChecked( s->value( sp +"/TabKeyIndents", true ).toBool() );
	cbGeneralAutoIndentation->setChecked( s->value( sp +"/AutoIndentation", true ).toBool() );
	cbGeneralAutomaticEndOfLineConversion->setChecked( s->value( sp +"/AutomaticEndOfLineConversion", true ).toBool() );
	cbStyleColourizeSelectedText->setChecked( s->value( sp +"/ColourizeSelectedText", false ).toBool() );
	//  Edge Mode
	cbStyleEdgeMode->setCurrentIndex( cbStyleEdgeMode->findData( s->value( sp +"/EdgeMode", QsciScintilla::EdgeNone ).toInt() ) );
	sStyleEdgeModeColumnNumber->setValue( s->value( sp +"/EdgeModeColumnNumber", 80 ).toInt() );
	tbStyleEdgeModeBackgroundColour->setIcon( colourizedPixmap( s->value( sp +"/EdgeModeBackground", Qt::gray ).value<QColor>() ) );
	//  EOL Mode
	bgEOLMode->button( s->value( sp +"/EOLMode", QsciScintilla::EolWindows ).toInt() )->setChecked( true );
	//  File Encoding
	cbGeneralEncoding->setCurrentIndex( cbGeneralEncoding->findText( s->value( sp +"/DefaultEncoding", "UTF-8" ).toString() ) );
	//  Folding style
	cbStyleFoldingStyle->setCurrentIndex( cbStyleFoldingStyle->findData( s->value( sp +"/FoldingStyle", QsciScintilla::NoFoldStyle ).toInt() ) );
	//  Caret width
	sbStyleCaretWidth->setValue( s->value( sp +"/CaretWidth", 1 ).toInt() );
	//  LineNumbers & Monospaced Font
	QFont f;
	if ( f.fromString( s->value( sp +"/LineNumbersFont" ).toString() ) )
		leStyleLineNumbersFont->setFont( f );
	if ( f.fromString( s->value( sp +"/MonospacedFont" ).toString() ) )
		leStyleMonospacedFont->setFont( f );
	// default monospaced font
	cbStyleUseMonospacedAsDefault->setChecked( s->value( sp +"/UseMonospacedAsDefault", false ).toBool() );

	//  Associations
	sp = QString( "%1/Editor/Associations" ).arg( SettingsPath );
	s->beginGroup( sp );
	foreach ( QString l, s->childKeys() )
	{
		QTreeWidgetItem* it = new QTreeWidgetItem( twHighlighterAssociations );
		it->setText( 0, l );
		it->setText( 1, s->value( l ).toString() );
	}
	s->endGroup();

	//  Highlighting
	if ( cbSyntaxHighlightingLexerLanguage->count() )
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( cbSyntaxHighlightingLexerLanguage->itemText( 0 ) );

/*
	//  Properties
	sp = QString( "%1/Editor/Properties" ).arg( SettingsPath );
	cbPropertiesCppFoldComments->setChecked( s->value( sp +"/CppFoldComments", true ).toBool() );
	cbPropertiesCppFoldAtElse->setChecked( s->value( sp +"/CppFoldAtElse", false ).toBool() );
	cbPropertiesCppFoldPreprocessorDirectives->setChecked( s->value( sp +"/CppFoldPreprocessorDirectives", false ).toBool() );
	cbPropertiesCppStylePreprocessorDirectives->setChecked( s->value( sp +"/CppStylePreprocessorDirectives", false ).toBool() );
	cbPropertiesCppIndentOpeningBrace->setChecked( s->value( sp +"/CppIndentOpeningBrace", false ).toBool() );
	cbPropertiesCppIndentClosingBrace->setChecked( s->value( sp +"/CppIndentClosingBrace", false ).toBool() );
	cbPropertiesHTMLFoldPreprocessorDirectives->setChecked( s->value( sp +"/HTMLFoldPreprocessorDirectives", false ).toBool() );
	cbPropertiesHTMLCaseSensitivesTags->setChecked( s->value( sp +"/HTMLCaseSensitivesTags", false ).toBool() );
	cbPropertiesPythonFoldComments->setChecked( s->value( sp +"/PythonFoldComments", true ).toBool() );
	cbPropertiesPythonFoldStrings->setChecked( s->value( sp +"/PythonFoldStrings", true ).toBool() );
	cbPythonIndentationwarning->setCurrentIndex( cbPythonIndentationwarning->findData( s->value( sp +"/PythonIndentationwarning", QsciLexerPython::NoWarning ).toInt() ) );
	cbPropertiesSQLFoldComments->setChecked( s->value( sp +"/SQLFoldComments", true ).toBool() );
	cbPropertiesSQLBackslashEscapes->setChecked( s->value( sp +"/SQLBackslashEscapes", false ).toBool() );
	cbPropertiesBashFoldComments->setChecked( s->value( sp +"/BashFoldComments", true ).toBool() );
	cbPropertiesCSSFoldComments->setChecked( s->value( sp +"/CSSFoldComments", true ).toBool() );
	cbPropertiesPerlFoldComments->setChecked( s->value( sp +"/PerlFoldComments", true ).toBool() );
	cbPropertiesDFoldComments->setChecked( s->value( sp +"/DFoldComments", true ).toBool() );
	cbPropertiesDFoldAtElse->setChecked( s->value( sp +"/DFoldAtElse", false ).toBool() );
	cbPropertiesCMakeFoldAtElse->setChecked( s->value( sp +"/CMakeFoldAtElse", false ).toBool() );
	cbPropertiesPOVFoldComments->setChecked( s->value( sp +"/POVFoldComments", true ).toBool() );
	cbPropertiesPOVFoldPreprocessorDirectives->setChecked( s->value( sp +"/POVFoldPreprocessorDirectives", false ).toBool() );
	cbPropertiesVHDLFoldComments->setChecked( s->value( sp +"/VHDLFoldComments", true ).toBool() );
	cbPropertiesVHDLFoldAtElse->setChecked( s->value( sp +"/VHDLFoldAtElse", false ).toBool() );
	cbPropertiesVHDLFoldAtBegin->setChecked( s->value( sp +"/VHDLFoldAtBegin", false ).toBool() );
	cbPropertiesVHDLFoldAtParenthesis->setChecked( s->value( sp +"/VHDLFoldAtParenthesis", false ).toBool() );
	cbPropertiesAllFoldCompact->setChecked( s->value( sp +"/AllFoldCompact", true ).toBool() );

	// Abbreviations
	// Tools Menu
	//QString m = QString( "%1/menus/%2applications.menu" ).arg( QString( qgetenv( "XDG_CONFIG_DIRS" ) ) ).arg( QString( qgetenv( "XDG_MENU_PREFIX" ) ) );
	//qWarning( qPrintable( m ) );
*/
}

void UISettings::saveSettings()
{
	pSettings* s = pSettings::instance();
	QString sp;

	// general
	sp = QString( "%1/Editor/General" ).arg( SettingsPath );
	s->setValue( sp +"/LoadLastProject", cbLoadLastProject->isChecked() );
	s->setValue( sp +"/DefaultProjectsDirectory", leDefaultProjectsDirectory->text() );
	s->setValue( sp +"/Designer", bgDesigner->checkedId() );

	// user interface
	sp = QString( "%1/Editor/UserInterface" ).arg( SettingsPath );
	s->setValue( sp +"/ExternalChanges", bgExternalChanges->checkedId() );

	// File Templates

	// Editor
	//  Auto Completion
	sp = QString( "%1/Editor/AutoCompletion" ).arg( SettingsPath );
	s->setValue( sp +"/Enabled", gbAutoCompletion->isChecked() );
	s->setValue( sp +"/CaseSensitive", cbAutoCompletionCaseSensitive->isChecked() );
	s->setValue( sp +"/ShowSingle", cbAutoCompletionShowSingle->isChecked() );
	s->setValue( sp +"/ReplaceWord", cbAutoCompletionReplaceWord->isChecked() );
	s->setValue( sp +"/Threshold", sAutoCompletionThreshold->value() );
	s->setValue( sp +"/APISource", bgAPISource->checkedId() );
	//  Calltips
	sp = QString( "%1/Editor/Calltips" ).arg( SettingsPath );
	s->setValue( sp +"/Enabled", gbAutoCompletionCalltips->isChecked() );
	s->setValue( sp +"/Visible", sAutoCompletionCalltips->value() );
	s->setValue( sp +"/Style", bgCallTipsStyle->checkedId() );
	//  APIs
	sp = QString( "%1/Editor/APIs/" ).arg( SettingsPath );
	for ( int i = 0; i < cbAPIsLanguages->count(); i++ )
		s->setValue( sp +cbAPIsLanguages->itemText( i ), cbAPIsLanguages->itemData( i ).toStringList() );

	//  Colours
	sp = QString( "%1/Editor/Colours" ).arg( SettingsPath );
	s->setValue( sp +"/CurrentLineMarker", iconBackgroundColor( tbColoursCurrentLineMarker->icon() ) );
	s->setValue( sp +"/ErrorLineMarker", iconBackgroundColor( tbColoursErrorLineMarker->icon() ) );
	s->setValue( sp +"/MatchedBraces", iconBackgroundColor( tbColoursMatchedBraces->icon() ) );
	s->setValue( sp +"/MatchedBracesBackground", iconBackgroundColor( tbColoursMatchedBracesBackground->icon() ) );
	s->setValue( sp +"/CalltipsBackground", iconBackgroundColor( tbColoursCalltipsBackground->icon() ) );
	s->setValue( sp +"/CalltipsForeground", iconBackgroundColor( tbColoursCalltipsForeground->icon() ) );
	s->setValue( sp +"/CalltipsHighlight", iconBackgroundColor( tbColoursCalltipsHighlight->icon() ) );
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
	//  Options
	s->setValue( sp +"/ShowLineNumbersMargin", cbGeneralShowLineNumbersMargin->isChecked() );
	s->setValue( sp +"/ShowFoldMargin", cbGeneralShowFoldMargin->isChecked() );
	s->setValue( sp +"/ShowWhitespace", cbGeneralShowWhitespace->isChecked() );
	s->setValue( sp +"/ShowEndOfLine", cbGeneralShowEndOfLine->isChecked() );
	s->setValue( sp +"/ShowIndentationGuides", cbGeneralShowIndentationGuides->isChecked() );
	s->setValue( sp +"/CreateBackupFileUponOpen", cbGeneralCreateBackupFileUponOpen->isChecked() );
	s->setValue( sp +"/CaretLineVisible", cbStyleCaretLineVisible->isChecked() );
	s->setValue( sp +"/AutomaticSyntaxCheck", cbGeneralAutomaticSyntaxCheck->isChecked() );
	s->setValue( sp +"/UseTabsForIndentations", cbGeneralUseTabsForIndentations->isChecked() );
	s->setValue( sp +"/ConvertTabsUponOpen", cbGeneralConvertTabsUponOpen->isChecked() );
	s->setValue( sp +"/TabKeyIndents", cbGeneralTabKeyIndents->isChecked() );
	s->setValue( sp +"/AutoIndentation", cbGeneralAutoIndentation->isChecked() );
	s->setValue( sp +"/AutomaticEndOfLineConversion", cbGeneralAutomaticEndOfLineConversion->isChecked() );
	s->setValue( sp +"/ColourizeSelectedText", cbStyleColourizeSelectedText->isChecked() );
	//  Edge Mode
	s->setValue( sp +"/EdgeMode", cbStyleEdgeMode->itemData( cbStyleEdgeMode->currentIndex() ).toInt() );
	s->setValue( sp +"/EdgeModeColumnNumber", sStyleEdgeModeColumnNumber->value() );
	s->setValue( sp +"/EdgeModeBackground", iconBackgroundColor( tbStyleEdgeModeBackgroundColour->icon() ) );
	//  EOL Mode
	s->setValue( sp +"/EOLMode", bgEOLMode->checkedId() );
	//  File Encoding
	s->setValue( sp +"/DefaultEncoding", cbGeneralEncoding->currentText() );
	//  Folding Style
	s->setValue( sp +"/FoldingStyle", cbStyleFoldingStyle->itemData( cbStyleFoldingStyle->currentIndex() ).toInt() );
	//  Caret width
	s->setValue( sp +"/CaretWidth", sbStyleCaretWidth->value() );
	//  LineNumbers & Monospaced Font
	s->setValue( sp +"/LineNumbersFont", leStyleLineNumbersFont->font().toString() );
	s->setValue( sp +"/MonospacedFont", leStyleMonospacedFont->font().toString() );
	//  default monospaced font
	s->setValue( sp +"/UseMonospacedAsDefault", cbStyleUseMonospacedAsDefault->isChecked() );

	//  Associations
	sp = QString( "%1/Editor/Associations/" ).arg( SettingsPath );
	for ( int i = 0; i < twHighlighterAssociations->topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* it = twHighlighterAssociations->topLevelItem( i );
		s->setValue( sp +it->text( 0 ), it->text( 1 ) );
	}

	//  Highlighting
	pQScintilla::instance()->writeSettings();

/*
	//  Properties
	sp = QString( "%1/Editor/Properties" ).arg( SettingsPath );
	s->setValue( sp +"/CppFoldComments", cbPropertiesCppFoldComments->isChecked() );
	s->setValue( sp +"/CppFoldAtElse", cbPropertiesCppFoldAtElse->isChecked() );
	s->setValue( sp +"/CppFoldPreprocessorDirectives", cbPropertiesCppFoldPreprocessorDirectives->isChecked() );
	s->setValue( sp +"/CppStylePreprocessorDirectives", cbPropertiesCppStylePreprocessorDirectives->isChecked() );
	s->setValue( sp +"/CppIndentOpeningBrace", cbPropertiesCppIndentOpeningBrace->isChecked() );
	s->setValue( sp +"/CppIndentClosingBrace", cbPropertiesCppIndentClosingBrace->isChecked() );
	s->setValue( sp +"/HTMLFoldPreprocessorDirectives", cbPropertiesHTMLFoldPreprocessorDirectives->isChecked() );
	s->setValue( sp +"/HTMLCaseSensitivesTags", cbPropertiesHTMLCaseSensitivesTags->isChecked() );
	s->setValue( sp +"/PythonFoldComments", cbPropertiesPythonFoldComments->isChecked() );
	s->setValue( sp +"/PythonFoldStrings", cbPropertiesPythonFoldStrings->isChecked() );
	s->setValue( sp +"/PythonIndentationwarning", cbPythonIndentationwarning->itemData( cbPythonIndentationwarning->currentIndex() ) );
	s->setValue( sp +"/SQLFoldComments", cbPropertiesSQLFoldComments->isChecked() );
	s->setValue( sp +"/SQLBackslashEscapes", cbPropertiesSQLBackslashEscapes->isChecked() );
	s->setValue( sp +"/BashFoldComments", cbPropertiesBashFoldComments->isChecked() );
	s->setValue( sp +"/CSSFoldComments", cbPropertiesCSSFoldComments->isChecked() );
	s->setValue( sp +"/PerlFoldComments", cbPropertiesPerlFoldComments->isChecked() );
	s->setValue( sp +"/DFoldComments", cbPropertiesDFoldComments->isChecked() );
	s->setValue( sp +"/DFoldAtElse", cbPropertiesDFoldAtElse->isChecked() );
	s->setValue( sp +"/CMakeFoldAtElse", cbPropertiesCMakeFoldAtElse->isChecked() );
	s->setValue( sp +"/POVFoldComments", cbPropertiesPOVFoldComments->isChecked() );
	s->setValue( sp +"/POVFoldPreprocessorDirectives", cbPropertiesPOVFoldPreprocessorDirectives->isChecked() );
	s->setValue( sp +"/VHDLFoldComments", cbPropertiesVHDLFoldComments->isChecked() );
	s->setValue( sp +"/VHDLFoldAtElse", cbPropertiesVHDLFoldAtElse->isChecked() );
	s->setValue( sp +"/VHDLFoldAtBegin", cbPropertiesVHDLFoldAtBegin->isChecked() );
	s->setValue( sp +"/VHDLFoldAtParenthesis", cbPropertiesVHDLFoldAtParenthesis->isChecked() );
	s->setValue( sp +"/AllFoldCompact", cbPropertiesAllFoldCompact->isChecked() );

	// Abbreviations
	// Tools Menu
	//QString m = QString( "%1/menus/%2applications.menu" ).arg( QString( qgetenv( "XDG_CONFIG_DIRS" ) ) ).arg( QString( qgetenv( "XDG_MENU_PREFIX" ) ) );
	//qWarning( qPrintable( m ) );
*/
}

QPixmap UISettings::colourizedPixmap( const QColor& c ) const
{
	QPixmap p( 48, 16 );
	p.fill( c );
	return p;
}

QColor UISettings::iconBackgroundColor( const QIcon& i ) const
{
	return QColor( i.pixmap( QSize( 5, 5 ) ).toImage().pixel( QPoint( 0, 0 ) ) );
}

void UISettings::on_twMenu_itemSelectionChanged()
{
	// get item
	QTreeWidgetItem* it = twMenu->selectedItems().value( 0 );

	if ( it )
	{
		lInformations->setText( it->text( 0 ) );
		int i = twMenu->indexOfTopLevelItem( it );
		if ( !it->parent() )
		{
			switch ( i )
			{
				case 0:
				case 1:
					swPages->setCurrentIndex( i );
					break;
				case 2:
					break;
				default:
					swPages->setCurrentIndex( i +4 );
					break;
			}
		}
		else
			swPages->setCurrentIndex( it->parent()->indexOfChild( it ) +2 );
	}
}

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

void UISettings::on_cbAPIsLanguages_currentIndexChanged( int i )
{
	lwAPIs->clear();
	lwAPIs->addItems( cbAPIsLanguages->itemData( i ).toStringList() );
}

void UISettings::on_pbAPIsDelete_clicked()
{
	QListWidgetItem* it = lwAPIs->currentItem();
	if ( it )
	{
		delete it;
		cbAPIsLanguages_beforeChanged( cbAPIsLanguages->currentIndex() );
	}
}

void UISettings::on_pbAPIsAdd_clicked()
{
	QString s = leAPIs->text();
	if ( !s.isEmpty() && lwAPIs->findItems( s, Qt::MatchFixedString ).count() == 0 )
	{
		lwAPIs->addItem( s );
		cbAPIsLanguages_beforeChanged( cbAPIsLanguages->currentIndex() );
	}
}

void UISettings::on_pbAPIsBrowse_clicked()
{
	QString s = QFileDialog::getOpenFileName( window(), tr( "Select API file" ), QString::null, tr( "API Files (*.api);;All Files (*)" ) );
	if ( !s.isNull() )
		leAPIs->setText( s );
}

void UISettings::tbColours_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	QColor c = QColorDialog::getColor( iconBackgroundColor( tb->icon() ), window() );
	if ( c.isValid() )
		tb->setIcon( colourizedPixmap( c ) );
}

void UISettings::on_twHighlighterAssociations_itemClicked( QTreeWidgetItem* it, int )
{
	if ( it )
	{
		leHighlighterAssociationFilenamePattern->setText( it->text( 0 ) );
		cbHighlighterAssociationLexerLanguage->setCurrentIndex( cbHighlighterAssociationLexerLanguage->findText( it->text( 1 ) ) );
	}
}

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

void UISettings::on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( const QString& s )
{
	QsciLexer* l = pQScintilla::instance()->lexers().value( s );
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

	QVariant v;

	// fold comments
	v = pQScintilla::instance()->property( "foldComments", l );
	cbFoldComments->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldComments->setChecked( v.toBool() );

	// fold compact
	v = pQScintilla::instance()->property( "foldCompact", l );
	cbFoldCompact->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldCompact->setChecked( v.toBool() );

	// fold quotes
	v = pQScintilla::instance()->property( "foldQuotes", l );
	cbFoldQuotes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldQuotes->setChecked( v.toBool() );

	// fold directives
	v = pQScintilla::instance()->property( "foldDirectives", l );
	cbFoldDirectives->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldDirectives->setChecked( v.toBool() );

	// fold at begin
	v = pQScintilla::instance()->property( "foldAtBegin", l );
	cbFoldAtBegin->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldAtBegin->setChecked( v.toBool() );

	// fold at parenthesis
	v = pQScintilla::instance()->property( "foldAtParenthesis", l );
	cbFoldAtParenthesis->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldAtParenthesis->setChecked( v.toBool() );

	// fold at else
	v = pQScintilla::instance()->property( "foldAtElse", l );
	cbFoldAtElse->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldAtElse->setChecked( v.toBool() );

	// fold preprocessor
	v = pQScintilla::instance()->property( "foldPreprocessor", l );
	cbFoldPreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbFoldPreprocessor->setChecked( v.toBool() );

	// style preprocessor
	v = pQScintilla::instance()->property( "stylePreprocessor", l );
	cbStylePreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbStylePreprocessor->setChecked( v.toBool() );

	// indent opening brace
	cbIndentOpeningBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiOpening );

	// indent closing brace
	cbIndentClosingBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiClosing );

	// case sensitive tags
	v = pQScintilla::instance()->property( "caseSensitiveTags", l );
	cbCaseSensitiveTags->setVisible( v.isValid() );
	if ( v.isValid() )
		cbCaseSensitiveTags->setChecked( v.toBool() );

	// backslash escapes
	v = pQScintilla::instance()->property( "backslashEscapes", l );
	cbBackslashEscapes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbBackslashEscapes->setChecked( v.toBool() );

	// indentation warning
	v = pQScintilla::instance()->property( "indentationWarning", l );
	lIndentationWarning->setVisible( v.isValid() );
	cbIndentationWarning->setVisible( lIndentationWarning->isVisible() );
	if ( v.isValid() )
		cbIndentationWarning->setCurrentIndex( cbIndentationWarning->findData( v.toInt() ) );
}

void UISettings::on_lwSyntaxHighlightingStyleElements_itemClicked( QListWidgetItem* it )
{
	if ( it )
		cbSyntaxHighlightingFillToEndOfLine->setChecked( it->data( EolFill ).toBool() );
}

void UISettings::on_pbSyntaxHighlightingForegroundColour_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		QColor c = QColorDialog::getColor( it->foreground().color(), window() );
		if ( c.isValid() )
		{
			it->setForeground( c );
			pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setColor( c, it->data( Id ).toInt() );
			on_lwSyntaxHighlightingStyleElements_itemClicked( it );
		}
	}
}

void UISettings::on_pbSyntaxHighlightingBackgroundColour_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		QColor c = QColorDialog::getColor( it->background().color(), window() );
		if ( c.isValid() )
		{
			it->setBackground( c );
			pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setPaper( c, it->data( Id ).toInt() );
			on_lwSyntaxHighlightingStyleElements_itemClicked( it );
		}
	}
}

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
			pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setFont( f, it->data( Id ).toInt());
			on_lwSyntaxHighlightingStyleElements_itemClicked( it );
		}
	}
}

void UISettings::on_cbSyntaxHighlightingFillToEndOfLine_clicked( bool b )
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->currentItem();
	if ( it )
	{
		it->setData( EolFill, b );
		pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setEolFill( b, it->data( Id ).toInt() );
	}
}

void UISettings::cbProperties_toggled( bool b )
{
	// get check box
	QCheckBox* cb = qobject_cast<QCheckBox*>( sender() );
	if ( !cb )
		return;

	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() );

	// set lexer properties
	pQScintilla::instance()->setProperty( cb->statusTip(), l, b );
}

void UISettings::on_pbSyntaxHighlightingAllBackgroundColours_clicked()
{
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() );
	QColor c = QColorDialog::getColor( l->defaultPaper(), window() );
	if ( c.isValid() )
	{
		l->setDefaultPaper( c );
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( l->language() );
	}
}

void UISettings::on_pbSyntaxHighlightingAllFonts_clicked()
{
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() );
	bool b;
	QFont f = QFontDialog::getFont( &b, l->defaultFont(), window() );
	if ( b )
	{
		l->setDefaultFont( f );
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( l->language() );
	}
}

void UISettings::on_pbStyleLineNumbersFont_clicked()
{
	bool b;
	QFont f = QFontDialog::getFont( &b, leStyleLineNumbersFont->font(), window() );
	if ( b )
		leStyleLineNumbersFont->setFont( f );
}

void UISettings::on_pbStyleMonospacedFont_clicked()
{
	bool b;
	QFont f = QFontDialog::getFont( &b, leStyleMonospacedFont->font(), window() );
	if ( b )
		leStyleMonospacedFont->setFont( f );
}

void UISettings::accept()
{
	saveSettings();
	QDialog::accept();
}
