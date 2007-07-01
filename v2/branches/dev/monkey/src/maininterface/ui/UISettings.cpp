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

	// resize column
	twAbbreviations->setColumnWidth( 0, 100 );
	twAbbreviations->setColumnWidth( 1, 300 );

	// read settings
	loadSettings();

	// connections
	foreach ( QToolButton* tb, pEditorColours->findChildren<QToolButton*>() )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbStyleEdgeModeBackgroundColour, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	foreach ( QCheckBox* cb, gbSyntaxHighlightingStyleElement->findChildren<QCheckBox*>() )
		if ( cb != cbSyntaxHighlightingFillToEndOfLine )
			connect( cb, SIGNAL( clicked( bool ) ), this, SLOT( cbProperties_clicked( bool ) ) );

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
	if ( cbSyntaxHighlightingLexerLanguage->count() )
		on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( cbSyntaxHighlightingLexerLanguage->itemText( 0 ) );

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

/*
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

/*
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
	QTreeWidgetItem* it = twHighlighterAssociations->selectedItems().value( 0 );
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
}

void UISettings::on_pbHighlighterAssociationDelete_clicked()
{
	QTreeWidgetItem* it = twHighlighterAssociations->selectedItems().value( 0 );
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
			it->setData( Qt::UserRole, i );
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

void UISettings::on_lwSyntaxHighlightingStyleElements_itemSelectionChanged()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->selectedItems().value( 0 );
	if ( it )
		cbSyntaxHighlightingFillToEndOfLine->setChecked( pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->eolFill( it->data( Qt::UserRole ).toInt() ) );
}

void UISettings::on_pbSyntaxHighlightingForegroundColour_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->selectedItems().value( 0 );
	if ( it )
	{
		QColor c = QColorDialog::getColor( it->foreground().color(), window() );
		if ( c.isValid() )
		{
			it->setForeground( c );
			pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setColor( c, it->data( Qt::UserRole ).toInt() );
		}
	}
}

void UISettings::on_pbSyntaxHighlightingBackgroundColour_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->selectedItems().value( 0 );
	if ( it )
	{
		QColor c = QColorDialog::getColor( it->background().color(), window() );
		if ( c.isValid() )
		{
			it->setBackground( c );
			pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setPaper( c, it->data( Qt::UserRole ).toInt() );
		}
	}
}

void UISettings::on_pbSyntaxHighlightingFont_clicked()
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->selectedItems().value( 0 );
	if ( it )
	{
		bool b;
		QFont f = QFontDialog::getFont( &b, it->font(), window() );
		if ( b )
		{
			it->setFont( f );
			pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setFont( f, it->data( Qt::UserRole ).toInt() );
		}
	}
}

void UISettings::on_cbSyntaxHighlightingFillToEndOfLine_clicked( bool b )
{
	QListWidgetItem* it = lwSyntaxHighlightingStyleElements->selectedItems().value( 0 );
	if ( it )
		pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() )->setEolFill( b, it->data( Qt::UserRole ).toInt() );
}

void UISettings::cbProperties_clicked( bool b )
{
	// get check box
	QCheckBox* cb = qobject_cast<QCheckBox*>( sender() );
	if ( !cb )
		return;

	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() );

	// set lexer properties
	if ( cb == cbIndentOpeningBrace || cb == cbIndentClosingBrace )
	{
		if ( cbIndentOpeningBrace->isChecked() && cbIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening | QsciScintilla::AiClosing );
		else if ( cbIndentOpeningBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening );
		else if ( cbIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiClosing );
		else
			l->setAutoIndentStyle( QsciScintilla::AiMaintain );
	}
	else
		pQScintilla::instance()->setProperty( cb->statusTip(), l, b );
}

void UISettings::on_cbIndentationWarning_currentIndexChanged( int i )
{
	// get lexer
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() );

	// set lexer properties
	pQScintilla::instance()->setProperty( cbIndentationWarning->statusTip(), l, cbIndentationWarning->itemData( i ) );
}

void UISettings::on_pbSyntaxHighlightingAllBackgroundColours_clicked()
{
	QsciLexer* l = pQScintilla::instance()->lexers().value( cbSyntaxHighlightingLexerLanguage->currentText() );
	QColor c = QColorDialog::getColor( l->defaultPaper(), window() );
	if ( c.isValid() )
	{
		l->setPaper( c, -1 );
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
		l->setFont( f, -1 );
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

void UISettings::on_twAbbreviations_itemSelectionChanged()
{
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );

	if ( it )
		teAbbreviation->setPlainText( it->data( 0, Qt::UserRole ).toString() );

	// enable/disable according to selection
	teAbbreviation->setEnabled( it );
}

void UISettings::on_pbAddAbbreviation_clicked()
{
	UIAddAbbreviation::edit( twAbbreviations );
}

void UISettings::on_pbRemoveAbbreviation_clicked()
{
	delete twAbbreviations->selectedItems().value( 0 );
	teAbbreviation->clear();
}

void UISettings::on_teAbbreviation_textChanged()
{
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );

	if ( it )
		it->setData( 0, Qt::UserRole, teAbbreviation->toPlainText() );
}

void UISettings::accept()
{
	saveSettings();
	QDialog::accept();
}
