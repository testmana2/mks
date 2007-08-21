#include "MonkeyEditor.h"
#include "Settings.h"
#include "qscintillaSearch.h"
//
#include "qscilexer.h"
#include "qsciprinter.h"
//
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QPrintDialog>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QDateTime>
//
MonkeyEditor::MonkeyEditor( QWidget* p )
	: QsciScintilla( p ), mCopyAvailable( false ), mCodec( 0 ), mTimer( new QTimer( this ) )
{
	connect( mTimer, SIGNAL( timeout() ), this, SLOT( saveFile() ) );
	readSettings();
/*
	// register bookmarks icons
	pBookmark* b;
	for ( int i = bm0; i < bm9 +1; i++ )
	{
		b = new pBookmark;
		b->Pixmap = new QPixmap( QString( ":/Icons/Icons/EditorBookmark%1" ).arg( i ) );
		b->Index = markerDefine( *b->Pixmap );
		b->Handle = -1;
		mBookmarks[(Bookmarks)i] = b;
	}
	//
	connect( this, SIGNAL( copyAvailable( bool ) ), this, SLOT( setCopyAvailable( bool ) ) );
	connect( QApplication::clipboard(), SIGNAL( dataChanged() ), this, SLOT( clipboard_dataChanged() ) );
	connect( this, SIGNAL( textChanged() ), this, SLOT( textChanged() ) );
	connect( this, SIGNAL( cursorPositionChanged( int, int ) ), this, SLOT( cursorPositionChanged( int, int ) ) );
	connect( this,SIGNAL( marginClicked( int, int, Qt::KeyboardModifiers ) ), this, SLOT( marginClicked( int, int, Qt::KeyboardModifiers ) ) );
	// create action
	mActions << new pAction( tr( "Matching Brace" ), this, SLOT( moveToMatchingBrace() ), pAction::TextLength, tr( "Ctrl+M" ) );
	mActions << new pAction( tr( "Undo" ), this, SLOT( undo() ), pAction::UndoAvailable, tr( "Ctrl+Z" ) );
	mActions << new pAction( tr( "Redo" ), this, SLOT( redo() ), pAction::RedoAvailable, tr( "Ctrl+Y" ) );
	mActions << new pAction( tr( "Separator" ) );
	mActions << new pAction( tr( "Copy" ), this, SLOT( copy() ), pAction::CopyAvailable, tr( "Ctrl+C" ) );
	mActions << new pAction( tr( "Cut" ), this, SLOT( cut() ), pAction::CopyAvailable, tr( "Ctrl+X" ) );
	mActions << new pAction( tr( "Paste" ), this, SLOT( paste() ), pAction::PasteAvailable, tr( "Ctrl+V" ) );
	mActions << new pAction( tr( "Separator" ) );
	mActions << new pAction( tr( "Select All" ), this, SLOT( selectAll() ), pAction::NotCopyAvailableAndTextLength, tr( "Ctrl+A" ) );
	mActions << new pAction( tr( "Select None" ), this, SLOT( selectNone() ), pAction::CopyAvailable, tr( "Ctrl+Shift+A" ) );
	mActions << new pAction( tr( "Separator" ) );
	mActions << new pAction( tr( "Go To Line..." ), this, SLOT( goToLine() ), pAction::TextLength, tr( "Ctrl+G" ) );
	mActions << new pAction( tr( "Search - Replace..." ), this, SLOT( searchReplace() ), pAction::TextLength, tr( "Ctrl+F" ) );
	// set sensitivity margin for mouse click
	setMarginSensitivity( 0, true );
*/
}
//
MonkeyEditor::~MonkeyEditor()
{
	foreach ( pBookmark* b, mBookmarks )
	{
		delete b->Pixmap;
		delete b;
	}
	mBookmarks.clear();
	//
	qDeleteAll( mActions );
}
//
MonkeyEditor* MonkeyEditor::createEditorForFilename( const QString& s, const QPoint& p, QTextCodec* c, QWidget* w )
{
	MonkeyEditor* e = 0;
	if ( QFile::exists( s ) )
	{
		e = new MonkeyEditor( w );
		e->openFile( s, p, c );
	}
	return e;
}
//
void MonkeyEditor::readSettings()
{
	Settings* s = Settings::current();
	QString sp = "Settings/Editor/AutoCompletion";
	// auto completion
	if ( s->value( sp +"/Enabled", true ).toBool() )
	{
		setAutoCompletionCaseSensitivity( s->value( sp +"/CaseSensitive", true ).toBool() );
		setAutoCompletionReplaceWord( s->value( sp +"/ReplaceWord", false ).toBool() );
		setAutoCompletionShowSingle( s->value( sp +"/ShowSingle", false ).toBool() );
		setAutoCompletionSource( (QsciScintilla::AutoCompletionSource)s->value( sp +"/APISource", QsciScintilla::AcsAPIs ).toInt() );
		setAutoCompletionThreshold( s->value( sp +"/Threshold", 0 ).toInt() );
	}
	// calltips
	sp = "Settings/Editor/Calltips";
	if ( s->value( sp +"/Enabled", true ).toBool() )
	{
		setCallTipsStyle( (QsciScintilla::CallTipsStyle)s->value( sp +"/Style", QsciScintilla::CallTipsNoContext ).toInt() );
		setCallTipsVisible( s->value( sp +"/Visible", -1 ).toInt() );
	}
	else
		setCallTipsStyle( (QsciScintilla::CallTipsStyle)s->value( sp +"/Style", QsciScintilla::CallTipsNone ).toInt() );
	// colours
	sp = QString( "Settings/Editor/Colours" );
	setCallTipsBackgroundColor( s->value( sp +"/CalltipsBackground", Qt::white ).value<QColor>() );
	setCallTipsForegroundColor( s->value( sp +"/CalltipsForeground", Qt::lightGray ).value<QColor>() );
	setCallTipsHighlightColor( s->value( sp +"/CalltipsHighlight", Qt::darkBlue ).value<QColor>() );
	setMatchedBraceBackgroundColor( s->value( sp +"/MatchedBracesBackground", Qt::white ).value<QColor>() );
	setMatchedBraceForegroundColor( s->value( sp +"/MatchedBraces", Qt::red ).value<QColor>() );
	setUnmatchedBraceBackgroundColor( s->value( sp +"/UnmatchedBracesBackground", Qt::white ).value<QColor>() );
	setUnmatchedBraceForegroundColor( s->value( sp +"/UnmatchedBraces", Qt::blue ).value<QColor>() );
	setCaretForegroundColor( s->value( sp +"/CaretForeground", Qt::black ).value<QColor>() );
	setCaretLineBackgroundColor( s->value( sp +"/CaretLineBackground", Qt::white ).value<QColor>() );
	// general
	// margin 0 = line numbers
	// margin 2 = fold
	sp = QString( "Settings/Editor/General" );
	setTabWidth( s->value( sp +"/TabWidth", 4 ).toInt() );
	setIndentationWidth( s->value( sp +"/IndentationWidth", 1 ).toInt() );
	setMarginWidth( 0, 0 );
	if ( s->value( sp +"/ShowLineNumbersMargin", true ).toBool() )
	{
		setMarginWidth( 0, QString().fill( '0', s->value( sp +"/LineNumbersWidth", 4 ).toInt() ) );
		setMarginLineNumbers( 0, s->value( sp +"/ShowLineNumbersMargin", true ).toBool() );
	}
	if ( !s->value( sp +"/ShowFoldMargin", true ).toBool() )
		setMarginWidth( 2, 0 );
	setWhitespaceVisibility( WsInvisible );
	if ( s->value( sp +"/ShowWhitespace", false ).toBool() )
		setWhitespaceVisibility( WsVisible );
	setEolVisibility( s->value( sp +"/ShowEndOfLine", false ).toBool() );
	setIndentationGuides( s->value( sp +"/ShowIndentationGuides", true ).toBool() );
	setIndentationsUseTabs( s->value( sp +"/UseTabsForIndentations", false ).toBool() );
	setTabIndents( s->value( sp +"/TabKeyIndents", true ).toBool() );
	setAutoIndent( s->value( sp +"/AutoIndentation", true ).toBool() );
	// style
	sp = QString( "Settings/Editor/Style" );
	setEolMode( (QsciScintilla::EolMode)s->value( sp +"/EOLMode", QsciScintilla::EolWindows ).toInt() );
	setEdgeMode( (QsciScintilla::EdgeMode)s->value( sp +"/FoldingStyle", QsciScintilla::NoFoldStyle ).toInt() );
	QFont f;
	if ( f.fromString( s->value( sp +"/LineNumbersFont" ).toString() ) )
		setMarginsFont( f );
	if ( f.fromString( s->value( sp +"/MonospacedFont" ).toString() ) )
		setFont( f );
	setCaretLineVisible( s->value( sp +"/CaretLineVisible", false ).toBool() );
	// s->value( sp +"/ColourizeSelectedText", false ).toBool()
	setCaretWidth( s->value( sp +"/CaretWidth", 1 ).toInt() );
	setEdgeMode( (QsciScintilla::EdgeMode)s->value( sp +"/EdgeMode", QsciScintilla::EdgeNone ).toInt() );
	setEdgeColor( s->value( sp +"/EdgeModeBackground", Qt::gray ).value<QColor>() );
	setEdgeColumn( s->value( sp +"/EdgeModeColumnNumber", 80 ).toInt() );
}
//
void MonkeyEditor::focusInEvent( QFocusEvent* e )
{
	QsciScintilla::focusInEvent( e );
	qscintillaSearch::self()->setEditor( this );
	emit focused( true );
}
//
void MonkeyEditor::focusOutEvent( QFocusEvent* e )
{
	QsciScintilla::focusOutEvent( e );
	emit focused( false );
}
//
void MonkeyEditor::contextMenuEvent( QContextMenuEvent* e )
{
	QMenu m( this );
	foreach ( pAction* a, mActions )
	{
		if ( a->Caption == tr( "Separator" ) )
			m.addSeparator();
		else
			m.addAction( QIcon( a->Pixmap ), a->Caption, a->Receiver, a->Slot, a->Shortcut )->setEnabled( getState( a->State ) );
	}
	m.exec( e->globalPos() );
}
//
void MonkeyEditor::keyPressEvent( QKeyEvent* e )
{
	// if auto repeat, process and quit
	if ( e->isAutoRepeat() )
	{
		QsciScintilla::keyPressEvent( e );
		return;
	}
	// get cursor position
	int l, i;
	getCursorPosition( &l, &i );
	// if control and alt
	if ( ( ( e->modifiers() & Qt::ControlModifier )  == Qt::ControlModifier ) && 
		( ( e->modifiers() & Qt::AltModifier ) == Qt::AltModifier ) )
	{
		switch ( e->key() )
		{
		case Qt::Key_0:
			checkBookmarks( bm0, l );
			break;
		case Qt::Key_1:
			checkBookmarks( bm1, l );
			break;
		case Qt::Key_2:
			checkBookmarks( bm2, l );
			break;
		case Qt::Key_3:
			checkBookmarks( bm3, l );
			break;
		case Qt::Key_4:
			checkBookmarks( bm4, l );
			break;
		case Qt::Key_5:
			checkBookmarks( bm5, l );
			break;
		case Qt::Key_6:
			checkBookmarks( bm6, l );
			break;
		case Qt::Key_7:
			checkBookmarks( bm7, l );
			break;
		case Qt::Key_8:
			checkBookmarks( bm8, l );
			break;
		case Qt::Key_9:
			checkBookmarks( bm9,l );
			break;
		default:
			QsciScintilla::keyPressEvent( e );
			break;
		}
	}
	// if control
	else if ( ( e->modifiers() & Qt::ControlModifier ) == Qt::ControlModifier )
	{
		// get line
		switch ( e->key() )
		{
		case Qt::Key_B:
			//emit toggleBreakPoint( l, false );
			break;
		case Qt::Key_0:
			l = markerLine( mBookmarks[bm0]->Handle );
			break;
		case Qt::Key_1:
			l = markerLine( mBookmarks[bm1]->Handle );
			break;
		case Qt::Key_2:
			l = markerLine( mBookmarks[bm2]->Handle );
			break;
		case Qt::Key_3:
			l = markerLine( mBookmarks[bm3]->Handle );
			break;
		case Qt::Key_4:
			l = markerLine( mBookmarks[bm4]->Handle );
			break;
		case Qt::Key_5:
			l = markerLine( mBookmarks[bm5]->Handle );
			break;
		case Qt::Key_6:
			l = markerLine( mBookmarks[bm6]->Handle );
			break;
		case Qt::Key_7:
			l = markerLine( mBookmarks[bm7]->Handle );
			break;
		case Qt::Key_8:
			l = markerLine( mBookmarks[bm8]->Handle );
			break;
		case Qt::Key_9:
			l = markerLine( mBookmarks[bm9]->Handle );
			break;
		default:
			l = -1;
			QsciScintilla::keyPressEvent( e );
			break;
		}
		// go to line
		if ( l != -1 )
		{
			// go to line
			setCursorPosition( l, 0 );
			// if line is not visible, show it
			ensureLineVisible( l );
		}
	}
	else
		QsciScintilla::keyPressEvent( e );
}
//
void MonkeyEditor::checkBookmarks( Bookmarks b, int l )
{
	if ( markerLine( mBookmarks[b]->Handle ) == l )
	{
		markerDeleteAll( mBookmarks[b]->Index );
		mBookmarks[b]->Handle = -1;
	}
	else
	{
		markerDeleteAll( mBookmarks[b]->Index );
		mBookmarks[b]->Handle = markerAdd( l, b );
	}
}
//
bool MonkeyEditor::pasteAvailable() const
{
	return QApplication::clipboard()->text( QClipboard::Clipboard ).length();
}
//
bool MonkeyEditor::copyAvailable() const
{
	return mCopyAvailable;
}
//
bool MonkeyEditor::getState( pAction::StateType s )
{
	switch ( s )
	{
	case pAction::TextLength:
		return text().length();
		break;
	case pAction::UndoAvailable:
		return isUndoAvailable();
		break;
	case pAction::RedoAvailable:
		return isRedoAvailable();
		break;
	case pAction::CopyAvailable:
		return mCopyAvailable;
		break;
	case pAction::PasteAvailable:
		return pasteAvailable();
		break;
	case pAction::NotCopyAvailableAndTextLength:
		return !mCopyAvailable && text().length();
		break;
	case pAction::Enabled:
	case pAction::None:
		return true;
		break;
	case pAction::Disabled:
	default:
		return false;
		break;
	}
}
//
void MonkeyEditor::print( bool b )
{
	QsciPrinter p;
	p.setWrapMode( WrapWord );
	if ( b )
	{
		p.printRange( this );
		return;
	}
	QPrintDialog d( &p );
	if ( d.exec() )
	{
		int f = -1, t = -1, i;
		if ( d.printRange() == QPrintDialog::Selection )
			getSelection( &f, &i, &t, &i ); 
		p.printRange( this, f, t );
	}
}
//
void MonkeyEditor::quickPrint()
{
	print( true );
}
//
void MonkeyEditor::cursorPositionChanged( int l, int p )
{
	emit cursorPositionChanged( QPoint( p, l ) );
}
//
void MonkeyEditor::textChanged()
{
	emit undoAvailable( isUndoAvailable() );
	emit redoAvailable( isRedoAvailable() );
}
//
void MonkeyEditor::clipboard_dataChanged()
{
	emit ( pasteAvailable() );
}
//
void MonkeyEditor::setCopyAvailable( bool b )
{
	mCopyAvailable = b;
}
//
void MonkeyEditor::marginClicked( int m, int l,  Qt::KeyboardModifiers k )
{
	Q_UNUSED( m );
	Q_UNUSED( k );
	Q_UNUSED( l );
	//emit toggleBreakPoint( l, false );
}
//
void MonkeyEditor::selectNone()
{
	selectAll( false );
}
//
void MonkeyEditor::searchReplace()
{
	qscintillaSearch::self()->setEditor( this );
	qscintillaSearch::self()->leSearch->setFocus();
	qscintillaSearch::self()->leSearch->selectAll();
	if ( !qscintillaSearch::self()->isVisible() )
		qscintillaSearch::self()->setVisible( true );
}
//
void MonkeyEditor::goToLine()
{
	bool b;
	int l, i;
	getCursorPosition( &l, &i );
	int j = QInputDialog::getInteger( this, tr( "Go To Line..." ), tr( "Enter the line you want to go:" ), l +1, 1, lines(), 1, &b );
	if ( b )
		setCursorPosition( j -1, 0 );
}
//
bool MonkeyEditor::openFile( const QString& s, const QPoint& p, QTextCodec* c )
{
	if ( isModified() )
		return false;
	//
	const QString fp = QFileInfo( s ).canonicalFilePath();
	QFile f( fp );
	if ( !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		QMessageBox::warning( this, tr( "MonkeyEditor" ),
			tr( "Cannot read file %1:\n%2." ).arg( fp ).arg( f.errorString() ) );
		return false;
	}
	// load file with c codec
	setCursor( Qt::WaitCursor );
	// remind properties
	mFilePath = fp;
	mCodec = c;
	// set correct lexer
	QString l = Settings::languageForFilename( mFilePath );
	if ( lexer() && lexer()->language() != l )
		delete lexer();
	setLexer( Settings::lexerForLanguage( l ) );
	// load file with correct codec
	QTextStream ts( &f );
	if ( c )
		ts.setCodec( c );
	setText( ts.readAll() );
	setModified( false );
	// read settings
	readSettings();
	// do backup
	if ( Settings::current()->value( "Settings/Editor/General/CreateBackupFileUponOpen", true ).toBool() )
		QFile::copy( fp, fp +QDateTime::currentDateTime().toString( ".yyyyMMdd_hhmm.backup" ) );
	// convert eol
	if ( Settings::current()->value( "Settings/Editor/General/AutomaticEndOfLineConversion", true ).toBool() )
		convertEols( eolMode() );
	// convert tabs
	if ( Settings::current()->value( "Settings/Editor/General/ConvertTabsUponOpen", false ).toBool() )
		convertTabs();
	// set auto save interval
	setAutoSaveInterval( Settings::current()->value( "Settings/Editor/General/AutoSaveInterval", 0 ).toInt() );
	// set cursor position
	if ( !p.isNull() )
		setCursorPosition( p.y(), p.x() );
	// s->value( "Settings/Editor/General/AutomaticSyntaxCheck", true ).toBool()
	unsetCursor();
	emit fileOpened( true );
	return true;
}
//
void MonkeyEditor::saveFile()
{
	// if no filename abort
	if ( mFilePath.isEmpty() || ! isModified() )
		return;
	// if can't open file abort
	QFile f( mFilePath );
	if ( !f.open( QFile::WriteOnly | QFile::Text ) )
	{
		QMessageBox::warning( this, tr( "MonkeyEditor" ),
			tr( "Cannot write file %1:\n%2." ).arg( mFilePath ).arg( f.errorString() ) );
		return;
	}
	// save file
	setCursor( Qt::WaitCursor );
	QTextStream ts( &f );
	if ( mCodec )
		ts.setCodec( mCodec );
	ts << text();
	unsetCursor();
	setModified( false );
}
//
void MonkeyEditor::closeFile()
{
	setCursor( Qt::WaitCursor );
	mFilePath.clear();
	mCodec = 0;
	clear();
	unsetCursor();
	setModified( false );
	setAutoSaveInterval( 0 );
	emit fileOpened( false );
}
//
void MonkeyEditor::setAutoSaveInterval( int i )
{
	mTimer->stop();
	if ( i > 0 )
	{
		mTimer->setInterval( 1000 *i *60 );
		mTimer->start();
	}
}
//
void MonkeyEditor::convertTabs( int i )
{
	int x, y;
	getCursorPosition( &y, &x );
	if ( i == -1 )
		i = tabWidth();
	bool b = findFirst( "\t", false, true, false, true, true );
	if ( b )
	{
		QString r = QString().fill( ' ', i );
		replace( r );
		while ( findNext() )
			replace( r );
	}
	setCursorPosition( y, x );
}
