#include "TextChild.h"
//
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
//
TextChild::TextChild()
{
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );
	mEditor = new MonkeyEditor;
	vl->addWidget( mEditor );
	//
	connect( mEditor, SIGNAL( cursorPositionChanged( const QPoint& ) ), this, SIGNAL( cursorPositionChanged( const QPoint& ) ) );
	connect( mEditor, SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
	connect( mEditor, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( pasteAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
}
// return cursor position if available
QPoint TextChild::cursorPosition() const
{
	int l = 0, i = 0;
	mEditor->getCursorPosition( &l, &i );
	return QPoint( i +1, l +1 );
}
// show the current file in child
void TextChild::showFile( const QString& )
{}
// the current visible / focused file
QString TextChild::currentFile() const
{
	return mFiles.at( 0 );
}
// return the current file modified flag
bool TextChild::isModified() const
{
	return mEditor->isModified();
}
// return the current file undo flag
bool TextChild::isUndoAvailable() const
{
	return mEditor->isUndoAvailable();
}
// return the current file redo flag
bool TextChild::isRedoAvailable() const
{
	return mEditor->isRedoAvailable();
}
// return the current file paste available
bool TextChild::isPasteAvailable() const
{
	return mEditor->pasteAvailable();
}
// return the current file copy available
bool TextChild::isCopyAvailable() const
{
	return mEditor->copyAvailable();
}
// return the modified state of file
bool TextChild::isModified( const QString& ) const
{
	return mEditor->isModified();
}
// ask to save file
void TextChild::saveFile( const QString& s )
{
	QFile f( s );
	if ( !f.open( QFile::WriteOnly | QFile::Text ) )
	{
		QMessageBox::warning( this, tr( "Application" ),
			tr( "Cannot write file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
        return;
    }
	QTextStream out( &f );
	QApplication::setOverrideCursor( Qt::WaitCursor );
	out << mEditor->text();
	QApplication::restoreOverrideCursor();
    mEditor->setModified( false );
}
//
void TextChild::saveFiles()
{
	saveCurrentFile();
}
// ask to save file
void TextChild::loadFile( const QString& s )
{
	setWindowTitle( s );
	QFile f( s );
	if ( !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		QMessageBox::warning( this, tr( "Application" ),
			tr( "Cannot read file %1:\n%2." ).arg( s ).arg( f.errorString() ) );
        return;
    }
    QTextStream in( &f );
    QApplication::setOverrideCursor( Qt::WaitCursor );
    mEditor->setText( in.readAll() );
    QApplication::restoreOverrideCursor();
	mEditor->setModified( false );
}
//
void TextChild::printFile( const QString& )
{
	mEditor->print();
}
// ask to quick print this file
void TextChild::quickPrintFile( const QString& )
{
	mEditor->print( true );
}
// undo
void TextChild::undo()
{
	mEditor->undo();
}
// redo
void TextChild::redo()
{
	mEditor->redo();
}
// cut
void TextChild::cut()
{
	mEditor->cut();
}
// copy
void TextChild::copy()
{
	mEditor->copy();
}
// paste
void TextChild::paste()
{
	mEditor->paste();
}
// search/replace
void TextChild::searchReplace()
{
	mEditor->searchReplace();
}
// go to
void TextChild::goTo()
{
	mEditor->goToLine();
}
// search/replace available
bool TextChild::isSearchReplaceAvailable() const
{
	return true;
}
// goto available
bool TextChild::isGoToAvailable() const
{
	return true;
}
// print available
bool TextChild::isPrintAvailable() const
{
	return true;
}
