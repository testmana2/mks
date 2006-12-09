#include "MonkeyEditor.h"
//
#include <QtGui>
//
MonkeyEditor::MonkeyEditor( QWidget* p )
	: QsciScintilla( p ), mCopyAvailable( false )
{
	connect( this, SIGNAL( copyAvailable( bool ) ), this, SLOT( setCopyAvailable( bool ) ) );
	// create action
	mActions << Action( tr( "Matching Brace" ), this, SLOT( moveToMatchingBrace() ), Action::TextLength, tr( "Ctrl+M" ) );
	mActions << Action( tr( "Undo" ), this, SLOT( undo() ), Action::UndoAvailable, tr( "Ctrl+Z" ) );
	mActions << Action( tr( "Redo" ), this, SLOT( redo() ), Action::RedoAvailable, tr( "Ctrl+Y" ) );
	mActions << Action( tr( "Separator" ) );
	mActions << Action( tr( "Copy" ), this, SLOT( copy() ), Action::CopyAvailable, tr( "Ctrl+C" ) );
	mActions << Action( tr( "Cut" ), this, SLOT( cut() ), Action::CopyAvailable, tr( "Ctrl+X" ) );
	mActions << Action( tr( "Paste" ), this, SLOT( paste() ), Action::PasteAvailable, tr( "Ctrl+V" ) );
	mActions << Action( tr( "Separator" ) );
	mActions << Action( tr( "Select All" ), this, SLOT( selectAll() ), Action::NotCopyAvailableAndTextLength, tr( "Ctrl+A" ) );
	mActions << Action( tr( "Select None" ), this, SLOT( selectNone() ), Action::CopyAvailable, tr( "Ctrl+Shift+A" ) );
	mActions << Action( tr( "Separator" ) );
	mActions << Action( tr( "Go To Line..." ), this, SLOT( goToLine() ), Action::TextLength, tr( "Ctrl+G" ) );
}
//
void MonkeyEditor::contextMenuEvent( QContextMenuEvent* e )
{
	QMenu m( this );
	foreach ( Action a, mActions )
	{
		if ( a.Caption == tr( "Separator" ) )
			m.addSeparator();
		else
			m.addAction( QIcon( a.Pixmap ), a.Caption, a.Receiver, a.Slot, a.Shortcut )->setEnabled( getState( a.State ) );
	}
	m.exec( e->globalPos() );
}
//
bool MonkeyEditor::pasteAvailable() const
{
	return QApplication::clipboard()->text( QClipboard::Clipboard ).length();
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
void MonkeyEditor::goToLine()
{
	bool b;
	int l, i;
	getCursorPosition( &l, &i );
	int j = QInputDialog::getInteger( this, tr( "Go To Line..." ), tr( "Enter the line you want to go:" ), l +1, 1, lines(), 1, &b );
	if ( b )
		setCursorPosition( j -1, 0/*i < lineLength( j -1 ) ? i : lineLength( j -1 ) -1*/  );
}
