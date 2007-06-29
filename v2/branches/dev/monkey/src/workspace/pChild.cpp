/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pChild.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "pChild.h"
#include "pEditor.h"

#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>

pChild::pChild()
	: pAbstractChild()
{
	// create textedit
	mEditor = new pEditor;

	// create layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );

	// add textedit
	vl->addWidget( mEditor );

	// connections
	connect( mEditor, SIGNAL( cursorPositionChanged( const QPoint& ) ), this, SIGNAL( cursorPositionChanged( const QPoint& ) ) );
	connect( mEditor, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( pasteAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
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
	return mEditor->isModified();
}

bool pChild::isUndoAvailable() const
{
	return mEditor->isUndoAvailable();
}

void pChild::undo()
{
	mEditor->undo();
}

bool pChild::isRedoAvailable() const
{
	return mEditor->isRedoAvailable();
}

void pChild::redo()
{
	mEditor->redo();
}

void pChild::cut()
{
	mEditor->cut();
}

void pChild::copy()
{
	mEditor->copy();
}

void pChild::paste()
{
	mEditor->paste();
}

void pChild::searchReplace()
{}

void pChild::goTo()
{}

void pChild::goTo( const QString&, const QPoint&, bool )
{}

bool pChild::isCopyAvailable() const
{
	return mEditor->selectedText().length();
}

bool pChild::isPasteAvailable() const
{
	return mEditor->canPaste();
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

	mEditor->saveFile( s );
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

	// change window title
	setWindowTitle( s );

	emit fileOpened( s, mProxy );
}

void pChild::closeFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	// reset editor
	mEditor->clear();
	//mEditor->document()->setModified( false );

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
