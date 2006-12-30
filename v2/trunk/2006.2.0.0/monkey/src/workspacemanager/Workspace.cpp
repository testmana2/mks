#include "Workspace.h"
#include "Settings.h"
#include "MenuBar.h"
#include "TabToolBar.h"
//#include "ProjectsManager.h"
#include "StatusBar.h"
#include "AbstractChild.h"
#include "RecentsManager.h"
#include "PluginsManager.h"
#include "UISaveFiles.h"
//
#include <QFileDialog>
#include <QFileInfo>
#include <QListWidget>
//
QPointer<Workspace> Workspace::mSelf = 0L;
//
Workspace* Workspace::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new Workspace( p );
	return mSelf;
}
//
Workspace::Workspace( QWidget* p )
	: pTabWorkspace( p )
{
	initialize();
}
//
Workspace::~Workspace()
{
}
//
void Workspace::initialize()
{
	// set mode
	setTabShape( QTabBar::RoundedSouth );
	setTabMode( tmMDI );
	// itself
	connect( this, SIGNAL( tabInserted( int ) ), this, SLOT( tabInserted( int ) ) );
	connect( this, SIGNAL( tabRemoved( int ) ), this, SLOT( tabRemoved( int ) ) );
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( tabChanged( int ) ) );
	// recentsmanager
	connect( RecentsManager::self(), SIGNAL( openFileRequested( const QString& ) ), this, SLOT( openFile( const QString& ) ) );
	connect( RecentsManager::self(), SIGNAL( openProjectRequested( const QString& ) ), this, SLOT( openFile( const QString& ) ) );
	// file connection
	connect( menuBar()->action( "mFile/aOpen" ), SIGNAL( triggered() ), this, SLOT( fileOpen_triggered() ) );
	connect( menuBar()->action( "mFile/mSave/aCurrent" ), SIGNAL( triggered() ), this, SLOT( fileSaveCurrent_triggered() ) );
	connect( menuBar()->action( "mFile/mSave/aAll" ), SIGNAL( triggered() ), this, SLOT( fileSaveAll_triggered() ) );
	connect( menuBar()->action( "mFile/mClose/aCurrent" ), SIGNAL( triggered() ), this, SLOT( fileCloseCurrent_triggered() ) );
	connect( menuBar()->action( "mFile/mClose/aAll" ), SIGNAL( triggered() ), this, SLOT( fileCloseAll_triggered() ) );
	connect( menuBar()->action( "mFile/aSaveAsTemplate" ), SIGNAL( triggered() ), this, SLOT( fileSaveAsTemplate_triggered() ) );
	connect( menuBar()->action( "mFile/aQuickPrint" ), SIGNAL( triggered() ), this, SLOT( fileQuickPrint_triggered() ) );
	connect( menuBar()->action( "mFile/aPrint" ), SIGNAL( triggered() ), this, SLOT( filePrint_triggered() ) );
	connect( menuBar()->action( "mFile/aExit" ), SIGNAL( triggered() ), this, SLOT( fileExit_triggered() ) );
	// edit connection
	connect( menuBar()->action( "mEdit/aUndo" ), SIGNAL( triggered() ), this, SLOT( editUndo_triggered() ) );
	connect( menuBar()->action( "mEdit/aRedo" ), SIGNAL( triggered() ), this, SLOT( editRedo_triggered() ) );
	connect( menuBar()->action( "mEdit/aCut" ), SIGNAL( triggered() ), this, SLOT( editCut_triggered() ) );
	connect( menuBar()->action( "mEdit/aCopy" ), SIGNAL( triggered() ), this, SLOT( editCopy_triggered() ) );
	connect( menuBar()->action( "mEdit/aPaste" ), SIGNAL( triggered() ), this, SLOT( editPaste_triggered() ) );
	connect( menuBar()->action( "mEdit/aSearchReplace" ), SIGNAL( triggered() ), this, SLOT( editSearchReplace_triggered() ) );
	connect( menuBar()->action( "mEdit/aGoTo" ), SIGNAL( triggered() ), this, SLOT( editGoTo_triggered() ) );
	// view connection
	connect( menuBar()->action( "mView/aNext" ), SIGNAL( triggered() ), this, SLOT( viewNext_triggered() ) );
	connect( menuBar()->action( "mView/aPrevious" ), SIGNAL( triggered() ), this, SLOT( viewPrevious_triggered() ) );
}
//
void Workspace::updateTabNumbers( int i )
{
	// loop documents starting at id i
	for ( int j = i; j < count(); j++ )
	{
		if ( j > 9 )
			return;
		// got tab text
		QString s = tabText( j );
		// look index of cut part
		int k = s.indexOf( ":" );
		// set new tab caption
		setTabText( j, QString( "&%1: %2" ).arg( j ).arg( s.mid( k != -1 ? k +2 : 0 ) ) );
	}
}
//
void Workspace::updateWorkspace()
{
	// a new child has been activated, update gui
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
	{
		// update file menu
		menuBar()->action( "mFile/mSave/aCurrent" )->setEnabled( c->isModified() );
		menuBar()->action( "mFile/mSave/aAll" )->setEnabled( true );
		menuBar()->action( "mFile/mClose/aCurrent" )->setEnabled( true );
		menuBar()->action( "mFile/mClose/aAll" )->setEnabled( true );
		menuBar()->action( "mFile/aSaveAsTemplate" )->setEnabled( false );
		menuBar()->action( "mFile/aQuickPrint" )->setEnabled( c->isPrintAvailable() );
		menuBar()->action( "mFile/aPrint" )->setEnabled( c->isPrintAvailable() );
		// update edit menu
		menuBar()->action( "mEdit/aUndo" )->setEnabled( c->isUndoAvailable() );
		menuBar()->action( "mEdit/aRedo" )->setEnabled( c->isRedoAvailable() );
		menuBar()->action( "mEdit/aCut" )->setEnabled( c->isCopyAvailable() );
		menuBar()->action( "mEdit/aCopy" )->setEnabled( c->isCopyAvailable() );
		menuBar()->action( "mEdit/aPaste" )->setEnabled( c->isPasteAvailable() );
		menuBar()->action( "mEdit/aSearchReplace" )->setEnabled( c->isSearchReplaceAvailable() );
		menuBar()->action( "mEdit/aGoTo" )->setEnabled( c->isGoToAvailable() );
		// update view menu
		menuBar()->action( "mView/aNext" )->setEnabled( true );
		menuBar()->action( "mView/aPrevious" )->setEnabled( true );
		// update status bar
		statusBar()->setCursorPosition( c->cursorPosition() );
		statusBar()->setModified( c->isModified() );
		statusBar()->setMode( c->mode() );
		statusBar()->setFileName( c->currentFile() );
	}
	else
	{
		// update file menu
		menuBar()->action( "mFile/mSave/aCurrent" )->setEnabled( false );
		menuBar()->action( "mFile/mSave/aAll" )->setEnabled( false );
		menuBar()->action( "mFile/mClose/aCurrent" )->setEnabled( false );
		menuBar()->action( "mFile/mClose/aAll" )->setEnabled( false );
		menuBar()->action( "mFile/aSaveAsTemplate" )->setEnabled( false );
		menuBar()->action( "mFile/aQuickPrint" )->setEnabled( false );
		menuBar()->action( "mFile/aPrint" )->setEnabled( false );
		// update edit menu
		menuBar()->action( "mEdit/aUndo" )->setEnabled( false );
		menuBar()->action( "mEdit/aRedo" )->setEnabled( false );
		menuBar()->action( "mEdit/aCut" )->setEnabled( false );
		menuBar()->action( "mEdit/aCopy" )->setEnabled( false );
		menuBar()->action( "mEdit/aPaste" )->setEnabled( false );
		menuBar()->action( "mEdit/aSearchReplace" )->setEnabled( false );
		menuBar()->action( "mEdit/aGoTo" )->setEnabled( false );
		// update view menu
		menuBar()->action( "mView/aNext" )->setEnabled( false );
		menuBar()->action( "mView/aPrevious" )->setEnabled( false );
		// update status bar
		statusBar()->setCursorPosition( QPoint( -1, -1 ) );
		statusBar()->setModified( false );
		statusBar()->setMode( AbstractChild::mNone );
		statusBar()->setFileName( QString::null );
	}
}
//
void Workspace::tabChanged( int )
{
	// updating workspace
	updateWorkspace();
}
//
void Workspace::tabInserted( int i )
{
	// if it s the first child we update workspace
	if ( count() == 1 )
		updateWorkspace();
	// update view menu
	updateTabNumbers( i );
}
//
void Workspace::tabRemoved( int i )
{
	// if there is no child we update workspace
	if ( !count() )
		updateWorkspace();
	// update view menu
	updateTabNumbers( i );
}
// file menu
void Workspace::fileOpen_triggered()
{
	const QString mPath = settings()->value( "Recents/OpenPath" ).toString();
	QString mFilters = PluginsManager::self()->childsFilters().join( ";;" );
	QStringList l = QFileDialog::getOpenFileNames( this, tr( "Choose the file(s) to open" ), mPath, mFilters );
	if ( l.count() )
	{
		settings()->setValue( "Recents/OpenPath", QFileInfo( l.at( 0 ) ).canonicalPath() );
		foreach ( QString s, l )
			openFile( s );
	}
}
//
void Workspace::fileSaveCurrent_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->saveCurrentFile();
}
//
void Workspace::fileSaveAll_triggered()
{
	AbstractChild* c;
	foreach ( QWidget* w, documents() )
	{
		c = qobject_cast<AbstractChild*>( w );
		if ( c )
			c->saveFiles();
	}
}
//
void Workspace::fileCloseCurrent_triggered()
{
	if ( currentWidget() )
		currentWidget()->close();
}
//
void Workspace::fileCloseAll_triggered()
{
	while ( currentWidget() )
		currentWidget()->close();
}
//
void Workspace::fileSaveAsTemplate_triggered()
{
	qWarning( "Not yet implemented." );
}
//
void Workspace::fileQuickPrint_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->quickPrintCurrentFile();
}
//
void Workspace::filePrint_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->printCurrentFile();
}
//
void Workspace::fileExit_triggered()
{
	if ( parentWidget() )
		parentWidget()->close();
}
// edit menu
void Workspace::editSettings_triggered()
{
	qWarning( "Not yet implemented" );
}
//
void Workspace::editUndo_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->undo();
}
//
void Workspace::editRedo_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->redo();
}
//
void Workspace::editCut_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->cut();
}
//
void Workspace::editCopy_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->copy();
}
//
void Workspace::editPaste_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->paste();
}
//
void Workspace::editSearchReplace_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->searchReplace();
}
//
void Workspace::editGoTo_triggered()
{
	AbstractChild* c = qobject_cast<AbstractChild*>( currentWidget() );
	if ( c )
		c->goTo();
}
//
void Workspace::viewNext_triggered()
{
	if ( currentIndex() +1 == count() )
		setCurrentIndex( 0 );
	else
		setCurrentIndex( currentIndex() +1 );	
}
//
void Workspace::viewPrevious_triggered()
{
	if ( currentIndex() -1 == -1 )
		setCurrentIndex( count() -1 );
	else
		setCurrentIndex( currentIndex() -1 );
}
//
void Workspace::openFile( const QString& s, AbstractProject* p )
{
	if ( PluginsManager::self()->childPluginOpenFile( s, p ) && !p )
		recentsManager()->addRecentFile( s );
	
}
//
int Workspace::addChild( AbstractChild* c, const QString& s )
{
	int i = addTab( c, s );
	if ( i != -1 )
	{
		// update file menu
		connect( c, SIGNAL( modifiedChanged( bool ) ), menuBar()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
		// update edit menu
		connect( c, SIGNAL( undoAvailableChanged( bool ) ), menuBar()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( redoAvailableChanged( bool ) ), menuBar()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), menuBar()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), menuBar()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( pasteAvailableChanged( bool ) ), menuBar()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( searchReplaceAvailableChanged( bool ) ), menuBar()->action( "mEdit/aSearchReplace" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( goToAvailableChanged( bool ) ), menuBar()->action( "mEdit/aGoTo" ), SLOT( setEnabled( bool ) ) );
		// update status bar
		connect( c, SIGNAL( cursorPositionChanged( const QPoint& ) ), statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
		connect( c, SIGNAL( modifiedChanged( bool ) ), statusBar(), SLOT( setModified( bool ) ) );
		connect( c, SIGNAL( modeChanged( AbstractChild::Mode ) ), statusBar(), SLOT( setMode( AbstractChild::Mode ) ) );
		connect( c, SIGNAL( currentFileChanged( const QString& ) ), statusBar(), SLOT( setFileName( const QString& ) ) );
		// update save child event
		connect( c, SIGNAL( closeEvent( AbstractChild*, QCloseEvent* ) ), this, SLOT( childCloseEvent( AbstractChild*, QCloseEvent* ) ) );
		// global workspace update requested
		connect( c, SIGNAL( updateWorkspaceRequested() ), this, SLOT( updateWorkspace() ) );
	}
	return i;
}
//
void Workspace::childCloseEvent( AbstractChild* c, QCloseEvent* e )
{
	UISaveFiles::execute( c, e );
}
//
Settings* Workspace::settings()
{
	return Settings::current();
}
//
MenuBar* Workspace::menuBar()
{
	return MenuBar::self();
}
//
RecentsManager* Workspace::recentsManager()
{
	return RecentsManager::self();
}
//
TabToolBar* Workspace::tabToolBar()
{
	return TabToolBar::self();
}
//
ProjectsManager* Workspace::projectsManager()
{
	return 0; //ProjectsManager::self();
}
//
StatusBar* Workspace::statusBar()
{
	return StatusBar::self();
}
