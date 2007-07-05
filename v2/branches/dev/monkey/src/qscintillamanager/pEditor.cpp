#include "pEditor.h"
#include "pQScintilla.h"
#include "pSearch.h"

#include "qsciprinter.h"

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QPrintDialog>

pEditor::pEditor( QWidget* p )
	: QsciScintilla( p )
{
	// deal with utf8
	setUtf8( true );

	// connection
	connect( this, SIGNAL( linesChanged() ), this, SLOT( linesChanged() ) );
	connect( this, SIGNAL( copyAvailable( bool ) ), this, SLOT( setCopyAvailable( bool ) ) );
	connect( this, SIGNAL( cursorPositionChanged( int, int ) ), this, SLOT( cursorPositionChanged( int, int ) ) );
	connect( this, SIGNAL( textChanged() ), this, SLOT( textChanged() ) );
	connect( QApplication::clipboard(), SIGNAL( dataChanged() ), this, SLOT( clipboardDataChanged() ) );
}

pEditor::~pEditor()
{
}

bool pEditor::lineNumbersMarginEnabled() const
{
	return marginLineNumbers( 0 );
}

int pEditor::lineNumbersMarginWidth() const
{
	return property( "LineNumbersMarginWidth" ).toInt();
}

bool pEditor::lineNumbersMarginAutoWidth() const
{
	return property( "LineNumbersMarginAutoWidth" ).toBool();
}

void pEditor::setLineNumbersMarginEnabled( bool b )
{
	setMarginLineNumbers( 0, b );
}

void pEditor::setLineNumbersMarginWidth( int i )
{
	int j = i;
	if ( i != 0 )
		j++;

	setProperty( "LineNumbersMarginWidth", i );
	setMarginWidth( 0, QString().fill( '0', j ) );
}

void pEditor::setLineNumbersMarginAutoWidth( bool b )
{
	setProperty( "LineNumbersMarginAutoWidth", b );
	emit linesChanged();
}

void pEditor::linesChanged()
{
	if ( lineNumbersMarginAutoWidth() )
		setLineNumbersMarginWidth( QString::number( lines() ).length() );
}

bool pEditor::copyAvailable()
{
	return mCopyAvailable;
}

bool pEditor::canPaste()
{
	return QApplication::clipboard()->text().length();
}

QPoint pEditor::cursorPosition() const
{
	return mCursorPosition;
}

void pEditor::setCopyAvailable( bool b )
{
	mCopyAvailable = b;
}

void pEditor::cursorPositionChanged( int l, int p )
{
	mCursorPosition = QPoint( p, l );
	emit cursorPositionChanged( mCursorPosition );
}

void pEditor::textChanged()
{
	emit undoAvailable( isUndoAvailable() );
	emit redoAvailable( isRedoAvailable() );
}

void pEditor::clipboardDataChanged()
{
	emit pasteAvailable( canPaste() );
}

bool pEditor::openFile( const QString& s )
{
	if ( isModified() )
		return false;

	// open file
	QFile f( s );
	if ( !f.open( QFile::ReadOnly ) )
	{
		QMessageBox::warning( this, tr( "Open file..." ), tr( "Cannot read file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
		return false;
	}

	// set lexer
	delete lexer();
	setLexer( pQScintilla::instance()->lexerForFilename( s ) );

	// set properties
	pQScintilla::instance()->applyProperties( this );

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
	if ( !isModified() )
		return true;

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

void pEditor::print( bool b )
{
	// get printer
	QsciPrinter p;

	// set wrapmode
	p.setWrapMode( WrapWord );

	// if quick print
	if ( b )
	{
		// print and return
		p.printRange( this );
		return;
	}

	// printer dialog
	QPrintDialog d( &p );

	// if ok
	if ( d.exec() )
	{
		// print
		int f = -1, t = -1, i;
		if ( d.printRange() == QPrintDialog::Selection )
			getSelection( &f, &i, &t, &i );
		p.printRange( this, f, t );
	}
}

void pEditor::quickPrint()
{
	print( true );
}

void pEditor::selectNone()
{
	selectAll( false );
}

void pEditor::invokeSearchReplace()
{
	pSearch::instance()->setEditor( this );
	pSearch::instance()->leSearch->setFocus();
	pSearch::instance()->leSearch->selectAll();
	if ( !pSearch::instance()->isVisible() )
		pSearch::instance()->setVisible( true );
}

void pEditor::invokeGoToLine()
{
	bool b;
	int l, i;
	getCursorPosition( &l, &i );
	int j = QInputDialog::getInteger( this, tr( "Go To Line..." ), tr( "Enter the line you want to go:" ), l +1, 1, lines(), 1, &b );
	if ( b )
		setCursorPosition( j -1, 0 );
}

void pEditor::convertTabs( int i )
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
