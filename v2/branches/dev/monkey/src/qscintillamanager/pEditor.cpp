#include "pEditor.h"
#include "pSearch.h"

#include "qscilexer.h"
#include "qsciprinter.h"

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

pEditor::pEditor( QWidget* p )
	: QsciScintilla( p )
{
	// deal with utf8
	setUtf8( true );

	// connection
	connect( this, SIGNAL( cursorPositionChanged( int, int ) ), this, SLOT( cursorPositionChanged( int, int ) ) );
	connect( this, SIGNAL( textChanged() ), this, SLOT( textChanged() ) );
	connect( QApplication::clipboard(), SIGNAL( dataChanged() ), this, SLOT( clipboardDataChanged() ) );
}

pEditor::~pEditor()
{
}

bool pEditor::canPaste()
{
	return QApplication::clipboard()->text().length();
}

void pEditor::cursorPositionChanged( int l, int p )
{
	emit cursorPositionChanged( QPoint( p, l ) );
}

void pEditor::textChanged()
{
	emit undoAvailable( isUndoAvailable() );
	emit redoAvailable( isRedoAvailable() );
}

void pEditor::clipboardDataChanged()
{
	emit pasteAvailable( QApplication::clipboard()->text().length() );
}

bool pEditor::openFile( const QString& s )
{
	// open file
	QFile f( s );
	if ( !f.open( QFile::ReadOnly ) )
	{
		QMessageBox::warning( this, tr( "Open file..." ), tr( "Cannot read file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
		return false;
	}

	// load file
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QTextStream i( &f );
	setText( i.readAll() );
	setModified( false );
	QApplication::restoreOverrideCursor();

	return true;
}

bool pEditor::saveFile( const QString& s )
{
	QFile f( s );
	if ( !f.open( QFile::WriteOnly ) )
	{
		QMessageBox::warning( this, tr( "Save file..." ), tr( "Cannot write file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
		return false;
	}

	// writing file
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QTextStream o( &f );
	o << text();
	setModified( false );
	QApplication::restoreOverrideCursor();

	return true;
}

void pEditor::closeFile()
{
	clear();
	setModified( false );
}


