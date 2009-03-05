#include "UIMonkeyDbg.h"
#include "../pEditor.h"

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMessageBox>

UIMonkeyDbg::UIMonkeyDbg( QWidget* parent)
	: QMainWindow( parent )
{
	setupUi( this );

	aCloseFile->setEnabled( false );
	aCloseAllFiles->setEnabled( false );
}

UIMonkeyDbg::~UIMonkeyDbg()
{
}

void UIMonkeyDbg::initConnections()
{
}

pEditor* UIMonkeyDbg::openFile( const QString& fileName )
{
	if ( mOpenedFiles.contains( fileName ) )
	{
		maWorkspace->setActiveSubWindow( mOpenedFiles[ fileName ].second );
		return qobject_cast<pEditor*>( mOpenedFiles[ fileName ].second->widget() );
	}
	
	pEditor* editor = new pEditor( maWorkspace );
	editor->setReadOnly( true );
	editor->setMarginLineNumbers( 1, true );
	editor->setMarginWidth( 1, 50 );
	editor->setMarginSensitivity( 1, true );
	editor->setFolding( QsciScintilla::BoxedTreeFoldStyle );
	editor->setIndentationWidth( 4 );
	editor->setCaretLineVisible( true );
	
	if ( !editor->openFile( fileName, "UTF-8" ) )
	{
		delete editor;
		return 0;
	}
	
	QListWidgetItem* item = new QListWidgetItem( lwFiles );
	item->setText( QFileInfo( fileName ).fileName() );
	item->setData( Qt::UserRole, fileName );
	
	QMdiSubWindow* subWindow = maWorkspace->addSubWindow( editor );
	subWindow->setWindowFilePath( fileName );
	
	mOpenedFiles[ fileName ].first = item;
	mOpenedFiles[ fileName ].second = subWindow;
	
	connect( editor, SIGNAL( breakpointToggled( int ) ), this, SLOT( editorBreakpointToggled( int ) ) );
	connect( subWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( subWindow_destroyed( QObject* ) ) );
	
	subWindow->showMaximized();
	
	return editor;
}

void UIMonkeyDbg::closeCurrentFile()
{
	maWorkspace->closeActiveSubWindow();
}

void UIMonkeyDbg::closeAllFiles()
{
	maWorkspace->closeAllSubWindows();
}

void UIMonkeyDbg::subWindow_destroyed( QObject* object )
{
	QWidget* widget = qobject_cast<QWidget*>( object );
	const QString file = widget->windowFilePath();
	
	delete mOpenedFiles[ file ].first;
	mOpenedFiles.remove( file );
}

void UIMonkeyDbg::on_lwFiles_itemActivated( QListWidgetItem* item )
{
	QMdiSubWindow* subWindow = mOpenedFiles.value( item->data( Qt::UserRole ).toString() ).second;
	
	if ( subWindow && maWorkspace->activeSubWindow() != subWindow )
	{
		maWorkspace->setActiveSubWindow( subWindow );
	}
}

void UIMonkeyDbg::on_maWorkspace_subWindowActivated( QMdiSubWindow* subWindow )
{
	aCloseFile->setEnabled( subWindow );
	aCloseAllFiles->setEnabled( subWindow );
	
	if ( !subWindow )
	{
		return;
	}
	
	QListWidgetItem* item = mOpenedFiles.value( subWindow->windowFilePath() ).first;
	
	if ( item && lwFiles->currentItem() != item )
	{
		lwFiles->setCurrentItem( item );
	}
}

void UIMonkeyDbg::on_aOpenFile_triggered()
{
}

void UIMonkeyDbg::on_aLoadTarget_triggered()
{
}
