#include "UISettings.h"
#include "pSettings.h"
#include "pQScintilla.h"
#include "UIEditTemplate.h"
#include "UIAddAbbreviation.h"

#include <QButtonGroup>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextCodec>
#include <QDir>

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

	// resize column
	twTemplatesType->setColumnWidth( 0, 100 );
	twTemplatesType->setColumnWidth( 1, 100 );

	// auto completion source
	bgAutoCompletionSource = new QButtonGroup( gbAutoCompletionSource );
	bgAutoCompletionSource->addButton( rbAcsDocument, QsciScintilla::AcsDocument );
	bgAutoCompletionSource->addButton( rbAcsAPIs, QsciScintilla::AcsAPIs );
	bgAutoCompletionSource->addButton( rbAcsAll, QsciScintilla::AcsAll );

	// calltips style
	bgCallTipsStyle = new QButtonGroup( gbCalltipsEnabled );
	bgCallTipsStyle->addButton( rbCallTipsNoContext, QsciScintilla::CallTipsNoContext );
	bgCallTipsStyle->addButton( rbCallTipsNoAutoCompletionContext, QsciScintilla::CallTipsNoAutoCompletionContext );
	bgCallTipsStyle->addButton( rbCallTipsContext, QsciScintilla::CallTipsContext );

	// brace match
	bgBraceMatch = new QButtonGroup( gbBraceMatchingEnabled );
	bgBraceMatch->addButton( rbStrictBraceMatch, QsciScintilla::StrictBraceMatch );
	bgBraceMatch->addButton( rbSloppyBraceMatch, QsciScintilla::SloppyBraceMatch );

	// edge mode
	bgEdgeMode = new QButtonGroup( gbEdgeModeEnabled );
	bgEdgeMode->addButton( rbEdgeLine, QsciScintilla::EdgeLine );
	bgEdgeMode->addButton( rbEdgeBackground, QsciScintilla::EdgeBackground );

	// fold style
	bgFoldStyle = new QButtonGroup( gbFoldMarginEnabled );
	bgFoldStyle->addButton( rbPlainFoldStyle, QsciScintilla::PlainFoldStyle );
	bgFoldStyle->addButton( rbCircledTreeFoldStyle, QsciScintilla::CircledTreeFoldStyle );
	bgFoldStyle->addButton( rbCircledFoldStyle, QsciScintilla::CircledFoldStyle );
	bgFoldStyle->addButton( rbBoxedFoldStyle, QsciScintilla::BoxedFoldStyle );
	bgFoldStyle->addButton( rbBoxedTreeFoldStyle, QsciScintilla::BoxedTreeFoldStyle );

	// eol mode
	bgEolMode = new QButtonGroup( gbEolMode );
	bgEolMode->addButton( rbEolUnix, QsciScintilla::EolUnix );
	bgEolMode->addButton( rbEolMac, QsciScintilla::EolMac );
	bgEolMode->addButton( rbEolWindows, QsciScintilla::EolWindows );

	// whitespace visibility
	bgWhitespaceVisibility = new QButtonGroup( gbWhitespaceVisibilityEnabled );
	bgWhitespaceVisibility->addButton( rbWsVisible, QsciScintilla::WsVisible );
	bgWhitespaceVisibility->addButton( rbWsVisibleAfterIndent, QsciScintilla::WsVisibleAfterIndent );

	// wrap mode
	bgWrapMode = new QButtonGroup( gbWrapModeEnabled );
	bgWrapMode->addButton( rbWrapWord, QsciScintilla::WrapWord );
	bgWrapMode->addButton( rbWrapCharacter, QsciScintilla::WrapCharacter );

	// wrap visual flag
	bgStartWrapVisualFlag = new QButtonGroup( wStartWrapVisualFlags );
	bgStartWrapVisualFlag->addButton( rbStartWrapFlagByText, QsciScintilla::WrapFlagByText );
	bgStartWrapVisualFlag->addButton( rbStartWrapFlagByBorder, QsciScintilla::WrapFlagByBorder );
	bgEndWrapVisualFlag = new QButtonGroup( wEndWrapVisualFlags );
	bgEndWrapVisualFlag->addButton( rbEndWrapFlagByText, QsciScintilla::WrapFlagByText );
	bgEndWrapVisualFlag->addButton( rbEndWrapFlagByBorder, QsciScintilla::WrapFlagByBorder );

	// fill lexers combo
	QStringList l = pQScintilla::instance()->languages();
	l.sort();
	cbAPIsLanguages->addItems( l );
	cbLexersAssociationsLanguage->addItems( l );
	cbLexersLanguage->addItems( l );

	// resize column
	twHighlighterAssociations->setColumnWidth( 0, 200 );

	// resize column
	twAbbreviations->setColumnWidth( 0, 100 );
	twAbbreviations->setColumnWidth( 1, 300 );

	// read settings
	loadSettings();

	// resize to minimum size
	resize( minimumSizeHint() );

/*
	// loads text codecs
	QStringList l;
	foreach ( QByteArray a, QTextCodec::availableCodecs() )
		l << a;
	l.sort();
	cbGeneralEncoding->addItems( l );

	// python indentation warning
	cbLexerIndentationWarning->addItem( tr( "No warning" ), QsciLexerPython::NoWarning );
	cbLexerIndentationWarning->addItem( tr( "Inconsistent" ), QsciLexerPython::Inconsistent );
	cbLexerIndentationWarning->addItem( tr( "Tabs after spaces" ), QsciLexerPython::TabsAfterSpaces );
	cbLexerIndentationWarning->addItem( tr( "Spaces" ), QsciLexerPython::Spaces );
	cbLexerIndentationWarning->addItem( tr( "Tabs" ), QsciLexerPython::Tabs );

	// connections
	foreach ( QToolButton* tb, pEditorColours->findChildren<QToolButton*>() )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbStyleEdgeModeBackgroundColour, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( pbLexerForeground, SIGNAL( clicked() ), this, SLOT( lexersColour_clicked() ) );
	connect( pbLexerBackground, SIGNAL( clicked() ), this, SLOT( lexersColour_clicked() ) );
	connect( pbLexerFont, SIGNAL( clicked() ), this, SLOT( lexersFont_clicked() ) );
	connect( pbLexerAllForeground, SIGNAL( clicked() ), this, SLOT( lexersColour_clicked() ) );
	connect( pbLexerAllBackground, SIGNAL( clicked() ), this, SLOT( lexersColour_clicked() ) );
	connect( pbLexerAllFont, SIGNAL( clicked() ), this, SLOT( lexersFont_clicked() ) );
	foreach ( QCheckBox* cb, gbLexers->findChildren<QCheckBox*>() )
		if ( cb != cbLexerFillEol )
			connect( cb, SIGNAL( clicked( bool ) ), this, SLOT( cbProperties_clicked( bool ) ) );

	
*/
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
	sp = QString( "%1/Editor/Templates" ).arg( SettingsPath );
	leTemplatesPath->setText( s->value( sp +"/DefaultDirectory", "%HOME%/.Monkey Studio/Templates" ).toString() );
	int size = s->beginReadArray( sp );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = new QTreeWidgetItem( twTemplatesType );
		it->setText( 0, s->value( "Language" ).toString() );
		it->setText( 1, s->value( "Name" ).toString() );
		it->setText( 2, s->value( "Description" ).toString() );
		it->setData( 0, Qt::UserRole, s->value( "Icon" ).toString() );
		it->setData( 0, Qt::UserRole +1, s->value( "Filename" ).toString() );
		it->setIcon( 0, QIcon( s->value( "Icon" ).toString() ) );
	}
	s->endArray();

	// Editor
	pQScintilla* sc = pQScintilla::instance();
	//  Auto Completion
	gbAutoCompletionEnabled->setChecked( sc->autocompletionEnabled() );
	cbAutoCompletionCaseSensitivity->setChecked( sc->autoCompletionCaseSensitivity() );
	cbAutoCompletionReplaceWord->setChecked( sc->autoCompletionReplaceWord() );
	cbAutoCompletionShowSingle->setChecked( sc->autoCompletionShowSingle() );
	sAutoCompletionThreshold->setValue( sc->autoCompletionThreshold() );
	if ( bgAutoCompletionSource->button( sc->autoCompletionSource() ) )
		bgAutoCompletionSource->button( sc->autoCompletionSource() )->setChecked( true );
	//  Call Tips
	gbCalltipsEnabled->setChecked( sc->callTipsEnabled() );
	sCallTipsVisible->setValue( sc->callTipsVisible() );
	if ( bgCallTipsStyle->button( sc->callTipsStyle() ) )
		bgCallTipsStyle->button( sc->callTipsStyle() )->setChecked( true );
	tbCalltipsBackground->setIcon( colourizedPixmap( sc->callTipsBackgroundColor() ) );
	tbCalltipsForeground->setIcon( colourizedPixmap( sc->callTipsForegroundColor() ) );
	tbCalltipsHighlight->setIcon( colourizedPixmap( sc->callTipsHighlightColor() ) );
	//  Indentation
	cbAutoIndent->setChecked( sc->autoIndent() );
	cbBackspaceUnindents->setChecked( sc->backspaceUnindents() );
	cbIndentationGuides->setChecked( sc->indentationGuides() );
	cbIndentationUseTabs->setChecked( sc->indentationsUseTabs() );
	cbTabIndents->setChecked( sc->tabIndents() );
	sIndentationTabWidth->setValue( sc->tabWidth() );
	sIndentationWidth->setValue( sc->indentationWidth() );
	tbIndentationGuidesBackground->setIcon( colourizedPixmap( sc->indentationGuidesBackgroundColor() ) );
	tbIndentationGuidesForeground->setIcon( colourizedPixmap( sc->indentationGuidesForegroundColor() ) );
	// Brace Matching
	gbBraceMatchingEnabled->setChecked( sc->braceMatchingEnabled() );
	if ( bgBraceMatch->button( sc->braceMatching() ) )
		bgBraceMatch->button( sc->braceMatching() )->setChecked( true );
	tbMatchedBraceForeground->setIcon( colourizedPixmap( sc->matchedBraceBackgroundColor() ) );
	tbMatchedBraceBackground->setIcon( colourizedPixmap( sc->matchedBraceForegroundColor() ) );
	tbUnmatchedBraceBackground->setIcon( colourizedPixmap( sc->unmatchedBraceBackgroundColor() ) );
	tbUnmatchedBraceBackground->setIcon( colourizedPixmap( sc->unmatchedBraceForegroundColor() ) );
	// Edge Mode
	gbEdgeModeEnabled->setChecked( sc->edgeModeEnabled() );
	if ( bgEdgeMode->button( sc->edgeMode() ) )
		bgEdgeMode->button( sc->edgeMode() )->setChecked( true );
	sEdgeColumnNumber->setValue( sc->edgeColumn() );
	tbEdgeBackground->setIcon( colourizedPixmap( sc->edgeColor() ) );
	// Caret
	gbCaretLineVisible->setChecked( sc->caretLineVisible() );
	tbCaretLineBackground->setIcon( colourizedPixmap( sc->caretLineBackgroundColor() ) );
	tbCaretForeground->setIcon( colourizedPixmap( sc->caretForegroundColor() ) );
	sCaretWidth->setValue( sc->caretWidth() );
	// Margins
	gbLineNumbersMarginEnabled->setChecked( sc->lineNumbersMarginEnabled() );
	sLineNumbersMarginWidth->setValue( sc->lineNumbersMarginWidth() );
	cbLineNumbersMarginAutoWidth->setChecked( sc->lineNumbersMarginAutoWidth() );
	gbFoldMarginEnabled->setChecked( sc->foldMarginEnabled() );
	if ( bgFoldStyle->button( sc->folding() ) )
		bgFoldStyle->button( sc->folding() )->setChecked( true );
	tbFoldMarginForeground->setIcon( colourizedPixmap( sc->foldMarginForegroundColor() ) );
	tbFoldMarginBackground->setIcon( colourizedPixmap( sc->foldMarginBackgroundColor() ) );
	gbGlobalMarginsEnabled->setChecked( sc->globalMarginsEnabled() );
	tbGlobalMarginsForeground->setIcon( colourizedPixmap( sc->marginsForegroundColor() ) );
	tbGlobalMarginsBackground->setIcon( colourizedPixmap( sc->marginsBackgroundColor() ) );
	tbGlobalMarginsFont->setFont( sc->marginsFont() );
	// Special Characters
	bgEolMode->button( sc->eolMode() )->setChecked( true );
	cbEolVisibility->setChecked( sc->eolVisibility() );
	gbWhitespaceVisibilityEnabled->setChecked( sc->whitespaceVisibility() != QsciScintilla::WsInvisible );
	if ( gbWhitespaceVisibilityEnabled->isChecked() )
		bgWhitespaceVisibility->button( sc->whitespaceVisibility() )->setChecked( true );
	gbWrapModeEnabled->setChecked( sc->wrapMode() != QsciScintilla::WrapNone );
	if ( gbWrapModeEnabled->isChecked() )
		bgWrapMode->button( sc->wrapMode() )->setChecked( true );
	gbWrapVisualFlagsEnabled->setChecked( sc->wrapVisualFlagsEnabled() );
	if ( bgStartWrapVisualFlag->button( sc->startWrapVisualFlag() ) )
		bgStartWrapVisualFlag->button( sc->startWrapVisualFlag() )->setChecked( true );
	if ( bgEndWrapVisualFlag->button( sc->endWrapVisualFlag() ) )
		bgEndWrapVisualFlag->button( sc->endWrapVisualFlag() )->setChecked( true );
	sWrappedLineIndentWidth->setValue( sc->wrappedLineIndentWidth() );

/*
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
	QHash<QString, QStringList> l = pQScintilla::instance()->suffixes();
	foreach ( QString k, l.keys() )
	{
		foreach ( QString e, l.value( k ) )
		{
			QTreeWidgetItem* it = new QTreeWidgetItem( twHighlighterAssociations );
			it->setText( 0, e );
			it->setText( 1, k );
		}
	}

	//  Highlighting
	pQScintilla::instance()->readSettings();
	if ( cbLexersLanguage->count() )
		on_cbLexersLanguage_currentIndexChanged( cbLexersLanguage->itemText( 0 ) );

	//  Abbreviations
	sp = QString( "%1/Editor/Abbreviations" ).arg( SettingsPath );
	cbAbbreviations->setChecked( s->value( sp +"/Enabled", false ).toBool() );
	size = s->beginReadArray( sp );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = new QTreeWidgetItem( twAbbreviations );
		it->setText( 0, s->value( "Template" ).toString() );
		it->setText( 1, s->value( "Description" ).toString() );
		it->setText( 2, s->value( "Language" ).toString() );
		it->setData( 0, Qt::UserRole, s->value( "Code" ).toString() );
	}
	s->endArray();

/
	// Tools Menu
	//QString m = QString( "%1/menus/%2applications.menu" ).arg( QString( qgetenv( "XDG_CONFIG_DIRS" ) ) ).arg( QString( qgetenv( "XDG_MENU_PREFIX" ) ) );
	//qWarning( qPrintable( m ) );
*/
}

void UISettings::saveSettings()
{
/*
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
	sp = QString( "%1/Editor/Templates" ).arg( SettingsPath );
	// remove key
	s->remove( sp );
	// default templates path
	s->setValue( sp +"/DefaultDirectory", leTemplatesPath->text() );
	// write new ones
	s->beginWriteArray( sp );
	for ( int i = 0; i < twTemplatesType->topLevelItemCount(); i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = twTemplatesType->topLevelItem( i );

		s->setValue( "Language", it->text( 0 ) );
		s->setValue( "Name", it->text( 1 ) );
		s->setValue( "Description", it->text( 2 ) );
		s->setValue( "Icon", it->data( 0, Qt::UserRole ).toString() );
		s->setValue( "Filename", it->data( 0, Qt::UserRole +1 ).toString() );
	}
	s->endArray();

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
	// remove old associations
	s->remove( sp );
	// write new ones
	for ( int i = 0; i < twHighlighterAssociations->topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* it = twHighlighterAssociations->topLevelItem( i );
		s->setValue( sp +it->text( 0 ), it->text( 1 ) );
	}

	//  Highlighting
	pQScintilla::instance()->writeSettings();

	//  Abbreviations
	sp = QString( "%1/Editor/Abbreviations" ).arg( SettingsPath );
	// remove key
	s->remove( sp );
	// enable abbreviations
	s->setValue( sp +"/Enabled", cbAbbreviations->isChecked() );
	// write new ones
	s->beginWriteArray( sp );
	for ( int i = 0; i < twAbbreviations->topLevelItemCount(); i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = twAbbreviations->topLevelItem( i );

		s->setValue( "Template", it->text( 0 ) );
		s->setValue( "Description", it->text( 1 ) );
		s->setValue( "Language", it->text( 2 ) );
		s->setValue( "Code", it->data( 0, Qt::UserRole ).toString() );
	}
	s->endArray();

/
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
					swPages->setCurrentIndex( i +10 );
					break;
			}
		}
		else
			swPages->setCurrentIndex( it->parent()->indexOfChild( it ) +2 );
	}
}

void UISettings::on_tbDefaultProjectsDirectory_clicked()
{
	QString s = QFileDialog::getExistingDirectory( window(), tr( "Select default projects directory" ), leDefaultProjectsDirectory->text() );
	if ( !s.isNull() )
		leDefaultProjectsDirectory->setText( s );
}

void UISettings::on_tbTemplatesPath_clicked()
{
	const QString d = leTemplatesPath->text().replace( "%HOME%", QDir::homePath() );
	QString s = QFileDialog::getExistingDirectory( window(), tr( "Select default templates directory" ), d );
	if ( !s.isNull() )
	{
		if ( s.endsWith( "/" ) )
			s.chop( 1 );
		leTemplatesPath->setText( s.replace( QDir::homePath(), "%HOME%" ) );
	}
}

void UISettings::on_pbAddTemplateType_clicked()
{
	UIEditTemplate::edit( twTemplatesType, 0, leTemplatesPath->text() );
}

void UISettings::on_pbEditTemplateType_clicked()
{
	UIEditTemplate::edit( twTemplatesType, twTemplatesType->selectedItems().value( 0 ), leTemplatesPath->text() );
}

void UISettings::on_pbRemoveTemplateType_clicked()
{
	delete twTemplatesType->selectedItems().value( 0 );
}

void UISettings::on_pbEditTemplate_clicked()
{
	// get item
	QTreeWidgetItem* it = twTemplatesType->selectedItems().value( 0 );

	// open template file
	if ( it )
	{
		// get correct full path
		const QString t = leTemplatesPath->text().replace( "%HOME%", QDir::homePath() );
		const QString f = it->data( 0, Qt::UserRole +1 ).toString().replace( "%TEMPLATE_PATH%", t );

		// open template file
		//pFileManager::instance()->openFile( f );

		qWarning( "edit tempalte require: %s", qPrintable( f ) );
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
	// get selected item
	QListWidgetItem* it = lwAPIs->selectedItems().value( 0 );

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
		leAPIs->clear();
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

void UISettings::on_twHighlighterAssociations_itemSelectionChanged()
{
/*
	QTreeWidgetItem* it = twHighlighterAssociations->selectedItems().value( 0 );
	if ( it )
	{
		leHighlighterAssociationFilenamePattern->setText( it->text( 0 ) );
		cbHighlighterAssociationLexerLanguage->setCurrentIndex( cbHighlighterAssociationLexerLanguage->findText( it->text( 1 ) ) );
	}
*/
}

void UISettings::on_pbHighlighterAssociationAddChange_clicked()
{
/*
	QString f = leHighlighterAssociationFilenamePattern->text();
	QString l = cbHighlighterAssociationLexerLanguage->currentText();
	if ( f.isEmpty() || l.isEmpty() )
		return;
	QTreeWidgetItem* it = twHighlighterAssociations->selectedItems().value( 0 );
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
*/
}

void UISettings::on_pbHighlighterAssociationDelete_clicked()
{
/*
	QTreeWidgetItem* it = twHighlighterAssociations->selectedItems().value( 0 );
	if ( it )
	{
		delete it;
		twHighlighterAssociations->setCurrentItem( 0 );
		twHighlighterAssociations->selectionModel()->clear();
		leHighlighterAssociationFilenamePattern->clear();
		cbHighlighterAssociationLexerLanguage->setCurrentIndex( -1 );
	}
*/
}

void UISettings::on_cbLexersLanguage_currentIndexChanged( const QString& s )
{
/*
	QsciLexer* l = pQScintilla::instance()->lexers().value( s );
	lwLexersElements->clear();
	for ( int i = 0; i < 128; i++ )
	{
		QString n = l->description( i );
		if ( !n.isEmpty() )
		{
			QListWidgetItem* it = new QListWidgetItem( lwLexersElements );
			it->setText( n );
			it->setForeground( l->color( i ) );
			it->setBackground( l->paper( i ) );
			it->setFont( l->font( i ) );
			it->setData( Qt::UserRole, i );
		}
	}

	QVariant v;

	// fold comments
	v = pQScintilla::instance()->property( "foldComments", l );
	cbLexerFoldComments->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldComments->setChecked( v.toBool() );

	// fold compact
	v = pQScintilla::instance()->property( "foldCompact", l );
	cbLexerFoldCompact->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldCompact->setChecked( v.toBool() );

	// fold quotes
	v = pQScintilla::instance()->property( "foldQuotes", l );
	cbLexerFoldQuotes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldQuotes->setChecked( v.toBool() );

	// fold directives
	v = pQScintilla::instance()->property( "foldDirectives", l );
	cbLexerFoldDirectives->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldDirectives->setChecked( v.toBool() );

	// fold at begin
	v = pQScintilla::instance()->property( "foldAtBegin", l );
	cbLexerFoldAtBegin->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldAtBegin->setChecked( v.toBool() );

	// fold at parenthesis
	v = pQScintilla::instance()->property( "foldAtParenthesis", l );
	cbLexerFoldAtParenthesis->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldAtParenthesis->setChecked( v.toBool() );

	// fold at else
	v = pQScintilla::instance()->property( "foldAtElse", l );
	cbLexerFoldAtElse->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldAtElse->setChecked( v.toBool() );

	// fold preprocessor
	v = pQScintilla::instance()->property( "foldPreprocessor", l );
	cbLexerFoldPreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerFoldPreprocessor->setChecked( v.toBool() );

	// style preprocessor
	v = pQScintilla::instance()->property( "stylePreprocessor", l );
	cbLexerStylePreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerStylePreprocessor->setChecked( v.toBool() );

	// indent opening brace
	cbLexerIndentOpeningBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiOpening );

	// indent closing brace
	cbLexerIndentClosingBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiClosing );

	// case sensitive tags
	v = pQScintilla::instance()->property( "caseSensitiveTags", l );
	cbLexerCaseSensitiveTags->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerCaseSensitiveTags->setChecked( v.toBool() );

	// backslash escapes
	v = pQScintilla::instance()->property( "backslashEscapes", l );
	cbLexerBackslashEscapes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexerBackslashEscapes->setChecked( v.toBool() );

	// indentation warning
	v = pQScintilla::instance()->property( "indentationWarning", l );
	lLexerIndentationWarning->setVisible( v.isValid() );
	cbLexerIndentationWarning->setVisible( lLexerIndentationWarning->isVisible() );
	if ( v.isValid() )
		cbLexerIndentationWarning->setCurrentIndex( cbLexerIndentationWarning->findData( v.toInt() ) );
*/
}

void UISettings::on_lwLexersElements_itemSelectionChanged()
{
/*
	QListWidgetItem* it = lwLexersElements->selectedItems().value( 0 );
	if ( it )
		cbLexerFillEol->setChecked( pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() )->eolFill( it->data( Qt::UserRole ).toInt() ) );
*/
}

void UISettings::lexersColour_clicked()
{
/*
	// get sender
	QObject* o = sender();

	// color
	QColor c;

	// element colour
	if ( o == pbLexerForeground || o == pbLexerBackground )
	{
		// get item
		QListWidgetItem* it = lwLexersElements->selectedItems().value( 0 );

		// cancel if no item
		if ( !it )
			return;

		// get color
		c = QColorDialog::getColor( o == pbLexerForeground ? it->foreground().color() : it->background().color(), window() );

		// apply color
		if ( c.isValid() )
		{
			if ( o == pbLexerForeground )
			{
				it->setForeground( c );
				pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() )->setColor( c, it->data( Qt::UserRole ).toInt() );
			}
			else if ( o == pbLexerBackground )
			{
				it->setBackground( c );
				pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() )->setPaper( c, it->data( Qt::UserRole ).toInt() );
			}
		}
	}
	// gobal color
	else if ( o == pbLexerAllForeground || o == pbLexerAllBackground )
	{
		// get lexer
		QsciLexer* l = pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() );

		// get color
		c = QColorDialog::getColor( o == pbLexerAllForeground ? l->color( -1 ) : l->paper( -1 ), window() );

		// apply
		if ( c.isValid() )
		{
			if ( o == pbLexerAllForeground )
				l->setColor( c, -1 );
			else if ( o == pbLexerAllBackground )
				l->setPaper( c, -1 );

			// refresh
			on_cbLexersLanguage_currentIndexChanged( l->language() );
		}
	}
*/
}

void UISettings::lexersFont_clicked()
{
/*
	// get sender
	QObject* o = sender();

	bool b;
	QFont f;

	if ( o == pbLexerFont )
	{
		// get item
		QListWidgetItem* it = lwLexersElements->selectedItems().value( 0 );

		// cancel if no item
		if ( !it )
			return;

		// get font
		f = QFontDialog::getFont( &b, it->font(), window() );

		// apply
		if ( b )
		{
			it->setFont( f );
			pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() )->setFont( f, it->data( Qt::UserRole ).toInt() );
		}
	}
	else if ( o == pbLexerAllFont )
	{
		// get lexer
		QsciLexer* l = pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() );

		// get font
		f = QFontDialog::getFont( &b, l->font( -1 ), window() );

		// apply
		if ( b )
		{
			l->setFont( f, -1 );
			on_cbLexersLanguage_currentIndexChanged( l->language() );
		}
	}
*/
}

void UISettings::on_cbLexerFillEol_clicked( bool b )
{
/*
	QListWidgetItem* it = lwLexersElements->selectedItems().value( 0 );
	if ( it )
		pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() )->setEolFill( b, it->data( Qt::UserRole ).toInt() );
*/
}

void UISettings::cbProperties_clicked( bool b )
{
/*
	// get check box
	QCheckBox* cb = qobject_cast<QCheckBox*>( sender() );
	if ( !cb )
		return;

	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() );

	// set lexer properties
	if ( cb == cbLexerIndentOpeningBrace || cb == cbLexerIndentClosingBrace )
	{
		if ( cbLexerIndentOpeningBrace->isChecked() && cbLexerIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening | QsciScintilla::AiClosing );
		else if ( cbLexerIndentOpeningBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening );
		else if ( cbLexerIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiClosing );
		else
			l->setAutoIndentStyle( QsciScintilla::AiMaintain );
	}
	else
		pQScintilla::instance()->setProperty( cb->statusTip(), l, b );
*/
}

void UISettings::on_cbLexerIndentationWarning_currentIndexChanged( int i )
{
/*
	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() );

	// set lexer properties
	pQScintilla::instance()->setProperty( cbLexerIndentationWarning->statusTip(), l, cbLexerIndentationWarning->itemData( i ) );
*/
}

void UISettings::on_pbLexerReset_clicked()
{
/*
	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbLexersLanguage->currentText() );

	// reset and refresh
	if ( l )
	{
		pQScintilla::instance()->resetLexer( l );
		on_cbLexersLanguage_currentIndexChanged( l->language() );
	}
*/
}

void UISettings::on_pbStyleLineNumbersFont_clicked()
{
/*
	bool b;
	QFont f = QFontDialog::getFont( &b, leStyleLineNumbersFont->font(), window() );
	if ( b )
		leStyleLineNumbersFont->setFont( f );
*/
}

void UISettings::on_pbStyleMonospacedFont_clicked()
{
/*
	bool b;
	QFont f = QFontDialog::getFont( &b, leStyleMonospacedFont->font(), window() );
	if ( b )
		leStyleMonospacedFont->setFont( f );
*/
}

void UISettings::on_twAbbreviations_itemSelectionChanged()
{
/*
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );

	if ( it )
		teAbbreviation->setPlainText( it->data( 0, Qt::UserRole ).toString() );

	// enable/disable according to selection
	teAbbreviation->setEnabled( it );
*/
}

void UISettings::on_pbAddAbbreviation_clicked()
{
/*
	UIAddAbbreviation::edit( twAbbreviations );
*/
}

void UISettings::on_pbRemoveAbbreviation_clicked()
{
/*
	delete twAbbreviations->selectedItems().value( 0 );
	teAbbreviation->clear();
*/
}

void UISettings::on_teAbbreviation_textChanged()
{
/*
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );

	if ( it )
		it->setData( 0, Qt::UserRole, teAbbreviation->toPlainText() );
*/
}

void UISettings::accept()
{
	saveSettings();
	QDialog::accept();
}
