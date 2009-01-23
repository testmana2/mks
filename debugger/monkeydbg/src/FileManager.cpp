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

void FileManager::gotoFileLine( const QString& fileName, int line )
{
	QsciScintilla* editor = 0;
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		if ( window->windowFilePath() == fileName )
		{
			editor = qobject_cast<pEditor*>( window->widget() );
			break;
		}
	}
	
	if ( !editor )
	{
		editor = openFile( fileName );
	}
	
	if ( editor )
	{
		editor->setCursorPosition( line -1, 0 );
	}
}

void FileManager::marginClicked( int margin, int line, Qt::KeyboardModifiers state )
{
	Q_UNUSED( state );
	
	if ( margin == 1 )
	{
		QMdiSubWindow* window = mMdiArea->activeSubWindow();
		QsciScintilla* editor = qobject_cast<QsciScintilla*>( window->widget() );
		
		editor->markerAdd( line, pEditor::mdEnabledBreak );
		
		emit breakpointToggled( window->windowFilePath(), line +1 );
	}
}

QsciScintilla* FileManager::openFile( const QString& fileName )
{
	if (fileName.isEmpty() )
	{
		return 0;
	}
	
	pEditor* sci = new pEditor( mMdiArea );
	QApplication::setOverrideCursor( Qt::WaitCursor );
	
	// open file
	QFile f( fileName );
	if ( !f.open( QFile::ReadOnly ) )
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::critical( mMdiArea, tr( "Open file" ), tr( "Cannot read file %1:\n%2." ).arg( fileName ).arg( f.errorString() ) );
		return 0;
	}

	// configure
	sci->setMarginLineNumbers( 1, true );
	sci->setMarginWidth( 1, 50 );
	sci->setMarginSensitivity( 1, true );
	sci->setFolding( QsciScintilla::BoxedFoldStyle );
	sci->setIndentationWidth( 4 );
	sci->setCaretLineVisible( true );
	
	connect( sci, SIGNAL( marginClicked( int, int, Qt::KeyboardModifiers ) ), this, SLOT( marginClicked( int, int, Qt::KeyboardModifiers ) ) );

	// load file
	QString datas = QString::fromUtf8( f.readAll() );
	sci->setText( datas );
	
	mMdiArea->addSubWindow( sci )->setWindowFilePath( fileName );
	sci->showMaximized();

	QApplication::restoreOverrideCursor();
	
	return sci;
}
