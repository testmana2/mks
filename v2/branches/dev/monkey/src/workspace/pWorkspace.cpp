/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pWorkspace.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "pWorkspace.h"
#include "pAbstractChild.h"
#include "pMenuBar.h"
#include "pRecentsManager.h"

#include "pChild.h"

#include <QFileDialog>

pWorkspace::pWorkspace( QWidget* p )
	: pTabbedWorkspace( p )
{
	setBackground( ":/application/icons/application/background.png" );

	// connections
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( internal_currentChanged( int ) ) );
}

pAbstractChild* pWorkspace::currentChild() const
{
	return qobject_cast<pAbstractChild*>( currentDocument() );
}

pAbstractChild* pWorkspace::child( int i ) const
{
	return qobject_cast<pAbstractChild*>( document( i ) );
}

QList<pAbstractChild*> pWorkspace::children() const
{
	QList<pAbstractChild*> l;
	foreach ( QWidget* w, documents() )
		l << qobject_cast<pAbstractChild*>( w );
	return l;
}

void pWorkspace::internal_currentChanged( int i )
{
	// get child
	pAbstractChild* c = child( i );

	// update file menu
	pMenuBar::instance()->action( "mFile/mSave/aCurrent" )->setEnabled( c ? c->isModified() : false );
	pMenuBar::instance()->action( "mFile/mSave/aAll" )->setEnabled( c );
	pMenuBar::instance()->action( "mFile/mClose/aCurrent" )->setEnabled( c );
	pMenuBar::instance()->action( "mFile/mClose/aAll" )->setEnabled( c );
	//pMenuBar::instance()->action( "mFile/aSaveAsTemplate" )->setEnabled( false );
	pMenuBar::instance()->action( "mFile/aQuickPrint" )->setEnabled( c ? c->isPrintAvailable() : false );
	pMenuBar::instance()->action( "mFile/aPrint" )->setEnabled( c ? c->isPrintAvailable() : false );

	// update edit menu
	pMenuBar::instance()->action( "mEdit/aUndo" )->setEnabled( c ? c->isUndoAvailable() : false );
	pMenuBar::instance()->action( "mEdit/aRedo" )->setEnabled( c ? c->isRedoAvailable() : false );
	pMenuBar::instance()->action( "mEdit/aCut" )->setEnabled( c ? c->isCopyAvailable() : false );
	pMenuBar::instance()->action( "mEdit/aCopy" )->setEnabled( c ? c->isCopyAvailable() : false );
	pMenuBar::instance()->action( "mEdit/aPaste" )->setEnabled( c ? c->isPasteAvailable() : false );
	pMenuBar::instance()->action( "mEdit/aSearchReplace" )->setEnabled( c ? c->isSearchReplaceAvailable() : false );
	pMenuBar::instance()->action( "mEdit/aGoTo" )->setEnabled( c ? c->isGoToAvailable() : false );

	// update view menu
	pMenuBar::instance()->action( "mView/aNext" )->setEnabled( c );
	pMenuBar::instance()->action( "mView/aPrevious" )->setEnabled( c );

	// update status bar
	//pMenuBar::instance()->setCursorPosition( c ? c->cursorPosition() : QPoint( -1, -1 ) );
	//pMenuBar::instance()->setModified( c ? c->isModified() : false );
	//pMenuBar::instance()->setDocumentMode( c ? c->documentMode() : AbstractChild::mNone );
	//pMenuBar::instance()->setLayoutMode( c ? c->layoutMode() : AbstractChild::lNone );
	//pMenuBar::instance()->setFileName( c ? c->currentFile() : QString::null );

	// left corner widget
	//
}

void pWorkspace::openFile( const QString& s, pAbstractProjectProxy* pp, const QPoint& p )
{
	// get file info
	const QFileInfo f( s );

	// get canonical file path
	QString v = f.canonicalFilePath();

	// if it not exists
	if ( !f.exists() )
	{
		// remove it from recents files
		pRecentsManager::instance()->removeRecentFile( v );
		return;
	}

	// open file
/*
	if ( pluginsManager()->childPluginOpenFile( v, p ) )
	{}
	else
*/
	{
		// create child
		pAbstractChild* c = new pChild;
	
		// set child proxy
		c->setProxy( pp );
	
		// add file to child
		c->addFile( s, p );
	
		// update file menu
		connect( c, SIGNAL( modifiedChanged( bool ) ), pMenuBar::instance()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
		// update edit menu
		connect( c, SIGNAL( undoAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( redoAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( pasteAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( searchReplaceAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aSearchReplace" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( goToAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aGoTo" ), SLOT( setEnabled( bool ) ) );
		// update status bar
		//connect( c, SIGNAL( cursorPositionChanged( const QPoint& ) ), statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
		//connect( c, SIGNAL( modifiedChanged( bool ) ), statusBar(), SLOT( setModified( bool ) ) );
		//connect( c, SIGNAL( documentModeChanged( AbstractChild::DocumentMode ) ), statusBar(), SLOT( setDocumentMode( AbstractChild::DocumentMode ) ) );
		//connect( c, SIGNAL( layoutModeChanged( AbstractChild::LayoutMode ) ), statusBar(), SLOT( setLayoutMode( AbstractChild::LayoutMode ) ) );
		//connect( c, SIGNAL( currentFileChanged( const QString& ) ), statusBar(), SLOT( setFileName( const QString& ) ) );
	
		addTab( c, c->currentFileName() );
	}

	// append file to recents
	if ( !pp )
		pRecentsManager::instance()->addRecentFile( v );
}

// file menu
void pWorkspace::fileOpen_triggered()
{
	// get last file open path
	const QString mPath = pRecentsManager::instance()->recentFileOpenPath();

	// get available filters
	//QString mFilters = pluginsManager()->childsFilters().join( ";;" );

	// open open file dialog
	QStringList l = QFileDialog::getOpenFileNames( this, tr( "Choose the file(s) to open" ), mPath ); //, mFilters );

	// for each entry, open file
	foreach ( QString s, l )
		openFile( s );

	// store file open path
	if ( !l.isEmpty() )
		pRecentsManager::instance()->setRecentFileOpenPath( QFileInfo( l.at( 0 ) ).canonicalPath() );
}

void pWorkspace::fileSaveCurrent_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->saveCurrentFile();
}

void pWorkspace::fileSaveAll_triggered()
{
	foreach ( pAbstractChild* c, children() )
		c->saveFiles();
}

void pWorkspace::fileCloseCurrent_triggered()
{
	closeCurrentTab();
}

void pWorkspace::fileCloseAll_triggered()
{
	closeAllTabs();
}

void pWorkspace::fileSaveAsTemplate_triggered()
{
	qWarning( "Not yet implemented." );
}

void pWorkspace::fileQuickPrint_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->quickPrintCurrentFile();
}

void pWorkspace::filePrint_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->printCurrentFile();
}

void pWorkspace::fileExit_triggered()
{
	window()->close();
}

// edit menu
void pWorkspace::editSettings_triggered()
{
	//UISettings::self( window() )->exec();
}

void pWorkspace::editUndo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->undo();
}

void pWorkspace::editRedo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->redo();
}

void pWorkspace::editCut_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->cut();
}

void pWorkspace::editCopy_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->copy();
}

void pWorkspace::editPaste_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->paste();
}

void pWorkspace::editSearchReplace_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->searchReplace();
}

void pWorkspace::editGoTo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->goTo();
}

// view menu
void pWorkspace::viewNext_triggered()
{
/*
	if ( currentIndex() +1 == count() )
		setCurrentIndex( 0 );
	else
		setCurrentIndex( currentIndex() +1 );
*/
}

void pWorkspace::viewPrevious_triggered()
{
/*
	if ( currentIndex() -1 == -1 )
		setCurrentIndex( count() -1 );
	else
		setCurrentIndex( currentIndex() -1 );
*/
}

// project menu
void pWorkspace::projectNew_triggered()
{
	//projectsManager()->newProject();
}

void pWorkspace::projectOpen_triggered()
{
	//projectsManager()->openProject();
}

void pWorkspace::projectSaveCurrent_triggered()
{
	//projectsManager()->saveCurrent();
}

void pWorkspace::projectSaveAll_triggered()
{
	//projectsManager()->saveAll();
}

void pWorkspace::projectCloseCurrent_triggered()
{
	//projectsManager()->closeCurrent();
}

void pWorkspace::projectCloseAll_triggered()
{
	//projectsManager()->closeAll();
}

void pWorkspace::projectSettings_triggered()
{
	//projectsManager()->projectSettings();
}
