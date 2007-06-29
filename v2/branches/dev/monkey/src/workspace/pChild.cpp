/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pChild.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "pChild.h"

#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>

pChild::pChild()
	: pAbstractChild()
{
	// create textedit
	mEdit = new QTextEdit;

	// create layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );

	// add textedit
	vl->addWidget( mEdit );

	// connections
	connect( mEdit, SIGNAL( cursorPositionChanged() ), this, SLOT( cursorPositionChanged() ) );
	connect( mEdit->document(), SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
	connect( mEdit, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mEdit, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mEdit, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
}

pChild::~pChild()
{
	closeFiles();
}

void pChild::cursorPositionChanged()
{
	emit pAbstractChild::cursorPositionChanged( cursorPosition() );
}

QPoint pChild::cursorPosition() const
{
	return QPoint();
}

void pChild::showFile( const QString& )
{}

QString pChild::currentFile() const
{
	return mFiles.value( 0 );
}

QString pChild::currentFileName() const
{
	return QFileInfo( currentFile() ).fileName();
}

bool pChild::isModified() const
{
	return mEdit->document()->isModified();
}

bool pChild::isUndoAvailable() const
{
	return mEdit->document()->isUndoAvailable();
}

void pChild::undo()
{
	mEdit->undo();
}

bool pChild::isRedoAvailable() const
{
	return mEdit->document()->isRedoAvailable();
}

void pChild::redo()
{
	mEdit->redo();
}

void pChild::cut()
{
	mEdit->cut();
}

void pChild::copy()
{
	mEdit->copy();
}

void pChild::paste()
{
	mEdit->paste();
}

void pChild::searchReplace()
{}

void pChild::goTo()
{}

void pChild::goTo( const QString&, const QPoint&, bool )
{}

bool pChild::isCopyAvailable() const
{
	return mEdit->textCursor().selectedText().length();
}

bool pChild::isPasteAvailable() const
{
	return mEdit->canPaste();
}

bool pChild::isSearchReplaceAvailable() const
{
	return false;
}

bool pChild::isGoToAvailable() const
{
	return false;
}

bool pChild::isModified( const QString& ) const
{
	return isModified();
}

bool pChild::isPrintAvailable() const
{
	return false;
}

void pChild::saveFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	QFile f( s );
	if ( !f.open( QFile::WriteOnly | QFile::Text ) )
	{
		QMessageBox::warning( this, tr( "Application" ), tr( "Cannot write file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
		return;
	}

	// writing file
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QTextStream o( &f );
	o << mEdit->toPlainText();
	mEdit->document()->setModified( false );
	QApplication::restoreOverrideCursor();
}

void pChild::saveFiles()
{ saveCurrentFile(); }

void pChild::openFile( const QString& s, const QPoint&, QTextCodec* c )
{
	// if already open file, cancel
	if ( !currentFile().isNull() )
		return;

	// add filename to list
	mFiles.append( s );

	// open file
	QFile f( s );
	if ( !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		QMessageBox::warning( this, tr( "Application" ), tr( "Cannot read file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
		return;
	}

	// change window title
	setWindowTitle( s );

	// get textcodec
	/*
	if ( !c && mProxy )
		c = mProxy
	*/

	// load file
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QTextStream i( &f );
	mEdit->setPlainText( i.readAll() );
	mEdit->document()->setModified( false );
	QApplication::restoreOverrideCursor();

	emit fileOpened( s, mProxy );
}

void pChild::closeFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	// reset editor
	mEdit->clear();
	mEdit->document()->setModified( false );

	// change window title
	setWindowTitle( tr( "[No Name]" ) );

	// remove from files list
	mFiles.removeAll( s );

	emit fileClosed( s, mProxy );
}

void pChild::closeFiles()
{ closeCurrentFile(); }

void pChild::printFile( const QString& )
{}

void pChild::quickPrintFile( const QString& )
{}
