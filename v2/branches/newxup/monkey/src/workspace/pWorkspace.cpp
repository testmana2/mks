/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pWorkspace.cpp
** Date      : 2008-01-14T00:37:21
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#include <QToolButton>
#include <QCloseEvent>
#include <QMainWindow>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QDir>
#include <QMdiSubWindow>
#include <QVBoxLayout>
#include <QMessageBox>

#include <QDebug>

#include "pWorkspace.h"
#include "pOpenedFileExplorer.h"
#include "pAbstractChild.h"
#include "../recentsmanager/pRecentsManager.h" // FIXME remove
#include "pFileManager.h"
#include "../maininterface/ui/UISettings.h"
#include "UISaveFiles.h"
#include "../maininterface/ui/UIAbout.h"
#include "../templatesmanager/ui/UITemplatesWizard.h"
#include "../abbreviationsmanager/pAbbreviationsManager.h"
#include "../pMonkeyStudio.h"
#include "../templatesmanager/pTemplatesManager.h"
#include "../xupmanager/gui/XUPProjectManager.h"
#include "../xupmanager/core/XUPProjectItem.h"
#include "../coremanager/MonkeyCore.h"
#include "../maininterface/UIMain.h"
#include "../statusbar/StatusBar.h"
#include "../pluginsmanager/PluginsManager.h"
#include "MkSFileDialog.h" // FIXME remove

#include "../qscintillamanager/pEditor.h"

#include <pQueuedMessageToolBar.h>
#include <pMenuBar.h>
#include <pMultiToolBar.h>
#include <TranslationDialog.h>
#include <TranslationManager.h>

int pWorkspace::CONTENT_CHANGED_TIME_OUT = 3000;
QString pWorkspace::DEFAULT_CONTEXT = QLatin1String( "Default" );

pWorkspace::pWorkspace( QMainWindow* parent )
	: QFrame( parent )
{
	Q_ASSERT( parent );
	mViewMode = pWorkspace::NoTabs;
	
	pMenuBar* mb = MonkeyCore::menuBar();
	
	// action group for view modes
	mViewModesGroup = new QActionGroup( this );
	mViewModesGroup->addAction( mb->action( "mWindow/aNoTabs" ) );
	mViewModesGroup->addAction( mb->action( "mWindow/aTopTabs" ) );
	mViewModesGroup->addAction( mb->action( "mWindow/aBottomTabs" ) );
	mViewModesGroup->addAction( mb->action( "mWindow/aLeftTabs" ) );
	mViewModesGroup->addAction( mb->action( "mWindow/aRightTabs" ) );
	mb->action( "mWindow/aSeparator1" );
	
	mb->menu( "mWindow" )->insertActions( mb->action( "mWindow/aCascase" ), mViewModesGroup->actions() );
	mb->menu( "mWindow" )->insertAction( mb->action( "mWindow/aCascase" ), mb->action( "mWindow/aSeparator1" ) );
	
	QList<QAction*> actions = mViewModesGroup->actions();
	
	for ( int i = pWorkspace::NoTabs; i < pWorkspace::RightTabs +1; i++ )
	{
		QAction* action = actions.at( i );
		action->setCheckable( true );
		action->setData( i );
		
		if ( mViewMode == i )
		{
			action->setChecked( true );
		}
		
		switch ( (pWorkspace::ViewMode)i )
		{
			case pWorkspace::NoTabs:
				action->setText( tr( "No Tabs" ) );
				action->setToolTip( tr( "No tabs, use 'Oopened Files List' to have a list of opened documents" ) );
				break;
			case pWorkspace::TopTabs:
				action->setText( tr( "Tabs at &Top" ) );
				action->setToolTip( action->text() );
				break;
			case pWorkspace::BottomTabs:
				action->setText( tr( "Tabs at &Bottom" ) );
				action->setToolTip( action->text() );
				break;
			case pWorkspace::LeftTabs:
				action->setText( tr( "Tabs at &Left" ) );
				action->setToolTip( action->text() );
				break;
			case pWorkspace::RightTabs:
				action->setText( tr( "Tabs at &Right" ) );
				action->setToolTip( action->text() );
				break;
		}
	}
	
	mOpenedFileExplorer = new pOpenedFileExplorer( this );
	
	// layout
	mLayout = new QVBoxLayout( this );
	mLayout->setMargin( 0 );
	mLayout->setSpacing( 0 );
	
	// multitoolbar
	QFrame* hline = new QFrame( this );
	hline->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	
	// document area
	mMdiArea = new QMdiArea( this );
	mMdiArea->setActivationOrder( QMdiArea::CreationOrder );
	mMdiArea->setDocumentMode( true );
	
	// add widgets to layout
	mLayout->addWidget( MonkeyCore::multiToolBar() );
	mLayout->addWidget( hline );
	mLayout->addWidget( mMdiArea );
	
	// creaet file watcher
	mFileWatcher = new QFileSystemWatcher( this );
	mContentChangedTimer = new QTimer( this );
	
	// load settings
	loadSettings();

	// connections
	connect( mViewModesGroup, SIGNAL( triggered( QAction* ) ), this, SLOT( viewModes_triggered( QAction* ) ) );
	connect( mMdiArea, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( mdiArea_subWindowActivated( QMdiSubWindow* ) ) );
	connect( parent, SIGNAL( urlsDropped( const QList<QUrl>& ) ), this, SLOT( internal_urlsDropped( const QList<QUrl>& ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ), this, SLOT( internal_currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ) );
	connect( mContentChangedTimer, SIGNAL( timeout() ), this, SLOT( contentChangedTimer_timeout() ) );
	connect( MonkeyCore::multiToolBar(), SIGNAL( notifyChanges() ), this, SLOT( multitoolbar_notifyChanges() ) );
}

bool pWorkspace::eventFilter( QObject* object, QEvent* event )
{
	// get document
	if ( object->isWidgetType() )
	{
		pAbstractChild* document = qobject_cast<pAbstractChild*>( object );
		
		if ( document && event->type() == QEvent::Close )
		{
			event->ignore();
			closeDocument( document );
			return true;
		}
	}
	
	return QFrame::eventFilter( object, event );
}

void pWorkspace::updateGuiState( pAbstractChild* document )
{
	// fix fucking flickering due to window activation change on application gain / lost focus.
	if ( !document && currentDocument() )
	{
		return;
	}
	
	// get child
	pEditor* editor = document ? document->editor() : 0;
	bool hasDocument = document;
	bool hasEditor = editor;
	bool modified = hasDocument ? document->isModified() : false;
	bool print = hasDocument ? document->isPrintAvailable() : false;
	bool undo = hasDocument ? document->isUndoAvailable() : false;
	bool redo = hasDocument ? document->isRedoAvailable() : false;
	bool copy = hasDocument ? document->isCopyAvailable() : false;
	bool paste = hasDocument ? document->isPasteAvailable() : false;
	bool go = hasDocument ? document->isGoToAvailable() : false;
	bool moreThanOneDocument = mMdiArea->subWindowList().count() > 1;
	
	// context toolbar
	pMultiToolBar* mtb = MonkeyCore::multiToolBar();
	
	if ( document )
	{
		if ( !mtb->contexts().contains( document->context() ) )
		{
			QToolBar* tb = mtb->toolBar( document->context() );
			
			initMultiToolBar( tb );
			document->initializeContext( tb );
		}
		
		mtb->setCurrentContext( document->context() );
	}
	else
	{
		if ( !mtb->contexts().contains( DEFAULT_CONTEXT ) )
		{
			QToolBar* tb = mtb->toolBar( DEFAULT_CONTEXT );
			
			initMultiToolBar( tb );
		}
		
		mtb->setCurrentContext( DEFAULT_CONTEXT );
	}
	
	multitoolbar_notifyChanges();

	// update file menu
	MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" )->setEnabled( modified );
	MonkeyCore::menuBar()->action( "mFile/mSave/aAll" )->setEnabled( hasDocument );
	MonkeyCore::menuBar()->action( "mFile/mClose/aCurrent" )->setEnabled( hasDocument );
	MonkeyCore::menuBar()->action( "mFile/mClose/aAll" )->setEnabled( hasDocument );
	MonkeyCore::menuBar()->action( "mFile/aReload" )->setEnabled( hasDocument );
	MonkeyCore::menuBar()->action( "mFile/aSaveAsBackup" )->setEnabled( hasDocument );
	MonkeyCore::menuBar()->action( "mFile/aQuickPrint" )->setEnabled( print );
	MonkeyCore::menuBar()->action( "mFile/aPrint" )->setEnabled( print );

	// update edit menu
	MonkeyCore::menuBar()->action( "mEdit/aUndo" )->setEnabled( undo );
	MonkeyCore::menuBar()->action( "mEdit/aRedo" )->setEnabled( redo );
	MonkeyCore::menuBar()->action( "mEdit/aCut" )->setEnabled( copy );
	MonkeyCore::menuBar()->action( "mEdit/aCopy" )->setEnabled( copy );
	MonkeyCore::menuBar()->action( "mEdit/aPaste" )->setEnabled( paste );
	
	MonkeyCore::menuBar()->action( "mEdit/aGoTo" )->setEnabled( go );
	MonkeyCore::menuBar()->action( "mEdit/aExpandAbbreviation" )->setEnabled( hasDocument );
	MonkeyCore::menuBar()->setMenuEnabled( MonkeyCore::menuBar()->menu( "mEdit/mAllCommands" ), hasEditor );
	MonkeyCore::menuBar()->setMenuEnabled( MonkeyCore::menuBar()->menu( "mEdit/mBookmarks" ), hasEditor );

	// update view menu
	MonkeyCore::menuBar()->action( "mView/aNext" )->setEnabled( moreThanOneDocument );
	MonkeyCore::menuBar()->action( "mView/aPrevious" )->setEnabled( moreThanOneDocument );

	// update status bar
	MonkeyCore::statusBar()->setModified( modified );
	MonkeyCore::statusBar()->setEOLMode( editor ? editor->eolMode() : (QsciScintilla::EolMode)-1 );
	MonkeyCore::statusBar()->setIndentMode( editor ? ( editor->indentationsUseTabs() ? 1 : 0 ) : -1 );
	MonkeyCore::statusBar()->setCursorPosition( document ? document->cursorPosition() : QPoint( -1, -1 ) );
	
	// internal update
	QDir::setCurrent( hasDocument ? document->path() : pMonkeyStudio::defaultProjectsDirectory() );
}

QString pWorkspace::defaultContext()
{
	return DEFAULT_CONTEXT;
}

void pWorkspace::loadSettings()
{
	// restore tabs settings
	/*
	tabBar()->setTabsHaveCloseButton( tabsHaveCloseButton() );
	tabBar()->setTabsHaveShortcut( tabsHaveShortcut() );
	tabBar()->setTabsElided( tabsElided() );
	tabBar()->setTabsColor( tabsTextColor() );
	tabBar()->setCurrentTabColor( currentTabTextColor() );
	*/
	
	mOpenedFileExplorer->setSortMode( pMonkeyStudio::openedFileSortingMode() );
	setDocumentMode( pMonkeyStudio::documentMode() );
	
	pMultiToolBar* mtb = MonkeyCore::multiToolBar();
	
	foreach ( const QString& context, mtb->contexts() )
	{
		QToolBar* tb = mtb->toolBar( context );
		
		initMultiToolBar( tb );
	}
	
	multitoolbar_notifyChanges();
}

void pWorkspace::initMultiToolBar( QToolBar* tb )
{
	if ( pMonkeyStudio::showQuickFileAccess() )
	{
		tb->insertAction( tb->actions().value( 0 ), MonkeyCore::workspace()->dockWidget()->comboBoxAction() );
	}
	else
	{
		tb->removeAction( MonkeyCore::workspace()->dockWidget()->comboBoxAction() );
	}
}

pOpenedFileExplorer* pWorkspace::dockWidget() const
{
	return mOpenedFileExplorer;
}

QFileSystemWatcher* pWorkspace::fileWatcher() const
{
	return mFileWatcher;
}

pAbstractChild* pWorkspace::document( int index ) const
{
	QMdiSubWindow* window = mMdiArea->subWindowList().value( index );
	return qobject_cast<pAbstractChild*>( window );
}

int pWorkspace::indexOfDocument( pAbstractChild* document ) const
{
	return mMdiArea->subWindowList().indexOf( document );
}

QList<pAbstractChild*> pWorkspace::documents() const
{
	QList<pAbstractChild*> documents;
	
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		documents << qobject_cast<pAbstractChild*>( window );
	}
	
	return documents;
}

void pWorkspace::setCurrentDocument( pAbstractChild* document )
{
	pAbstractChild* curDocument = currentDocument();
	
	if ( curDocument != document )
	{
		mMdiArea->setActiveSubWindow( document );
	}
}

pAbstractChild* pWorkspace::currentDocument() const
{
	QMdiSubWindow* window = mMdiArea->currentSubWindow();
	return qobject_cast<pAbstractChild*>( window );
}

void pWorkspace::goToLine( const QString& fileName, const QPoint& pos, const QString& codec, int selectionLength )
{
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		pAbstractChild* document = qobject_cast<pAbstractChild*>( window );
		
		if ( pMonkeyStudio::isSameFile( document->filePath(), fileName ) )
		{
			setCurrentDocument( document );
			document->goTo( pos, selectionLength );
			return;
		}
	}

	pAbstractChild* document = MonkeyCore::fileManager()->openFile( fileName, codec );

	if ( document )
	{
		document->goTo( pos, selectionLength );
	}
}

void pWorkspace::closeDocument( pAbstractChild* document, bool showDialog )
{
	if ( showDialog && UISaveFiles::saveDocument( window(), document, false ) == UISaveFiles::bCancelClose )
	{
		return;
	}
	
	// stop watching files
	const QString file = document->filePath();
	
	if ( QFileInfo( file ).isFile() && mFileWatcher->files().contains( file ) )
	{
		mFileWatcher->removePath( file );
	}
	
	// close document
	emit documentAboutToClose( document );
	document->closeFile();
	
	if ( document->testAttribute( Qt::WA_DeleteOnClose ) )
	{
		document->deleteLater();
	}
	else
	{
		unhandleDocument( document );
	}
}

pWorkspace::ViewMode pWorkspace::documentMode() const
{
	return mViewMode;
}

void pWorkspace::handleDocument( pAbstractChild* document )
{
	// init document connections
	connect( document, SIGNAL( fileOpened() ), this, SLOT( document_fileOpened() ) );
	connect( document, SIGNAL( contentChanged() ), this, SLOT( document_contentChanged() ) );
	connect( document, SIGNAL( modifiedChanged( bool ) ), this, SLOT( document_modifiedChanged( bool ) ) );
	connect( document, SIGNAL( fileClosed() ), this, SLOT( document_fileClosed() ) );
	connect( document, SIGNAL( fileReloaded() ), this, SLOT( document_fileReloaded() ) );
	// update file menu
	connect( document, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
	// update edit menu
	connect( document, SIGNAL( undoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
	connect( document, SIGNAL( redoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
	connect( document, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
	connect( document, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
	connect( document, SIGNAL( pasteAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
	// update status bar
	connect( document, SIGNAL( cursorPositionChanged( const QPoint& ) ), MonkeyCore::statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
	connect( document, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::statusBar(), SLOT( setModified( bool ) ) );
	
	// add to workspace
	document->installEventFilter( this );
	mMdiArea->blockSignals( true );
	mMdiArea->addSubWindow( document );
	mMdiArea->blockSignals( false );
}

void pWorkspace::unhandleDocument( pAbstractChild* document )
{
	const bool maximized = document->isMaximized();
	
	// init document connections
	disconnect( document, SIGNAL( fileOpened() ), this, SLOT( document_fileOpened() ) );
	disconnect( document, SIGNAL( contentChanged() ), this, SLOT( document_contentChanged() ) );
	disconnect( document, SIGNAL( modifiedChanged( bool ) ), this, SLOT( document_modifiedChanged( bool ) ) );
	disconnect( document, SIGNAL( fileClosed() ), this, SLOT( document_fileClosed() ) );
	disconnect( document, SIGNAL( fileReloaded() ), this, SLOT( document_fileReloaded() ) );
	// update file menu
	disconnect( document, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
	// update edit menu
	disconnect( document, SIGNAL( undoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
	disconnect( document, SIGNAL( redoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
	disconnect( document, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
	disconnect( document, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
	disconnect( document, SIGNAL( pasteAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
	// update status bar
	disconnect( document, SIGNAL( cursorPositionChanged( const QPoint& ) ), MonkeyCore::statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
	disconnect( document, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::statusBar(), SLOT( setModified( bool ) ) );
	
	// remove from workspace
	document->removeEventFilter( this );
	mMdiArea->removeSubWindow( document );
	document->hide();
	
	// maximize current window if needed
	if ( maximized )
	{
		pAbstractChild* doc = currentDocument();
		
		if ( doc )
		{
			doc->showMaximized();
		}
	}
}

void pWorkspace::closeFile( const QString& filePath )
{
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		pAbstractChild* document = qobject_cast<pAbstractChild*>( window );
		
		if ( pMonkeyStudio::isSameFile( document->filePath(), filePath ) )
		{
			closeDocument( document );
			return;
		}
	}
}

void pWorkspace::closeCurrentDocument()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		closeDocument( document );
	}
}

bool pWorkspace::closeAllDocuments()
{
	// try save documents
	UISaveFiles::Buttons button = UISaveFiles::saveDocuments( window(), documents(), false );
	
	// close all object, disconnecting them
	if ( button != UISaveFiles::bCancelClose )
	{
		// stop watching files
		foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
		{
			pAbstractChild* document = qobject_cast<pAbstractChild*>( window );
			closeDocument( document, false );
		}
		
		return true;
	}
	else
	{
		return false; //not close IDE
	}
}

void pWorkspace::activateNextDocument()
{
	if ( mViewMode == pWorkspace::NoTabs )
	{
		pAbstractChild* document = currentDocument();
		const QModelIndex curIndex = mOpenedFileExplorer->model()->index( document );
		QModelIndex index = mOpenedFileExplorer->model()->index( document );
		
		index = curIndex.sibling( curIndex.row() +1, curIndex.column() );
		
		if ( !index.isValid() )
		{
			index = curIndex.sibling( 0, curIndex.column() );
		}
		
		document = mOpenedFileExplorer->model()->document( index );
		
		setCurrentDocument( document );
	}
	else
	{
		mMdiArea->activateNextSubWindow();
	}
}

void pWorkspace::activatePreviousDocument()
{
	if ( mViewMode == pWorkspace::NoTabs )
	{
		pAbstractChild* document = currentDocument();
		const QModelIndex curIndex = mOpenedFileExplorer->model()->index( document );
		QModelIndex index = mOpenedFileExplorer->model()->index( document );
		
		index = curIndex.sibling( curIndex.row() -1, curIndex.column() );
		
		if ( !index.isValid() )
		{
			index = curIndex.sibling( mOpenedFileExplorer->model()->rowCount() -1, curIndex.column() );
		}
		
		document = mOpenedFileExplorer->model()->document( index );
		
		setCurrentDocument( document );
	}
	else
	{
		mMdiArea->activatePreviousSubWindow();
	}
}

void pWorkspace::focusEditor()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->setFocus();
	}
}

void pWorkspace::tile()
{
	mMdiArea->tileSubWindows();
}

void pWorkspace::cascade()
{
	mMdiArea->cascadeSubWindows();
}

void pWorkspace::minimize()
{
	setDocumentMode( pWorkspace::NoTabs );
	
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		window->showMinimized();
	}
}

void pWorkspace::restore()
{
	setDocumentMode( pWorkspace::NoTabs );
	
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		window->showNormal();
	}
}

void pWorkspace::setDocumentMode( pWorkspace::ViewMode mode )
{
	if ( mViewMode == mode )
	{
		return;
	}
	
	QMdiSubWindow* document = mMdiArea->currentSubWindow();
	mViewMode = mode;
	
	switch ( mViewMode )
	{
		case pWorkspace::NoTabs:
			mMdiArea->setViewMode( QMdiArea::SubWindowView );
			break;
		case pWorkspace::TopTabs:
			mMdiArea->setTabPosition( QTabWidget::North );
			mMdiArea->setViewMode( QMdiArea::TabbedView );
			break;
		case pWorkspace::BottomTabs:
			mMdiArea->setTabPosition( QTabWidget::South );
			mMdiArea->setViewMode( QMdiArea::TabbedView );
			break;
		case pWorkspace::LeftTabs:
			mMdiArea->setTabPosition( QTabWidget::West );
			mMdiArea->setViewMode( QMdiArea::TabbedView );
			break;
		case pWorkspace::RightTabs:
			mMdiArea->setTabPosition( QTabWidget::East );
			mMdiArea->setViewMode( QMdiArea::TabbedView );
			break;
	}
	
	mOpenedFileExplorer->setVisible( mViewMode == pWorkspace::NoTabs );
	
	if ( document && !document->isMaximized() )
	{
		document->showMaximized();
	}
	
	foreach ( QAction* action, mViewModesGroup->actions() )
	{
		if ( action->data().toInt() == mViewMode )
		{
			if ( !action->isChecked() )
			{
				action->setChecked( true );
			}
			
			return;
		}
	}
}

pAbstractChild* pWorkspace::createNewTextEditor()
{
	pFileDialogResult result = MkSFileDialog::getNewEditorFile( window() );

	// open open file dialog
	QString fileName = result[ "filename" ].toString();
	
	// return 0 if user cancel
	if ( fileName.isEmpty() )
	{
		return 0;
	}
	
	// close file if already open
	closeFile( fileName );

	// create/reset file
	QFile file( fileName );
	
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		MonkeyCore::messageManager()->appendMessage( tr( "Can't create new file '%1'" ).arg( QFileInfo( fileName ).fileName() ) );
		return 0;
	}
	
	// reset file
	file.resize( 0 );
	file.close();
	
	if ( result.value( "addtoproject", false ).toBool() )
	{
		// add files to scope
		MonkeyCore::projectsManager()->addFiles( QStringList( fileName ), result[ "scope" ].value<XUPItem*>() );
	}
	
	// open file
	return MonkeyCore::fileManager()->openFile( fileName, result[ "codec" ].toString() );
}

void pWorkspace::document_fileOpened()
{
	pAbstractChild* document = qobject_cast<pAbstractChild*>( sender() );
	
	if ( QFileInfo( document->filePath() ).isFile() && !mFileWatcher->files().contains( document->filePath() ) )
	{
		mFileWatcher->addPath( document->filePath() );
	}
	
	emit documentOpened( document );
}

void pWorkspace::document_contentChanged()
{
	mContentChangedTimer->start( CONTENT_CHANGED_TIME_OUT );
	pAbstractChild* document = qobject_cast<pAbstractChild*>( sender() );
	
	// externally deleted files make the filewatcher to no longer watch them
	const QString path = document->filePath();
	
	if ( !mFileWatcher->files().contains( path ) )
	{
		mFileWatcher->addPath( path );
	}
	
	emit documentChanged( document );
}

void pWorkspace::document_modifiedChanged( bool modified )
{
	pAbstractChild* document = qobject_cast<pAbstractChild*>( sender() );
	emit documentModifiedChanged( document, modified );
}

void pWorkspace::document_fileClosed()
{
	pAbstractChild* document = qobject_cast<pAbstractChild*>( sender() );
	pMultiToolBar* mtb = MonkeyCore::multiToolBar();
	
	mtb->removeContext( document->context(), true );
	emit documentClosed( document );
}

void pWorkspace::document_fileReloaded()
{
	pAbstractChild* document = qobject_cast<pAbstractChild*>( sender() );
	emit documentReloaded( document );
}

void pWorkspace::contentChangedTimer_timeout()
{
	mContentChangedTimer->stop();
	MonkeyCore::fileManager()->computeModifiedBuffers();
}

void pWorkspace::multitoolbar_notifyChanges()
{
	pMultiToolBar* mtb = MonkeyCore::multiToolBar();
	QToolBar* tb = mtb->currentToolBar();
	bool show = tb && !tb->actions().isEmpty();
	
	mtb->setVisible( show );
}

void pWorkspace::viewModes_triggered( QAction* action )
{
	setDocumentMode( (pWorkspace::ViewMode)action->data().toInt() );
}

void pWorkspace::mdiArea_subWindowActivated( QMdiSubWindow* _document )
{
	pAbstractChild* document = qobject_cast<pAbstractChild*>( _document );
	
	// update gui state
	updateGuiState( document );
	
	// emit file changed
	emit currentDocumentChanged( document );
}

void pWorkspace::internal_urlsDropped( const QList<QUrl>& urls )
{
	// create menu
	QMenu menu;
	QAction* aof = menu.addAction( tr( "Open As &File" ) );
	QAction* aop = menu.addAction( tr( "Open As &Project" ) );
	menu.addSeparator();
	menu.addAction( tr( "Cancel" ) );
	
	// execute menu
	QAction* action = menu.exec( QCursor::pos() );
	
	// check triggered action
	if ( action == aof )
	{
		foreach ( const QUrl& url, urls )
		{
			if ( !url.toLocalFile().trimmed().isEmpty() )
			{
				MonkeyCore::fileManager()->openFile( url.toLocalFile(), pMonkeyStudio::defaultCodec() );
			}
		}
	}
	else if ( action == aop )
	{
		foreach ( const QUrl& url, urls )
		{
			if ( !url.toLocalFile().trimmed().isEmpty() )
			{
				MonkeyCore::projectsManager()->openProject( url.toLocalFile(), pMonkeyStudio::defaultCodec() );
			}
		}
	}
}

void pWorkspace::internal_currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject )
{
	// uninstall old commands
	if ( previousProject ) {
		previousProject->uninstallCommands();
	}
	
	if ( previousProject ) {
		foreach( const QString& pluginName, previousProject->autoActivatePlugins() ) {
			BasePlugin* plugin = MonkeyCore::pluginsManager()->plugin<BasePlugin*>( PluginsManager::stAll, pluginName );
			
			if ( plugin ) {
				plugin->setEnabled( false );
			}
		}
	}
	
	if ( currentProject ) {
		foreach( const QString& pluginName, currentProject->autoActivatePlugins() ) {
			BasePlugin* plugin = MonkeyCore::pluginsManager()->plugin<BasePlugin*>( PluginsManager::stAll, pluginName );
			
			if ( plugin ) {
				plugin->setEnabled( true );
			}
		}
	}
	
	// install new commands
	if ( currentProject ) {
		currentProject->installCommands();
	}
	
	// update menu visibility
	MonkeyCore::mainWindow()->menu_CustomAction_aboutToShow();
}

// file menu
void pWorkspace::fileNew_triggered()
{
	UITemplatesWizard wizard( this );
	wizard.setType( "Files" );
	wizard.exec();
}

void pWorkspace::fileSessionSave_triggered()
{
	QStringList files, projects;
	
	// files
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		pAbstractChild* document = qobject_cast<pAbstractChild*>( window );
		files << document->filePath();
	}
	
	MonkeyCore::settings()->setValue( "Session/Files", files );
	
	// projects
	foreach ( XUPProjectItem* project, MonkeyCore::projectsManager()->topLevelProjects() )
	{
		projects << project->fileName();
	}
	
	MonkeyCore::settings()->setValue( "Session/Projects", projects );
}

void pWorkspace::fileSessionRestore_triggered()
{
	// restore files
	foreach ( const QString& file, MonkeyCore::settings()->value( "Session/Files", QStringList() ).toStringList() )
	{
		if ( !MonkeyCore::fileManager()->openFile( file, pMonkeyStudio::defaultCodec() ) ) // remove it from recents files
		{
			MonkeyCore::recentsManager()->removeRecentFile( file );
		}
	}
	
	// restore projects
	foreach ( const QString& project, MonkeyCore::settings()->value( "Session/Projects", QStringList() ).toStringList() )
	{
		if ( !MonkeyCore::projectsManager()->openProject( project, pMonkeyStudio::defaultCodec() ) ) // remove it from recents projects
		{
			MonkeyCore::recentsManager()->removeRecentProject( project );
		}
	}
}

void pWorkspace::fileSaveCurrent_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		const QString fn = document->filePath();
		mFileWatcher->removePath( fn );
		document->saveFile();
		mFileWatcher->addPath( fn );
	}
}

void pWorkspace::fileSaveAll_triggered()
{
	foreach ( QMdiSubWindow* window, mMdiArea->subWindowList() )
	{
		pAbstractChild* document = qobject_cast<pAbstractChild*>( window );
		const QString fn = document->filePath();
		mFileWatcher->removePath( fn );
		document->saveFile();
		mFileWatcher->addPath( fn );
	}
}

void pWorkspace::fileCloseCurrent_triggered()
{
	closeCurrentDocument();
}

void pWorkspace::fileCloseAll_triggered()
{
	closeAllDocuments();
}

void pWorkspace::fileReload_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		QMessageBox::StandardButton button = QMessageBox::Yes;
		
		if ( document->isModified() )
		{
			button = QMessageBox::question( this, tr( "Confirmation needed..." ), tr( "The file has been modified, reload anyway ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
		}
		
		if ( button == QMessageBox::Yes )
		{
			/*const QString fileName = document->filePath();
			const QString codec = document->textCodec();
			
			closeDocument( document );
			openFile( fileName, codec );*/
			document->reload();
		}
	}
}

void pWorkspace::fileSaveAsBackup_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document ) {
		const QString filePath = MkSFileDialog::getSaveFileName( false, this, tr( "Choose a filename to backup your file" ), document->fileName(), QString::null ).value( "filename" ).toString();
		
		if ( !filePath.isEmpty() ) {
			document->backupFileAs( filePath );
		}
	}
}

void pWorkspace::fileQuickPrint_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->quickPrintFile();
	}
}

void pWorkspace::filePrint_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->printFile();
	}
}

void pWorkspace::fileExit_triggered()
{
	window()->close();
}

// edit menu
void pWorkspace::editSettings_triggered()
{
	UISettings::instance( this )->exec();
}

void pWorkspace::editTranslations_triggered()
{
	const QString locale = TranslationDialog::getLocale( MonkeyCore::translationsManager(), this );
	
	if ( !locale.isEmpty() )
	{
		MonkeyCore::settings()->setValue( "Translations/Locale", locale );
		MonkeyCore::settings()->setValue( "Translations/Accepted", true );
		MonkeyCore::translationsManager()->setCurrentLocale( locale );
		MonkeyCore::translationsManager()->reloadTranslations();
	}
}

void pWorkspace::editUndo_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->undo();
	}
}

void pWorkspace::editRedo_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->redo();
	}
}

void pWorkspace::editCut_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->cut();
	}
}

void pWorkspace::editCopy_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->copy();
	}
}

void pWorkspace::editPaste_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->paste();
	}
}

void pWorkspace::editSearch_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document && !document->editor() )
	{
		document->invokeSearch();
	}
}

void pWorkspace::editGoTo_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		document->goTo();
	}
}

void pWorkspace::editExpandAbbreviation_triggered()
{
	pAbstractChild* document = currentDocument();
	
	if ( document )
	{
		MonkeyCore::abbreviationsManager()->expandMacro( document->editor() );
	}
}

void pWorkspace::editPrepareAPIs_triggered()
{
	pMonkeyStudio::prepareAPIs();
}

// help menu
void pWorkspace::helpAboutApplication_triggered()
{
	UIAbout* dlg = new UIAbout( this );
	dlg->open();
}

void pWorkspace::helpAboutQt_triggered()
{
	qApp->aboutQt();
}

#ifdef __COVERAGESCANNER__
void pWorkspace::helpTestReport_triggered()
{
	UITestReport::instance( this )->exec();
}
#endif
