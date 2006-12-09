#include "MonkeyEditor.h"
#include "qscintillaSearch.h"
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
// qscintilla include
#include <qsciprinter.h>
//
MonkeyEditor::MonkeyEditor( QWidget* p )
	: QsciScintilla( p ), mCopyAvailable( false ), mCodec( 0 )
{
	// defined default font for OSs
	QFont f( font() );
#if defined( Q_OS_UNIX )
	f.setFamily( "Monospace" );
#endif
	setFont( f );
	// settings some default qscintilla settings
	setMarginsFont( f );
	setMarginWidth( 0, "00000" );
	setMarginLineNumbers( 0, true );
	setEolMode( EolWindows );
	setEdgeMode( EdgeLine );
	setEdgeColor( Qt::red );
	setEdgeColumn( 140 );
	//
	connect( this, SIGNAL( copyAvailable( bool ) ), this, SLOT( setCopyAvailable( bool ) ) );
	connect( QApplication::clipboard(), SIGNAL( dataChanged() ), this, SLOT( clipboard_dataChanged() ) );
	connect( this, SIGNAL( textChanged() ), this, SLOT( textChanged() ) );
	connect( this, SIGNAL( cursorPositionChanged( int, int ) ), this, SLOT( cursorPositionChanged( int, int ) ) );
	// create action
	mActions << new Action( tr( "Matching Brace" ), this, SLOT( moveToMatchingBrace() ), Action::TextLength, tr( "Ctrl+M" ) );
	mActions << new Action( tr( "Undo" ), this, SLOT( undo() ), Action::UndoAvailable, tr( "Ctrl+Z" ) );
	mActions << new Action( tr( "Redo" ), this, SLOT( redo() ), Action::RedoAvailable, tr( "Ctrl+Y" ) );
	mActions << new Action( tr( "Separator" ) );
	mActions << new Action( tr( "Copy" ), this, SLOT( copy() ), Action::CopyAvailable, tr( "Ctrl+C" ) );
	mActions << new Action( tr( "Cut" ), this, SLOT( cut() ), Action::CopyAvailable, tr( "Ctrl+X" ) );
	mActions << new Action( tr( "Paste" ), this, SLOT( paste() ), Action::PasteAvailable, tr( "Ctrl+V" ) );
	mActions << new Action( tr( "Separator" ) );
	mActions << new Action( tr( "Select All" ), this, SLOT( selectAll() ), Action::NotCopyAvailableAndTextLength, tr( "Ctrl+A" ) );
	mActions << new Action( tr( "Select None" ), this, SLOT( selectNone() ), Action::CopyAvailable, tr( "Ctrl+Shift+A" ) );
	mActions << new Action( tr( "Separator" ) );
	mActions << new Action( tr( "Go To Line..." ), this, SLOT( goToLine() ), Action::TextLength, tr( "Ctrl+G" ) );
}
//
MonkeyEditor::~MonkeyEditor()
{
	foreach ( Action* a, mActions )
		delete a;
	mActions.clear();
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
	emit focused( false );
	QsciScintilla::focusOutEvent( e );
}
//
void MonkeyEditor::contextMenuEvent( QContextMenuEvent* e )
{
	QMenu m( this );
	foreach ( Action* a, mActions )
	{
		if ( a->Caption == tr( "Separator" ) )
			m.addSeparator();
		else
			m.addAction( QIcon( a->Pixmap ), a->Caption, a->Receiver, a->Slot, a->Shortcut )->setEnabled( getState( a->State ) );
	}
	m.exec( e->globalPos() );
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
bool MonkeyEditor::getState( Action::StateType s )
{
	switch ( s )
	{
	case Action::TextLength:
		return text().length();
		break;
	case Action::UndoAvailable:
		return isUndoAvailable();
		break;
	case Action::RedoAvailable:
		return isRedoAvailable();
		break;
	case Action::CopyAvailable:
		return mCopyAvailable;
		break;
	case Action::PasteAvailable:
		return pasteAvailable();
		break;
	case Action::NotCopyAvailableAndTextLength:
		return !mCopyAvailable && text().length();
		break;
	case Action::Enabled:
	case Action::None:
		return true;
		break;
	case Action::Disabled:
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
void MonkeyEditor::selectNone()
{
	selectAll( false );
}
//
void MonkeyEditor::searchReplace()
{
	qscintillaSearch::self()->setEditor( this );
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
		setCursorPosition( j -1, 0/*i < lineLength( j -1 ) ? i : lineLength( j -1 ) -1*/  );
}
//
bool MonkeyEditor::openFile( const QString& s, QTextCodec* c )
{
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
    mFilePath = fp;
    mCodec = c;
	QTextStream ts( &f );
	if ( c )
		ts.setCodec( c );
	setText( ts.readAll() );
	setModified( false );
	unsetCursor();
	emit fileOpened( true );
	return true;
}
//
void MonkeyEditor::saveFile()
{
	// if no filename abort
	if ( mFilePath.isEmpty() )
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
	emit fileOpened( false );
}
