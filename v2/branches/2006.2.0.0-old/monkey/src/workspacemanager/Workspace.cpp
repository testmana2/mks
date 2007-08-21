#include "Workspace.h"
#include "Settings.h"
#include "UISettings.h"
#include "MenuBar.h"
#include "TabToolBar.h"
#include "ProjectsManager.h"
#include "StatusBar.h"
#include "AbstractChild.h"
#include "RecentsManager.h"
#include "PluginsManager.h"
#include "UISaveFiles.h"
#include "AbstractProjectProxy.h"
#include "LeftCorner.h"
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
	// set left corner
	//setCornerWidget( LeftCorner::self( this ), Qt::TopLeftCorner );
	// set mode
	setTabShape( QTabBar::RoundedNorth );
	setTabMode( tmMDI );
	// add project manager to dock area
	tabToolBar()->bar( TabToolBar::Left )->appendTab( projectsManager(), QPixmap( ":/Icons/Icons/project.png" ), tr( "Projects" ) );
	connect( projectsManager(), SIGNAL( fileOpenRequested( const QString&, AbstractProjectProxy* ) ), this, SLOT( openFile( const QString&, AbstractProjectProxy* ) ) );
	// itself
	connect( this, SIGNAL( tabInserted( int ) ), this, SLOT( tabInserted( int ) ) );
	connect( this, SIGNAL( tabRemoved( int ) ), this, SLOT( tabRemoved( int ) ) );
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( tabChanged( int ) ) );
	// recentsmanager
	connect( RecentsManager::self(), SIGNAL( openFileRequested( const QString& ) ), this, SLOT( openFile( const QString& ) ) );
	connect( RecentsManager::self(), SIGNAL( openProjectRequested( const QString& ) ), this, SLOT( openProject( const QString& ) ) );
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
	connect( menuBar()->action( "mEdit/aSettings" ), SIGNAL( triggered() ), this, SLOT( editSettings_triggered() ) );
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
	// project connection
	connect( menuBar()->action( "mProject/aNew" ), SIGNAL( triggered() ), this, SLOT( projectNew_triggered() ) );
	connect( menuBar()->action( "mProject/aOpen" ), SIGNAL( triggered() ), this, SLOT( projectOpen_triggered() ) );
	connect( menuBar()->action( "mProject/mSave/aCurrent" ), SIGNAL( triggered() ), this, SLOT( projectSaveCurrent_triggered() ) );
	connect( menuBar()->action( "mProject/mSave/aAll" ), SIGNAL( triggered() ), this, SLOT( projectSaveAll_triggered() ) );
	connect( menuBar()->action( "mProject/mClose/aCurrent" ), SIGNAL( triggered() ), this, SLOT( projectCloseCurrent_triggered() ) );
	connect( menuBar()->action( "mProject/mClose/aAll" ), SIGNAL( triggered() ), this, SLOT( projectCloseAll_triggered() ) );
	connect( menuBar()->action( "mProject/aSettings" ), SIGNAL( triggered() ), this, SLOT( projectSettings_triggered() ) );
	// plugins menu
	connect( menuBar()->action( "mPlugins/aManage" ), SIGNAL( triggered() ), pluginsManager(), SLOT( manageRequested() ) );
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
	// update file menu
	menuBar()->action( "mFile/mSave/aCurrent" )->setEnabled( c ? c->isModified() : false );
	menuBar()->action( "mFile/mSave/aAll" )->setEnabled( c );
	menuBar()->action( "mFile/mClose/aCurrent" )->setEnabled( c );
	menuBar()->action( "mFile/mClose/aAll" )->setEnabled( c );
	menuBar()->action( "mFile/aSaveAsTemplate" )->setEnabled( false );
	menuBar()->action( "mFile/aQuickPrint" )->setEnabled( c ? c->isPrintAvailable() : false );
	menuBar()->action( "mFile/aPrint" )->setEnabled( c ? c->isPrintAvailable() : false );
	// update edit menu
	menuBar()->action( "mEdit/aUndo" )->setEnabled( c ? c->isUndoAvailable() : false );
	menuBar()->action( "mEdit/aRedo" )->setEnabled( c ? c->isRedoAvailable() : false );
	menuBar()->action( "mEdit/aCut" )->setEnabled( c ? c->isCopyAvailable() : false );
	menuBar()->action( "mEdit/aCopy" )->setEnabled( c ? c->isCopyAvailable() : false );
	menuBar()->action( "mEdit/aPaste" )->setEnabled( c ? c->isPasteAvailable() : false );
	menuBar()->action( "mEdit/aSearchReplace" )->setEnabled( c ? c->isSearchReplaceAvailable() : false );
	menuBar()->action( "mEdit/aGoTo" )->setEnabled( c ? c->isGoToAvailable() : false );
	// update view menu
	menuBar()->action( "mView/aNext" )->setEnabled( c );
	menuBar()->action( "mView/aPrevious" )->setEnabled( c );
	// update status bar
	statusBar()->setCursorPosition( c ? c->cursorPosition() : QPoint( -1, -1 ) );
	statusBar()->setModified( c ? c->isModified() : false );
	statusBar()->setDocumentMode( c ? c->documentMode() : AbstractChild::mNone );
	statusBar()->setLayoutMode( c ? c->layoutMode() : AbstractChild::lNone );
	statusBar()->setFileName( c ? c->currentFile() : QString::null );
	// left corner widget
	if ( c && c->cornerWidget() && cornerWidget( Qt::TopLeftCorner ) != c->cornerWidget() )
		setCornerWidget( c->cornerWidget(), Qt::TopLeftCorner );
	else if ( !count() || ( c && !c->cornerWidget() ) )
		delete cornerWidget( Qt::TopLeftCorner );
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
//
void Workspace::tabChanged( int )
{
	// updating workspace
	updateWorkspace();
}
// file menu
void Workspace::fileOpen_triggered()
{
	const QString mPath = settings()->value( "Recents/FileOpenPath" ).toString();
	QString mFilters = pluginsManager()->childsFilters().join( ";;" );
	QStringList l = QFileDialog::getOpenFileNames( this, tr( "Choose the file(s) to open" ), mPath, mFilters );
	foreach ( QString s, l )
		openFile( s );
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
		widget( currentIndex() )->close();
		//currentWidget()->close();
}
//
void Workspace::fileCloseAll_triggered()
{
	foreach ( QWidget* w, documents() )
		w->close();
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
	UISettings::self( window() )->exec();
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
// view menu
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
// project menu
void Workspace::projectNew_triggered()
{
	projectsManager()->newProject();
}
//
void Workspace::projectOpen_triggered()
{
	projectsManager()->openProject();
}
//
void Workspace::projectSaveCurrent_triggered()
{
	projectsManager()->saveCurrent();
}
//
void Workspace::projectSaveAll_triggered()
{
	projectsManager()->saveAll();
}
//
void Workspace::projectCloseCurrent_triggered()
{
	projectsManager()->closeCurrent();
}
//
void Workspace::projectCloseAll_triggered()
{
	projectsManager()->closeAll();
}
//
void Workspace::projectSettings_triggered()
{
	projectsManager()->projectSettings();
}
//
void Workspace::openFile( const QString& s, AbstractProjectProxy* p )
{
	// activate window if it s not it
	if ( qApp->activeWindow() != window() )
		qApp->setActiveWindow( window() );
	//
	const QFileInfo f( s );
	QString v = f.canonicalFilePath();
	if ( !f.exists() )
	{
		// remove it from recents files
		recentsManager()->removeRecentFile( v );
		return;
	}
	// open file
	if ( pluginsManager()->childPluginOpenFile( v, p ) )
	{
		// save recent file path
		settings()->setValue( "Recents/FileOpenPath", f.canonicalPath() );
		// append it to recents
		if ( !p )
			recentsManager()->addRecentFile( v );
	}
	//else
		//qWarning( qPrintable( tr( "Can't open file (or already open) :\n%1" ).arg( v ) ) );
}
//
void Workspace::openProject( const QString& s )
{
	projectsManager()->openProject( s );
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
		connect( c, SIGNAL( documentModeChanged( AbstractChild::DocumentMode ) ), statusBar(), SLOT( setDocumentMode( AbstractChild::DocumentMode ) ) );
		connect( c, SIGNAL( layoutModeChanged( AbstractChild::LayoutMode ) ), statusBar(), SLOT( setLayoutMode( AbstractChild::LayoutMode ) ) );
		connect( c, SIGNAL( currentFileChanged( const QString& ) ), statusBar(), SLOT( setFileName( const QString& ) ) );
		// update save child event
		connect( c, SIGNAL( closeEvent( AbstractChild*, QCloseEvent* ) ), this, SLOT( childCloseEvent( AbstractChild*, QCloseEvent* ) ) );
		// global workspace update requested
		connect( c, SIGNAL( updateWorkspaceRequested() ), this, SLOT( updateWorkspace() ) );
		// register child with bridge debugger
		BasePlugin* bp = pluginsManager()->plugin( "Debugger" );
		if ( bp )
			connect( c, SIGNAL( void toggleBreakPoint( MonkeyEditor*, int, bool ) ), bp, SLOT( void toggleBreakPoint( MonkeyEditor*, int, bool ) ) );
	}
	return i;
}
//
void Workspace::childCloseEvent( AbstractChild* c, QCloseEvent* e )
{
	UISaveFiles::execute( c, e );
	if ( e->isAccepted() )
		removeTab( c );
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
	return ProjectsManager::self();
}
//
PluginsManager* Workspace::pluginsManager()
{
	return PluginsManager::self();
}
//
StatusBar* Workspace::statusBar()
{
	return StatusBar::self();
}
