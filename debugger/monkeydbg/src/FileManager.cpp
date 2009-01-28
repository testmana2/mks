#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMdiSubWindow>
#include <QVariant>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>

#include <QDebug>
#include "FileManager.h"

#include "pEditor.h"

FileManager::FileManager( QObject* parent, QMdiArea* mdiArea )
	: QObject( parent ), mMdiArea( mdiArea )
{
}

FileManager::~FileManager()
{	
}

void FileManager::openFileTriggered ()
{
	QString fileName = QFileDialog::getOpenFileName (NULL, tr("Open source"), QDir::current().path());
	if (! fileName.isEmpty())
		openFile (fileName);
}

void FileManager::closeFileTriggered ()
{
	mMdiArea->closeActiveSubWindow ();
}

void FileManager::clearBreakpoints( const QString& file, int line )
{
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		if ( window->windowFilePath() == file || file.isEmpty() )
		{
			pEditor* editor = qobject_cast<pEditor*>( window->widget() );
			editor->clearBreakpoints( line );
		}
	}
}

void FileManager::setBreakpoint( const QString& file, int line, int type )
{
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		if ( window->windowFilePath() == file || file.isEmpty() )
		{
			pEditor* editor = qobject_cast<pEditor*>( window->widget() );
			editor->setBreakpoint( line, type );
		}
	}
}

void FileManager::clearDebuggerPosition()
{
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		pEditor* editor = qobject_cast<pEditor*>( window->widget() );
		editor->clearDebuggerPosition();
	}
}

void FileManager::setDebuggerPosition( const QString& fileName, int line )
{
	pEditor* editor = 0;
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		pEditor* tmpEditor = qobject_cast<pEditor*>( window->widget() );
		tmpEditor->clearDebuggerPosition();
		
		if ( window->windowFilePath() == fileName )
		{
			editor = tmpEditor;
		}
	}
	
	if ( !editor )
	{
		editor = openFile( fileName );
	}
	
	if ( editor )
	{
		QMdiSubWindow* window = qobject_cast<QMdiSubWindow*>( editor->parentWidget() );
		mMdiArea->setActiveSubWindow( window );
		editor->setDebuggerPosition( line -1 );
	}
}

void FileManager::breakpointAdded( const QGdbDriver::Breakpoint& breakpoint )
{
	setBreakpoint( breakpoint.absolute_file, breakpoint.line -1, 0 );
}

void FileManager::breakpointRemoved( const QGdbDriver::Breakpoint& breakpoint )
{
	clearBreakpoints( breakpoint.absolute_file, breakpoint.line -1 );
}

void FileManager::frameSelected( const QGdbDriver::Frame& frame )
{
	if ( frame.from.isEmpty() )
	{
		setDebuggerPosition( frame.file, frame.line );
	}
}

void FileManager::marginClicked( int margin, int line, Qt::KeyboardModifiers state )
{
	Q_UNUSED( state );
	
	if ( margin == 1 )
	{
		QMdiSubWindow* window = mMdiArea->activeSubWindow();
		emit breakpointToggled( window->windowFilePath(), line +1 );
	}
}

pEditor* FileManager::openFile( const QString& fileName )
{
	
	pEditor* sci = new pEditor( mMdiArea );
	
	if ( !sci->openFile( fileName, "UTF-8" ) )
	{
		QMessageBox::critical( mMdiArea, tr( "Open file" ), tr( "Cannot load file %1" ).arg( fileName ) );
		delete sci;
		return 0;
	}

	// configure
	sci->setReadOnly( true );
	sci->setMarginLineNumbers( 1, true );
	sci->setMarginWidth( 1, 50 );
	sci->setMarginSensitivity( 1, true );
	sci->setFolding( QsciScintilla::BoxedFoldStyle );
	sci->setIndentationWidth( 4 );
	sci->setCaretLineVisible( true );
	
	connect( sci, SIGNAL( marginClicked( int, int, Qt::KeyboardModifiers ) ), this, SLOT( marginClicked( int, int, Qt::KeyboardModifiers ) ) );
	
	mMdiArea->addSubWindow( sci )->setWindowFilePath( fileName );
	sci->showMaximized();
	
	return sci;
}
