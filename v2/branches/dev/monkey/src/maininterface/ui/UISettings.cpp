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

	QStringList l;

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

	// loads text codecs
	l.clear();
	foreach ( QByteArray a, QTextCodec::availableCodecs() )
		l << a;
	l.sort();
	cbDefaultEncoding->addItems( l );

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
	l = pQScintilla::instance()->languages();
	l.sort();
	cbSourceAPIsLanguages->addItems( l );
	cbLexersAssociationsLanguages->addItems( l );
	cbLexersHighlightingLanguages->addItems( l );

	// resize column
	twLexersAssociations->setColumnWidth( 0, 200 );

	// python indentation warning
	cbLexersHighlightingIndentationWarning->addItem( tr( "No warning" ), QsciLexerPython::NoWarning );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Inconsistent" ), QsciLexerPython::Inconsistent );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Tabs after spaces" ), QsciLexerPython::TabsAfterSpaces );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Spaces" ), QsciLexerPython::Spaces );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Tabs" ), QsciLexerPython::Tabs );

	// resize column
	twAbbreviations->setColumnWidth( 0, 100 );
	twAbbreviations->setColumnWidth( 1, 300 );

	// read settings
	loadSettings();

	// connections
	// Colours Button
	connect( tbSelectionBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbSelectionForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCalltipsBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCalltipsForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCalltipsHighlight, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbIndentationGuidesBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbIndentationGuidesForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMatchedBraceForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMatchedBraceBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbUnmatchedBraceForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbUnmatchedBraceBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbEdgeColor, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCaretLineBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCaretForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbFoldMarginForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbFoldMarginBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMarginsForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMarginsBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	// margin font
	connect( tbMarginsFont, SIGNAL( clicked() ), this, SLOT( tbFonts_clicked() ) );
	// lexer elements highlighting
	connect( pbLexersHighlightingForeground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingBackground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingFont, SIGNAL( clicked() ), this, SLOT( lexersHighlightingFont_clicked() ) );
	connect( pbLexersHighlightingAllForeground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingAllBackground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingAllFont, SIGNAL( clicked() ), this, SLOT( lexersHighlightingFont_clicked() ) );
	foreach ( QCheckBox* cb, gbLexersHighlightingElements->findChildren<QCheckBox*>() )
		if ( cb != cbLexersHighlightingFillEol )
			connect( cb, SIGNAL( clicked( bool ) ), this, SLOT( cbLexersHighlightingProperties_clicked( bool ) ) );
	// apply button
	connect( dbbButtons->button( QDialogButtonBox::Apply ), SIGNAL( clicked() ), this, SLOT( apply() ) );

	// resize to minimum size
	resize( minimumSizeHint() );
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
	//  General
	cbAutoSyntaxCheck->setChecked( sc->autoSyntaxCheck() );
	cbConvertTabsUponOpen->setChecked( sc->convertTabsUponOpen() );
	cbCreateBackupUponOpen->setChecked( sc->createBackupUponOpen() );
	cbAutoEolConversion->setChecked( sc->autoEolConversion() );
	cbDefaultEncoding->setCurrentIndex( cbDefaultEncoding->findText( sc->defaultEncoding() ) );
	tbSelectionBackground->setIcon( colourizedPixmap( sc->selectionBackgroundColor() ) );
	tbSelectionForeground->setIcon( colourizedPixmap( sc->selectionForegroundColor() ) );
	//  Auto Completion
	gbAutoCompletionEnabled->setChecked( sc->autoCompletionSource() != QsciScintilla::AcsNone );
	cbAutoCompletionCaseSensitivity->setChecked( sc->autoCompletionCaseSensitivity() );
	cbAutoCompletionReplaceWord->setChecked( sc->autoCompletionReplaceWord() );
	cbAutoCompletionShowSingle->setChecked( sc->autoCompletionShowSingle() );
	sAutoCompletionThreshold->setValue( sc->autoCompletionThreshold() );
	if ( bgAutoCompletionSource->button( sc->autoCompletionSource() ) )
		bgAutoCompletionSource->button( sc->autoCompletionSource() )->setChecked( true );
	//  Call Tips
	gbCalltipsEnabled->setChecked( sc->callTipsStyle() != QsciScintilla::CallTipsNone );
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
	//  Brace Matching
	gbBraceMatchingEnabled->setChecked( sc->braceMatching() != QsciScintilla::NoBraceMatch );
	if ( bgBraceMatch->button( sc->braceMatching() ) )
		bgBraceMatch->button( sc->braceMatching() )->setChecked( true );
	tbMatchedBraceForeground->setIcon( colourizedPixmap( sc->matchedBraceForegroundColor() ) );
	tbMatchedBraceBackground->setIcon( colourizedPixmap( sc->matchedBraceBackgroundColor() ) );
	tbUnmatchedBraceBackground->setIcon( colourizedPixmap( sc->unmatchedBraceBackgroundColor() ) );
	tbUnmatchedBraceForeground->setIcon( colourizedPixmap( sc->unmatchedBraceForegroundColor() ) );
	//  Edge Mode
	gbEdgeModeEnabled->setChecked( sc->edgeMode() != QsciScintilla::EdgeNone );
	if ( bgEdgeMode->button( sc->edgeMode() ) )
		bgEdgeMode->button( sc->edgeMode() )->setChecked( true );
	sEdgeColumnNumber->setValue( sc->edgeColumn() );
	tbEdgeColor->setIcon( colourizedPixmap( sc->edgeColor() ) );
	//  Caret
	gbCaretLineVisible->setChecked( sc->caretLineVisible() );
	tbCaretLineBackground->setIcon( colourizedPixmap( sc->caretLineBackgroundColor() ) );
	tbCaretForeground->setIcon( colourizedPixmap( sc->caretForegroundColor() ) );
	sCaretWidth->setValue( sc->caretWidth() );
	//  Margins
	gbLineNumbersMarginEnabled->setChecked( sc->lineNumbersMarginEnabled() );
	sLineNumbersMarginWidth->setValue( sc->lineNumbersMarginWidth() );
	cbLineNumbersMarginAutoWidth->setChecked( sc->lineNumbersMarginAutoWidth() );
	gbFoldMarginEnabled->setChecked( sc->folding() != QsciScintilla::NoFoldStyle );
	if ( bgFoldStyle->button( sc->folding() ) )
		bgFoldStyle->button( sc->folding() )->setChecked( true );
	tbFoldMarginForeground->setIcon( colourizedPixmap( sc->foldMarginForegroundColor() ) );
	tbFoldMarginBackground->setIcon( colourizedPixmap( sc->foldMarginBackgroundColor() ) );
	gbMarginsEnabled->setChecked( sc->marginsEnabled() );
	tbMarginsForeground->setIcon( colourizedPixmap( sc->marginsForegroundColor() ) );
	tbMarginsBackground->setIcon( colourizedPixmap( sc->marginsBackgroundColor() ) );
	tbMarginsFont->setFont( sc->marginsFont() );
	//  Special Characters
	bgEolMode->button( sc->eolMode() )->setChecked( true );
	cbEolVisibility->setChecked( sc->eolVisibility() );
	gbWhitespaceVisibilityEnabled->setChecked( sc->whitespaceVisibility() != QsciScintilla::WsInvisible );
	if ( bgWhitespaceVisibility->button( sc->whitespaceVisibility() ) )
		bgWhitespaceVisibility->button( sc->whitespaceVisibility() )->setChecked( true );
	gbWrapModeEnabled->setChecked( sc->wrapMode() != QsciScintilla::WrapNone );
	if ( bgWrapMode->button( sc->wrapMode() ) )
		bgWrapMode->button( sc->wrapMode() )->setChecked( true );
	gbWrapVisualFlagsEnabled->setChecked( sc->wrapVisualFlagsEnabled() );
	if ( bgStartWrapVisualFlag->button( sc->startWrapVisualFlag() ) )
		bgStartWrapVisualFlag->button( sc->startWrapVisualFlag() )->setChecked( true );
	if ( bgEndWrapVisualFlag->button( sc->endWrapVisualFlag() ) )
		bgEndWrapVisualFlag->button( sc->endWrapVisualFlag() )->setChecked( true );
	sWrappedLineIndentWidth->setValue( sc->wrappedLineIndentWidth() );
	// Source APIs
	for ( int i = 0; i < cbSourceAPIsLanguages->count(); i++ )
		cbSourceAPIsLanguages->setItemData( i, s->value( QString( "%1/Editor/SourceAPIs/%2" ).arg( SettingsPath, cbSourceAPIsLanguages->itemText( i ) ) ).toStringList() );
	if ( cbSourceAPIsLanguages->count() > 0 )
		cbSourceAPIsLanguages->setCurrentIndex( 0 );
	//  Lexers Associations
	QHash<QString, QStringList> l = pQScintilla::instance()->suffixes();
	foreach ( QString k, l.keys() )
	{
		foreach ( QString e, l.value( k ) )
		{
			QTreeWidgetItem* it = new QTreeWidgetItem( twLexersAssociations );
			it->setText( 0, e );
			it->setText( 1, k );
		}
	}
	//  Lexers Highlighting
	pQScintilla::instance()->lexersSettings();
	if ( cbLexersHighlightingLanguages->count() )
		on_cbLexersHighlightingLanguages_currentIndexChanged( cbLexersHighlightingLanguages->itemText( 0 ) );

	//  Abbreviations
	sp = "Scintilla/Abbreviations";
	// enable abbreviations
	cbAbbreviationsAutoWordCompletion->setChecked( s->value( sp +"/Enabled" ).toBool() );

	foreach ( pAbbreviation* a, pQScintilla::instance()->abbreviations() )
	{
		QTreeWidgetItem* it = new QTreeWidgetItem( twAbbreviations );
		it->setText( 0, a->Template );
		it->setText( 1, a->Description );
		it->setText( 2, a->Language );
		it->setData( 0, Qt::UserRole, a->Code );
	}

	// Tools
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
	pQScintilla* sc = pQScintilla::instance();
	//  General
	sc->setAutoSyntaxCheck( cbAutoSyntaxCheck->isChecked() );
	sc->setConvertTabsUponOpen( cbConvertTabsUponOpen->isChecked() );
	sc->setCreateBackupUponOpen( cbCreateBackupUponOpen->isChecked() ) ;
	sc->setAutoEolConversion( cbAutoEolConversion->isChecked() );
	sc->setDefaultEncoding( cbDefaultEncoding->currentText() );
	sc->setSelectionBackgroundColor( iconBackgroundColor( tbSelectionBackground->icon() ) );
	sc->setSelectionForegroundColor( iconBackgroundColor( tbSelectionForeground->icon() ) );
	//  Auto Completion
	sc->setAutoCompletionSource( QsciScintilla::AcsNone );
	if ( gbAutoCompletionEnabled->isChecked() )
		sc->setAutoCompletionSource( (QsciScintilla::AutoCompletionSource)bgAutoCompletionSource->checkedId() );
	sc->setAutoCompletionCaseSensitivity( cbAutoCompletionCaseSensitivity->isChecked() );
	sc->setAutoCompletionReplaceWord( cbAutoCompletionReplaceWord->isChecked() );
	sc->setAutoCompletionShowSingle( cbAutoCompletionShowSingle->isChecked() );
	sc->setAutoCompletionThreshold( sAutoCompletionThreshold->value() );
	//  Call Tips
	sc->setCallTipsStyle( QsciScintilla::CallTipsNone );
	if ( gbCalltipsEnabled->isChecked() )
		sc->setCallTipsStyle( (QsciScintilla::CallTipsStyle)bgCallTipsStyle->checkedId() );
	sc->setCallTipsVisible( sCallTipsVisible->value() );
	sc->setCallTipsBackgroundColor( iconBackgroundColor( tbCalltipsBackground->icon() ) );
	sc->setCallTipsForegroundColor( iconBackgroundColor( tbCalltipsForeground->icon() ) );
	sc->setCallTipsHighlightColor( iconBackgroundColor( tbCalltipsHighlight->icon() ) );
	//  Indentation
	sc->setAutoIndent( cbAutoIndent->isChecked()  );
	sc->setBackspaceUnindents( cbBackspaceUnindents->isChecked() );
	sc->setIndentationGuides( cbIndentationGuides->isChecked() );
	sc->setIndentationsUseTabs( cbIndentationUseTabs->isChecked() );
	sc->setTabIndents( cbTabIndents->isChecked() );
	sc->setTabWidth( sIndentationTabWidth->value() );
	sc->setIndentationWidth( sIndentationWidth->value() );
	sc->setIndentationGuidesBackgroundColor( iconBackgroundColor( tbIndentationGuidesBackground->icon() ) );
	sc->setIndentationGuidesForegroundColor( iconBackgroundColor( tbIndentationGuidesForeground->icon() ) );
	//  Brace Matching
	sc->setBraceMatching( QsciScintilla::NoBraceMatch );
	if ( gbBraceMatchingEnabled->isChecked() )
		sc->setBraceMatching( (QsciScintilla::BraceMatch)bgBraceMatch->checkedId() );
	sc->setMatchedBraceBackgroundColor( iconBackgroundColor( tbMatchedBraceBackground->icon() ) );
	sc->setMatchedBraceForegroundColor( iconBackgroundColor( tbMatchedBraceForeground->icon() ) );
	sc->setUnmatchedBraceBackgroundColor( iconBackgroundColor( tbUnmatchedBraceBackground->icon() ) );
	sc->setUnmatchedBraceForegroundColor( iconBackgroundColor( tbUnmatchedBraceForeground->icon() ) );
	//  Edge Mode
	sc->setEdgeMode( QsciScintilla::EdgeNone );
	if ( gbEdgeModeEnabled->isChecked() )
		sc->setEdgeMode( (QsciScintilla::EdgeMode)bgEdgeMode->checkedId() );
	sc->setEdgeColumn( sEdgeColumnNumber->value() );
	sc->setEdgeColor( iconBackgroundColor( tbEdgeColor->icon() ) );
	//  Caret
	sc->setCaretLineVisible( gbCaretLineVisible->isChecked() );
	sc->setCaretLineBackgroundColor( iconBackgroundColor( tbCaretLineBackground->icon() ) );
	sc->setCaretForegroundColor( iconBackgroundColor( tbCaretForeground->icon() ) );
	sc->setCaretWidth( sCaretWidth->value() );
	//  Margins
	sc->setLineNumbersMarginEnabled( gbLineNumbersMarginEnabled->isChecked() );
	sc->setLineNumbersMarginWidth( sLineNumbersMarginWidth->value() );
	sc->setLineNumbersMarginAutoWidth( cbLineNumbersMarginAutoWidth->isChecked() );
	sc->setFolding( QsciScintilla::NoFoldStyle );
	if ( gbFoldMarginEnabled->isChecked() )
		sc->setFolding( (QsciScintilla::FoldStyle)bgFoldStyle->checkedId() );
	sc->setFoldMarginForegroundColor( iconBackgroundColor( tbFoldMarginForeground->icon() ) );
	sc->setFoldMarginBackgroundColor( iconBackgroundColor( tbFoldMarginBackground->icon() ) );
	sc->setMarginsEnabled( gbMarginsEnabled->isChecked() );
	sc->setMarginsForegroundColor( iconBackgroundColor( tbMarginsForeground->icon() ) );
	sc->setMarginsBackgroundColor( iconBackgroundColor( tbMarginsBackground->icon() ) );
	sc->setMarginsFont( tbMarginsFont->font() );
	//  Special Characters
	sc->setEolMode( (QsciScintilla::EolMode)bgEolMode->checkedId() );
	sc->setEolVisibility( cbEolVisibility->isChecked() );
	sc->setWhitespaceVisibility( QsciScintilla::WsInvisible );
	if ( gbWhitespaceVisibilityEnabled->isChecked() )
		sc->setWhitespaceVisibility( (QsciScintilla::WhitespaceVisibility)bgWhitespaceVisibility->checkedId() );
	sc->setWrapMode( QsciScintilla::WrapNone );
	if ( gbWrapModeEnabled->isChecked() )
		sc->setWrapMode( (QsciScintilla::WrapMode)bgWrapMode->checkedId() );
	sc->setWrapVisualFlagsEnabled( gbWrapVisualFlagsEnabled->isChecked() );
	sc->setStartWrapVisualFlag( QsciScintilla::WrapFlagNone );
	if ( gbWrapVisualFlagsEnabled->isChecked() )
		sc->setStartWrapVisualFlag( (QsciScintilla::WrapVisualFlag)bgStartWrapVisualFlag->checkedId() );
	sc->setEndWrapVisualFlag( QsciScintilla::WrapFlagNone );
	if ( gbWrapVisualFlagsEnabled->isChecked() )
		sc->setEndWrapVisualFlag( (QsciScintilla::WrapVisualFlag)bgEndWrapVisualFlag->checkedId() );
	sc->setWrappedLineIndentWidth( sWrappedLineIndentWidth->value() );
	// Source APIs
	sp = QString( "%1/Editor/SourceAPIs/" ).arg( SettingsPath );
	for ( int i = 0; i < cbSourceAPIsLanguages->count(); i++ )
		s->setValue( sp +cbSourceAPIsLanguages->itemText( i ), cbSourceAPIsLanguages->itemData( i ).toStringList() );
	//  Lexers Associations
	sp = QString( "%1/Editor/LexersAssociations/" ).arg( SettingsPath );
	// remove old associations
	s->remove( sp );
	// write new ones
	for ( int i = 0; i < twLexersAssociations->topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* it = twLexersAssociations->topLevelItem( i );
		s->setValue( sp +it->text( 0 ), it->text( 1 ) );
	}
	//  Lexers Highlighting
	pQScintilla::instance()->writeLexersSettings();

	//  Abbreviations
	sp = "Scintilla/Abbreviations";
	// remove key
	s->remove( sp );
	// enable abbreviations
	s->setValue( sp +"/Enabled", cbAbbreviationsAutoWordCompletion->isChecked() );
	// write new ones
	s->beginWriteArray( sp );
	for ( int i = 0; i < twAbbreviations->topLevelItemCount(); i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = twAbbreviations->topLevelItem( i );

		s->setValue( "Template", it->text( 0 ).trimmed() );
		s->setValue( "Description", it->text( 1 ).trimmed() );
		s->setValue( "Language", it->text( 2 ) );
		s->setValue( "Code", it->data( 0, Qt::UserRole ).toString().trimmed() );
	}
	s->endArray();

	// Tools
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
					swPages->setCurrentIndex( i +11 );
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

void UISettings::tbColours_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	QColor c = QColorDialog::getColor( iconBackgroundColor( tb->icon() ), window() );
	if ( c.isValid() )
		tb->setIcon( colourizedPixmap( c ) );
}

void UISettings::tbFonts_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	bool b;
	QFont f = QFontDialog::getFont( &b, tb->font(), window() );
	if ( b )
		tb->setFont( f );
}

void UISettings::cbSourceAPIsLanguages_beforeChanged( int i )
{
	if ( i == cbSourceAPIsLanguages->currentIndex() )
	{
		QStringList l;
		for ( int j = 0; j < lwSourceAPIs->count(); j++ )
			l << lwSourceAPIs->item( j )->text();
		cbSourceAPIsLanguages->setItemData( i, l );
	}
}

void UISettings::on_cbSourceAPIsLanguages_currentIndexChanged( int i )
{
	lwSourceAPIs->clear();
	lwSourceAPIs->addItems( cbSourceAPIsLanguages->itemData( i ).toStringList() );
}

void UISettings::on_pbSourceAPIsDelete_clicked()
{
	// get selected item
	QListWidgetItem* it = lwSourceAPIs->selectedItems().value( 0 );
	if ( it )
	{
		delete it;
		cbSourceAPIsLanguages_beforeChanged( cbSourceAPIsLanguages->currentIndex() );
	}
}

void UISettings::on_pbSourceAPIsAdd_clicked()
{
	QString s = leSourceAPIs->text();
	if ( !s.isEmpty() && lwSourceAPIs->findItems( s, Qt::MatchFixedString ).count() == 0 )
	{
		lwSourceAPIs->addItem( s );
		leSourceAPIs->clear();
		cbSourceAPIsLanguages_beforeChanged( cbSourceAPIsLanguages->currentIndex() );
	}
}

void UISettings::on_pbSourceAPIsBrowse_clicked()
{
	QString s = QFileDialog::getOpenFileName( window(), tr( "Select API file" ), QString::null, tr( "API Files (*.api);;All Files (*)" ) );
	if ( !s.isNull() )
		leSourceAPIs->setText( s );
}

void UISettings::on_twLexersAssociations_itemSelectionChanged()
{
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value( 0 );
	if ( it )
	{
		leLexersAssociationsFilenamePattern->setText( it->text( 0 ) );
		cbLexersAssociationsLanguages->setCurrentIndex( cbLexersAssociationsLanguages->findText( it->text( 1 ) ) );
	}
}

void UISettings::on_pbLexersAssociationsAddChange_clicked()
{
	QString f = leLexersAssociationsFilenamePattern->text();
	QString l = cbLexersAssociationsLanguages->currentText();
	if ( f.isEmpty() || l.isEmpty() )
		return;
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value( 0 );
	if ( !it || it->text( 0 ) != f )
	{
		// check if item with same parameters already exists
		QList<QTreeWidgetItem*> l = twLexersAssociations->findItems( f, Qt::MatchFixedString );
		if ( l.count() )
			it = l.at( 0 );
		else
			it = new QTreeWidgetItem( twLexersAssociations );
	}
	it->setText( 0, f );
	it->setText( 1, l );
	twLexersAssociations->setCurrentItem( 0 );
	twLexersAssociations->selectionModel()->clear();
	leLexersAssociationsFilenamePattern->clear();
	cbLexersAssociationsLanguages->setCurrentIndex( -1 );
}

void UISettings::on_pbLexersAssociationsDelete_clicked()
{
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value( 0 );
	if ( it )
	{
		delete it;
		twLexersAssociations->setCurrentItem( 0 );
		twLexersAssociations->selectionModel()->clear();
		leLexersAssociationsFilenamePattern->clear();
		cbLexersAssociationsLanguages->setCurrentIndex( -1 );
	}
}

void UISettings::on_cbLexersHighlightingLanguages_currentIndexChanged( const QString& s )
{
	QsciLexer* l = pQScintilla::instance()->lexersSettings().value( s );
	lwLexersHighlightingElements->clear();
	for ( int i = 0; i < 128; i++ )
	{
		QString n = l->description( i );
		if ( !n.isEmpty() )
		{
			QListWidgetItem* it = new QListWidgetItem( lwLexersHighlightingElements );
			it->setText( n );
			it->setForeground( l->color( i ) );
			it->setBackground( l->paper( i ) );
			it->setFont( l->font( i ) );
			it->setData( Qt::UserRole, i );
		}
	}

	QVariant v;

	// fold comments
	v = pQScintilla::instance()->lexerProperty( "foldComments", l );
	cbLexersHighlightingFoldComments->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldComments->setChecked( v.toBool() );

	// fold compact
	v = pQScintilla::instance()->lexerProperty( "foldCompact", l );
	cbLexersHighlightingFoldCompact->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldCompact->setChecked( v.toBool() );

	// fold quotes
	v = pQScintilla::instance()->lexerProperty( "foldQuotes", l );
	cbLexersHighlightingFoldQuotes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldQuotes->setChecked( v.toBool() );

	// fold directives
	v = pQScintilla::instance()->lexerProperty( "foldDirectives", l );
	cbLexersHighlightingFoldDirectives->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldDirectives->setChecked( v.toBool() );

	// fold at begin
	v = pQScintilla::instance()->lexerProperty( "foldAtBegin", l );
	cbLexersHighlightingFoldAtBegin->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtBegin->setChecked( v.toBool() );

	// fold at parenthesis
	v = pQScintilla::instance()->lexerProperty( "foldAtParenthesis", l );
	cbLexersHighlightingFoldAtParenthesis->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtParenthesis->setChecked( v.toBool() );

	// fold at else
	v = pQScintilla::instance()->lexerProperty( "foldAtElse", l );
	cbLexersHighlightingFoldAtElse->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtElse->setChecked( v.toBool() );

	// fold preprocessor
	v = pQScintilla::instance()->lexerProperty( "foldPreprocessor", l );
	cbLexersHighlightingFoldPreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldPreprocessor->setChecked( v.toBool() );

	// style preprocessor
	v = pQScintilla::instance()->lexerProperty( "stylePreprocessor", l );
	cbLexersHighlightingStylePreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingStylePreprocessor->setChecked( v.toBool() );

	// indent opening brace
	cbLexersHighlightingIndentOpeningBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiOpening );

	// indent closing brace
	cbLexersHighlightingIndentClosingBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiClosing );

	// case sensitive tags
	v = pQScintilla::instance()->lexerProperty( "caseSensitiveTags", l );
	cbLexersHighlightingCaseSensitiveTags->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingCaseSensitiveTags->setChecked( v.toBool() );

	// backslash escapes
	v = pQScintilla::instance()->lexerProperty( "backslashEscapes", l );
	cbLexersHighlightingBackslashEscapes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingBackslashEscapes->setChecked( v.toBool() );

	// indentation warning
	v = pQScintilla::instance()->lexerProperty( "indentationWarning", l );
	lLexersHighlightingIndentationWarning->setVisible( v.isValid() );
	cbLexersHighlightingIndentationWarning->setVisible( lLexersHighlightingIndentationWarning->isVisible() );
	if ( v.isValid() )
		cbLexersHighlightingIndentationWarning->setCurrentIndex( cbLexersHighlightingIndentationWarning->findData( v.toInt() ) );
}

void UISettings::on_lwLexersHighlightingElements_itemSelectionChanged()
{
	QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
	if ( it )
		cbLexersHighlightingFillEol->setChecked( pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() )->eolFill( it->data( Qt::UserRole ).toInt() ) );
}

void UISettings::lexersHighlightingColour_clicked()
{
	// get sender
	QObject* o = sender();

	// color
	QColor c;

	// element colour
	if ( o == pbLexersHighlightingForeground || o == pbLexersHighlightingBackground )
	{
		// get item
		QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );

		// cancel if no item
		if ( !it )
			return;

		// get color
		c = QColorDialog::getColor( o == pbLexersHighlightingForeground ? it->foreground().color() : it->background().color(), window() );

		// apply color
		if ( c.isValid() )
		{
			if ( o == pbLexersHighlightingForeground )
			{
				it->setForeground( c );
				pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() )->setColor( c, it->data( Qt::UserRole ).toInt() );
			}
			else if ( o == pbLexersHighlightingBackground )
			{
				it->setBackground( c );
				pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() )->setPaper( c, it->data( Qt::UserRole ).toInt() );
			}
		}
	}
	// gobal color
	else if ( o == pbLexersHighlightingAllForeground || o == pbLexersHighlightingAllBackground )
	{
		// get lexer
		QsciLexer* l = pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() );

		// get color
		c = QColorDialog::getColor( o == pbLexersHighlightingAllForeground ? l->color( -1 ) : l->paper( -1 ), window() );

		// apply
		if ( c.isValid() )
		{
			if ( o == pbLexersHighlightingAllForeground )
				l->setColor( c, -1 );
			else if ( o == pbLexersHighlightingAllBackground )
				l->setPaper( c, -1 );

			// refresh
			on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
		}
	}
}

void UISettings::lexersHighlightingFont_clicked()
{
	// get sender
	QObject* o = sender();

	bool b;
	QFont f;

	if ( o == pbLexersHighlightingFont )
	{
		// get item
		QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );

		// cancel if no item
		if ( !it )
			return;

		// get font
		f = QFontDialog::getFont( &b, it->font(), window() );

		// apply
		if ( b )
		{
			it->setFont( f );
			pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() )->setFont( f, it->data( Qt::UserRole ).toInt() );
		}
	}
	else if ( o == pbLexersHighlightingAllFont )
	{
		// get lexer
		QsciLexer* l = pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() );

		// get font
		f = QFontDialog::getFont( &b, l->font( -1 ), window() );

		// apply
		if ( b )
		{
			l->setFont( f, -1 );
			on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
		}
	}
}

void UISettings::on_cbLexersHighlightingFillEol_clicked( bool b )
{
	QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
	if ( it )
		pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() )->setEolFill( b, it->data( Qt::UserRole ).toInt() );
}

void UISettings::cbLexersHighlightingProperties_clicked( bool b )
{
	// get check box
	QCheckBox* cb = qobject_cast<QCheckBox*>( sender() );
	if ( !cb )
		return;

	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() );

	// set lexer properties
	if ( cb == cbLexersHighlightingIndentOpeningBrace || cb == cbLexersHighlightingIndentClosingBrace )
	{
		if ( cbLexersHighlightingIndentOpeningBrace->isChecked() && cbLexersHighlightingIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening | QsciScintilla::AiClosing );
		else if ( cbLexersHighlightingIndentOpeningBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening );
		else if ( cbLexersHighlightingIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiClosing );
		else
			l->setAutoIndentStyle( QsciScintilla::AiMaintain );
	}
	else
		pQScintilla::instance()->setLexerProperty( cb->statusTip(), l, b );
}

void UISettings::on_cbLexersHighlightingIndentationWarning_currentIndexChanged( int i )
{
	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() );

	// set lexer properties
	pQScintilla::instance()->setLexerProperty( cbLexersHighlightingIndentationWarning->statusTip(), l, cbLexersHighlightingIndentationWarning->itemData( i ) );
}

void UISettings::on_pbLexersHighlightingReset_clicked()
{
	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexersSettings().value( cbLexersHighlightingLanguages->currentText() );

	// reset and refresh
	if ( l )
	{
		pQScintilla::instance()->resetLexer( l );
		on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
	}
}

void UISettings::on_twAbbreviations_itemSelectionChanged()
{
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );

	if ( it )
		teAbbreviationsCode->setPlainText( it->data( 0, Qt::UserRole ).toString() );

	// enable/disable according to selection
	teAbbreviationsCode->setEnabled( it );
}

void UISettings::on_pbAbbreviationsAdd_clicked()
{
	UIAddAbbreviation::edit( twAbbreviations );
}

void UISettings::on_pbAbbreviationsRemove_clicked()
{
	delete twAbbreviations->selectedItems().value( 0 );
	teAbbreviationsCode->clear();
}

void UISettings::on_teAbbreviationsCode_textChanged()
{
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );

	if ( it )
		it->setData( 0, Qt::UserRole, teAbbreviationsCode->toPlainText() );
}

void UISettings::accept()
{
	apply();
	QDialog::accept();
}

void UISettings::apply()
{
	saveSettings();
	pQScintilla::instance()->applyProperties();
}