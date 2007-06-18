/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "UIMain.h"
#include "pMenuBar.h"
#include "pActionManager.h"
#include "pSettings.h"
#include "UIAbout.h"

/*
#include "Settings.h"
#include "TabToolBar.h"
#include "MenuBar.h"
#include "Workspace.h"
#include "StatusBar.h"

#include "UITestReport.h"



#include <QCloseEvent>

*/

#include <QActionGroup>
#include <QStyleFactory>
#include <QStatusBar>

UIMain::UIMain( QWidget* p )
	: pTabbedMainWindow( p )
{
	// create menubar
	initMenuBar();

	// init connections
	initConnections();

	// init gui
	initGui();
}

void UIMain::initMenuBar()
{
	// set icon size for toolbar
	setIconSize( QSize( 16, 16 ) );

	// create menubar menus and actions
	pMenuBar* mb = menuBar();
	mb->menu( "mFile", tr( "&File" ) );
	mb->beginGroup( "mFile" );
		mb->action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/fileopen.png" ), tr( "Ctrl+O" ), tr( "Open a file" ) );
		mb->menu( "mRecents", tr( "&Recents" ), QIcon( ":/Icons/Icons/recentsfiles.png" ) );
		mb->action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ), QString::null, tr( "Clear the recents files list" ) );
		mb->action( "mRecents/aSeparator1" );
		mb->action( "aSeparator1" );
		mb->menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/filesave.png" ) );
		mb->action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/filesave.png" ), tr( "Ctrl+S" ), tr( "Save the current file" ) )->setEnabled( false );
		mb->action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filesaveall.png" ), tr( "Ctrl+Shift+S" ), tr( "Save all files" ) )->setEnabled( false );
		mb->menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/fileclose.png" ) );
		mb->action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/fileclose.png" ), tr( "Ctrl+W" ), tr( "Close the current file" ) )->setEnabled( false );
		mb->action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filecloseall.png" ), tr( "Ctrl+Shift+W" ), tr( "Close all files" ) )->setEnabled( false );
		mb->action( "aSeparator2" );
		mb->action( "aSaveAsTemplate", tr( "Save As &Template" ), QIcon( ":/Icons/Icons/filesaveastemplate.png" ), tr( "Ctrl+T" ), tr( "Save the current file as template" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		mb->action( "aQuickPrint", tr( "Quic&k Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+k" ), tr( "Quick print the current file" ) )->setEnabled( false );
		mb->action( "aPrint", tr( "&Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+P" ), tr( "Print the current file" ) )->setEnabled( false );
		mb->action( "aSeparator4" );
		mb->action( "aQuit", tr( "&Quit" ), QIcon( ":/Icons/Icons/quit.png" ), tr( "Ctrl+Q" ), tr( "Quit the application" ) );
	mb->endGroup();
	mb->menu( "mEdit", tr( "&Edit" ) );
	mb->beginGroup( "mEdit" );
		mb->action( "aSettings", tr( "Settings..." ), QIcon( ":/Icons/Icons/editsettings.png" ) );
		mb->action( "aShortcutsEditor", tr( "Shortcuts Editor..." ), QIcon(), tr( "Ctrl+E" ), tr( "Edit the application shortcuts" ) );
		mb->action( "aSeparator1" );
		mb->action( "aUndo", tr( "&Undo" ), QIcon( ":/Icons/Icons/editundo.png" ), tr( "Ctrl+Z" ), tr( "Undo" ) )->setEnabled( false );
		mb->action( "aRedo", tr( "&Redo" ), QIcon( ":/Icons/Icons/editredo.png" ), tr( "Ctrl+Y" ), tr( "Redo" ) )->setEnabled( false );
		mb->action( "aSeparator2" );
		mb->action( "aCopy", tr( "&Copy" ), QIcon( ":/Icons/Icons/editcopy.png" ), tr( "Ctrl+C" ), tr( "Copy" ) )->setEnabled( false );
		mb->action( "aCut", tr( "Cu&t" ), QIcon( ":/Icons/Icons/editcut.png" ), tr( "Ctrl+X" ), tr( "Cut" ) )->setEnabled( false );
		mb->action( "aPaste", tr( "&Paste" ), QIcon( ":/Icons/Icons/editpaste.png" ), tr( "Ctrl+V" ), tr( "Paste" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		mb->action( "aSearchReplace", tr( "&Search - Replace..." ), QIcon( ":/Icons/Icons/editreplace.png" ), tr( "Ctrl+F" ), tr( "Search - Replace..." ) )->setEnabled( false );
		mb->action( "aGoTo", tr( "&Go To..." ), QIcon( ":/Icons/Icons/editgoto.png" ), tr( "Ctrl+G" ), tr( "Go To..." ) )->setEnabled( false );
	mb->endGroup();
	mb->menu( "mView", tr( "&View" ) );
	mb->beginGroup( "mView" );
		mb->menu( "mStyle", tr( "&Style" ), QIcon( ":/Icons/Icons/viewstyle.png" ) );
		mb->action( "aNext", tr( "&Next tab" ), QIcon( ":/Icons/Icons/buttonnext.png" ), tr( "Alt+Right" ), tr( "Active the next tab" ) )->setEnabled( false );
		mb->action( "aPrevious", tr( "&Previous tab" ), QIcon( ":/Icons/Icons/buttonprevious.png" ), tr( "Alt+Left" ), tr( "Active the previous tab" ) )->setEnabled( false );
		mb->action( "aFilteredView", tr( "&Filtered view" ), QIcon( ":/Icons/Icons/helptesters.png" ), tr( "" ), tr( "Filtered project view" ) )->setEnabled( false );	
	mb->endGroup();
	mb->menu( "mProject", tr( "&Project" ) );
	mb->beginGroup( "mProject" );
		mb->action( "aNew", tr( "&New" ), QIcon( ":/Icons/Icons/projectnew.png" ), tr( "Ctrl+Shift+N" ), tr( "New project..." ) );
		mb->action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/projectopen.png" ), tr( "Ctrl+Shift+O" ), tr( "Open a project..." ) );
		mb->menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		mb->action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectsave.png" ), QString::null, tr( "Save the current project" ) );
		mb->action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectsaveall.png" ), QString::null, tr( "Save all projects" ) );
		mb->menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		mb->action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectclose.png" ), QString::null, tr( "Close the current project" ) );
		mb->action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectcloseall.png" ), QString::null, tr( "Close all projects" ) );
		mb->action( "aSeparator2" );
		mb->action( "aSettings", tr( "Set&tings..." ), QIcon( ":/Icons/Icons/projectsettings.png" ), QString::null, tr( "Project settings" ) );
		mb->action( "aSeparator3" );
		mb->menu( "mRecents", tr( "&Recents" ), QIcon( ":/Icons/Icons/recentsprojects.png" ) );
		mb->action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ), QString::null, tr( "Clear the recents projects list" ) );
		mb->action( "mRecents/aSeparator1" );
	mb->endGroup();
	mb->menu( "mBuild", tr( "&Build" ) );
/* Need to be plugin that instore its actions
	mb->beginGroup( "mBuild" );
		mb->menu( "mBuild", tr( "&Build" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		mb->action( "mBuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildbuild.png" ), QString::null, tr( "Build current" ) );
		mb->action( "mBuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildbuild.png" ), QString::null, tr( "Build all" ) );
		mb->menu( "mRebuild", tr( "&Rebuild" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		mb->action( "mRebuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildrebuild.png" ), QString::null, tr( "Rebuild current" ) );
		mb->action( "mRebuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildrebuild.png" ), QString::null, tr( "Rebuild all" ) );
		mb->action( "aSeparator1" );
		mb->action( "aStop", tr( "&Stop" ), QIcon( ":/Icons/Icons/buildstop.png" ), QString::null, tr( "Stop current" ) );
		mb->action( "aSeparator2" );
		mb->menu( "mClean", tr( "&Clean" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		mb->action( "mClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildclean.png" ), QString::null, tr( "Clean current" ) );
		mb->action( "mClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildclean.png" ), QString::null, tr( "Clean all" ) );
		mb->menu( "mDistClean", tr( "&Dist Clean" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		mb->action( "mDistClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/builddistclean.png" ), QString::null, tr( "Dist clean current" ) );
		mb->action( "mDistClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/builddistclean.png" ), QString::null, tr( "Dist clean all" ) );
		mb->action( "aSeparator3" );
		mb->action( "aExecute", tr( "&Execute" ), QIcon( ":/Icons/Icons/buildexecute.png" ), QString::null, tr( "Execute current" ) );
		mb->action( "aExecuteWithParameters", tr( "Execute with &parameters..." ), QIcon( ":/Icons/Icons/buildexecute.png" ), QString::null, tr( "Execute current with parameters..." ) );
		mb->action( "aSeparator4" );
		mb->action( "aBuildExecute", tr( "Build, Execu&te" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Build and execute current" ) );
		mb->action( "aDistCleanBuildExecute", tr( "Dist Clean, Build, E&xecute" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Dist clean, build and execute current" ) );
	mb->endGroup();
*/
	mb->menu( "mTools", tr( "&Tools" ) );
	mb->beginGroup( "mTools" );
		mb->action( "aEdit", tr( "&Edit..." ), QIcon( ":/Icons/Icons/toolsedit.png" ), QString::null, tr( "Edit tools..." ) );
		mb->action( "aSeparator1" );
	mb->endGroup();
	mb->menu( "mPlugins", tr( "Plu&gins" ) );
	mb->beginGroup( "mPlugins" );
		mb->action( "aManage", tr( "&Manage..." ), QIcon( ":/Icons/Icons/toolsedit.png" ), QString::null, tr( "Manage plugins..." ) );
		mb->action( "aSeparator1" );
	mb->endGroup();
	mb->menu( "mHelp", tr( "&Help" ) );
	mb->beginGroup( "mHelp" );
		mb->action( "aManual", tr( "&Manual" ), QIcon( ":/Icons/Icons/helpassistant.png" ), QString::null, tr( "Manual" ) );
		mb->action( "aSeparator1" );
		mb->action( "aAbout", tr( "&About..." ), QIcon( ":/Icons/Icons/icon.png" ), QString::null, tr( "About application..." ) );
		mb->action( "aAboutQt", tr( "About &Qt..." ), QIcon( ":/Icons/Icons/helpqt.png" ), QString::null, tr( "About Qt..." ) );
#ifdef __COVERAGESCANNER__
		action( "aTestReport", tr( "&Test Report" ), QIcon( ) , tr( "Pause" ), tr( "Coverage Meter Test Report..." ) );
#endif
	mb->endGroup();

	// create action for styles
	agStyles = new QActionGroup( menuBar()->menu( "mView/mStyle" ) );
	foreach ( QString s, QStyleFactory::keys() )
	{
		QAction* a = agStyles->addAction( s );
		a->setCheckable( true );
		if ( settings()->value( "MainWindow/Style" ).toString() == s )
			a->setChecked( true );
	}

	// add styles action to menu
	menuBar()->menu( "mView/mStyle" )->addActions( agStyles->actions() );
}

void UIMain::initConnections()
{
	connect( agStyles, SIGNAL( triggered( QAction* ) ), this, SLOT( agStyles_triggered( QAction* ) ) );
	connect( menuBar()->action( "mEdit/aShortcutsEditor" ), SIGNAL( triggered() ), pActionManager::instance(), SLOT( showSettings() ) );
	connect( menuBar()->action( "mHelp/aAbout" ), SIGNAL( triggered() ), this, SLOT( aboutApplication_triggered() ) );
	connect( menuBar()->action( "mHelp/aAboutQt" ), SIGNAL( triggered() ), this, SLOT( aboutQt_triggered() ) );
#ifdef __COVERAGESCANNER__
	connect( menuBar()->action( "mHelp/aTestReport" ), SIGNAL( triggered() ), this, SLOT( testReport_triggered() ) );
#endif
}

void UIMain::initGui()
{
	statusBar()->show();
}

void UIMain::agStyles_triggered( QAction* a )
{	
	qApp->setStyle( a->text() );
	settings()->setValue( "MainWindow/Style", a->text() );
}

void UIMain::aboutApplication_triggered()
{
	UIAbout::instance( this )->exec();
}

void UIMain::aboutQt_triggered()
{
	qApp->aboutQt();
}

#ifdef __COVERAGESCANNER__
void UIMain::testReport_triggered()
{
	UITestReport::instance( this )->exec();
}
#endif
