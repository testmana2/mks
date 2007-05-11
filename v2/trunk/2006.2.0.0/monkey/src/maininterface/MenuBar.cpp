/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 01h15
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : MenuBar.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class is the monkey studio menuBar created using the pMenuBar class
 ********************************************************************************************************/
#include "MenuBar.h"
//
QPointer<MenuBar> MenuBar::mSelf = 0L;
//
MenuBar* MenuBar::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new MenuBar( p );
	return mSelf;
}
//
MenuBar::MenuBar( QWidget* p )
	: pMenuBar( p )
{
	initialize();
}
//
void MenuBar::initialize()
{	
	menu( "mFile", tr( "&File" ) );
	beginGroup( "mFile" );
		action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/fileopen.png" ), tr( "Ctrl+O" ), tr( "Open a file" ) );
		menu( "mRecents", tr( "&Recents" ), QIcon( ":/Icons/Icons/recentsfiles.png" ) );
		action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ), QString::null, tr( "Clear the recents files list" ) );
		action( "mRecents/aSeparator1" );
		action( "aSeparator1" );
		menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/filesave.png" ) );
		action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/filesave.png" ), tr( "Ctrl+S" ), tr( "Save the current file" ) )->setEnabled( false );
		action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filesaveall.png" ), tr( "Ctrl+Shift+S" ), tr( "Save all files" ) )->setEnabled( false );
		menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/fileclose.png" ) );
		action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/fileclose.png" ), tr( "Ctrl+W" ), tr( "Close the current file" ) )->setEnabled( false );
		action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filecloseall.png" ), tr( "Ctrl+Shift+W" ), tr( "Close all files" ) )->setEnabled( false );
		action( "aSeparator2" );
		action( "aSaveAsTemplate", tr( "Save As &Template" ), QIcon( ":/Icons/Icons/filesaveastemplate.png" ), tr( "Ctrl+T" ), tr( "Save the current file as template" ) )->setEnabled( false );
		action( "aSeparator3" );
		action( "aQuickPrint", tr( "&Quick Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+Q" ), tr( "Quick print the current file" ) )->setEnabled( false );
		action( "aPrint", tr( "&Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+P" ), tr( "Print the current file" ) )->setEnabled( false );
		action( "aSeparator4" );
		action( "aExit", tr( "&Exit" ), QIcon( ":/Icons/Icons/exit.png" ), tr( "Ctrl+E" ), tr( "Exit the application" ) );
	endGroup();
	menu( "mEdit", tr( "&Edit" ) );
	beginGroup( "mEdit" );
		action( "aSettings", tr( "Settings..." ), QIcon( ":/Icons/Icons/editsettings.png" ) );
		action( "aSeparator1" );
		action( "aUndo", tr( "&Undo" ), QIcon( ":/Icons/Icons/editundo.png" ), tr( "Ctrl+Z" ), tr( "Undo" ) )->setEnabled( false );
		action( "aRedo", tr( "&Redo" ), QIcon( ":/Icons/Icons/editredo.png" ), tr( "Ctrl+Y" ), tr( "Redo" ) )->setEnabled( false );
		action( "aSeparator2" );
		action( "aCopy", tr( "&Copy" ), QIcon( ":/Icons/Icons/editcopy.png" ), tr( "Ctrl+C" ), tr( "Copy" ) )->setEnabled( false );
		action( "aCut", tr( "Cu&t" ), QIcon( ":/Icons/Icons/editcut.png" ), tr( "Ctrl+X" ), tr( "Cut" ) )->setEnabled( false );
		action( "aPaste", tr( "&Paste" ), QIcon( ":/Icons/Icons/editpaste.png" ), tr( "Ctrl+V" ), tr( "Paste" ) )->setEnabled( false );
		action( "aSeparator3" );
		action( "aSearchReplace", tr( "&Search - Replace..." ), QIcon( ":/Icons/Icons/editreplace.png" ), tr( "Ctrl+F" ), tr( "Search - Replace..." ) )->setEnabled( false );
		action( "aGoTo", tr( "&Go To..." ), QIcon( ":/Icons/Icons/editgoto.png" ), tr( "Ctrl+G" ), tr( "Go To..." ) )->setEnabled( false );
	endGroup();
	menu( "mView", tr( "&View" ) );
	beginGroup( "mView" );
		menu( "mStyle", tr( "&Style" ), QIcon( ":/Icons/Icons/viewstyle.png" ) );
		action( "aNext", tr( "&Next tab" ), QIcon( ":/Icons/Icons/buttonnext.png" ), tr( "Alt+Right" ), tr( "Active the next tab" ) )->setEnabled( false );
		action( "aPrevious", tr( "&Previous tab" ), QIcon( ":/Icons/Icons/buttonprevious.png" ), tr( "Alt+Left" ), tr( "Active the previous tab" ) )->setEnabled( false );
	endGroup();
	menu( "mProject", tr( "&Project" ) );
	beginGroup( "mProject" );
		action( "aNew", tr( "&New" ), QIcon( ":/Icons/Icons/projectnew.png" ), tr( "Ctrl+Shift+N" ), tr( "New project..." ) );
		action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/projectopen.png" ), tr( "Ctrl+Shift+O" ), tr( "Open a project..." ) );
		menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectsave.png" ), QString::null, tr( "Save the current project" ) );
		action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectsaveall.png" ), QString::null, tr( "Save all projects" ) );
		menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectclose.png" ), QString::null, tr( "Close the current project" ) );
		action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectcloseall.png" ), QString::null, tr( "Close all projects" ) );
		action( "aSeparator2" );
		action( "aSettings", tr( "Set&tings..." ), QIcon( ":/Icons/Icons/projectsettings.png" ), QString::null, tr( "Project settings" ) );
		action( "aSeparator3" );
		menu( "mRecents", tr( "&Recents" ), QIcon( ":/Icons/Icons/recentsprojects.png" ) );
		action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ), QString::null, tr( "Clear the recents projects list" ) );
		action( "mRecents/aSeparator1" );
	endGroup();
	menu( "mBuild", tr( "&Build" ) );
	beginGroup( "mBuild" );
		menu( "mBuild", tr( "&Build" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		action( "mBuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildbuild.png" ), QString::null, tr( "Build current" ) );
		action( "mBuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildbuild.png" ), QString::null, tr( "Build all" ) );
		menu( "mRebuild", tr( "&Rebuild" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		action( "mRebuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildrebuild.png" ), QString::null, tr( "Rebuild current" ) );
		action( "mRebuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildrebuild.png" ), QString::null, tr( "Rebuild all" ) );
		action( "aSeparator1" );
		action( "aStop", tr( "&Stop" ), QIcon( ":/Icons/Icons/buildstop.png" ), QString::null, tr( "Stop current" ) );
		action( "aSeparator2" );
		menu( "mClean", tr( "&Clean" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		action( "mClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildclean.png" ), QString::null, tr( "Clean current" ) );
		action( "mClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildclean.png" ), QString::null, tr( "Clean all" ) );
		menu( "mDistClean", tr( "&Dist Clean" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "mDistClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/builddistclean.png" ), QString::null, tr( "Dist clean current" ) );
		action( "mDistClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/builddistclean.png" ), QString::null, tr( "Dist clean all" ) );
		action( "aSeparator3" );
		action( "aExecute", tr( "&Execute" ), QIcon( ":/Icons/Icons/buildexecute.png" ), QString::null, tr( "Execute current" ) );
		action( "aExecuteWithParameters", tr( "Execute with &parameters..." ), QIcon( ":/Icons/Icons/buildexecute.png" ), QString::null, tr( "Execute current with parameters..." ) );
		action( "aSeparator4" );
		action( "aBuildExecute", tr( "Build, Execu&te" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Build and execute current" ) );
		action( "aDistCleanBuildExecute", tr( "Dist Clean, Build, E&xecute" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Dist clean, build and execute current" ) );
	endGroup();
	/*
	menu( "mBuild", tr( "&Build" ) );
	beginGroup( "mBuild" );
		action( "aBuildCurrent", tr( "&Build Current" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		connect ( action ("aBuildCurrent"),SIGNAL (triggered()),Studio::self(),SIGNAL (buildCurrent()));
		action( "aBuildExecute", tr( "Build and &Execute" ), QIcon( ":/Icons/Icons/buildexecute.png" ) );
		connect ( action ("aBuildExecute"),SIGNAL (triggered()),Studio::self(),SIGNAL (buildExecute()));
		action( "aStop", tr( "&Stop" ), QIcon( ":/Icons/Icons/buildstop.png" ) );
		connect ( action ("aStop"),SIGNAL (triggered()),Studio::self(),SIGNAL (stopExecuting()));
		action( "aSeparator1" );
		menu( "mBuild", tr( "&Build" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		action( "mBuild/aAll", tr( "&Build All" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		connect ( action ("mBuild/aAll"),SIGNAL (triggered()),Studio::self(),SIGNAL (buildAll()));
		action( "mBuild/aRebCurrent", tr( "&Rebuild Current" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		connect ( action ("mBuild/aRebCurrent"),SIGNAL (triggered()),Studio::self(),SIGNAL (reBuildCurrent()));
		action( "mBuild/aRebAll", tr( "&Rebuild All" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		connect ( action ("mBuild/aRebAll"),SIGNAL (triggered()),Studio::self(),SIGNAL (reBuildAll()));
		menu( "mClean", tr( "&Clean" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		action( "mClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		connect ( action ("mClean/aCurrent"),SIGNAL (triggered()),Studio::self(),SIGNAL (cleanCurrent()));
		action( "mClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		connect ( action ("mClean/aAll"),SIGNAL (triggered()),Studio::self(),SIGNAL (cleanAll()));
		menu( "mDistClean", tr( "&Dist Clean" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "mDistClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		connect ( action ("mDistClean/aCurrent"),SIGNAL (triggered()),Studio::self(),SIGNAL (distCleanCurrent()));
		action( "mDistClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		connect ( action ("mDistClean/aAll"),SIGNAL (triggered()),Studio::self(),SIGNAL (distCleanAll()));
		action( "aSeparator2" );
		action( "aExecute", tr( "&Execute" ), QIcon( ":/Icons/Icons/buildexecute.png" ) );
		connect ( action ("aExecute"),SIGNAL (triggered()),Studio::self(),SIGNAL (Execute()));
		action( "aExecuteWithParams", tr( "&Execute with params" ), QIcon( ":/Icons/Icons/buildexecute.png" ) );
		connect ( action ("aExecuteWithParams"),SIGNAL (triggered()),Studio::self(),SIGNAL (executeWithParametres()));
		action( "aSeparator3" );
		action( "aSeparator4" );
		action( "alupdate", tr( "l&update" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
		action( "alrelease", tr( "l&release" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
	endGroup();
	*/
	menu( "mTools", tr( "&Tools" ) );
	beginGroup( "mTools" );
		action( "aEdit", tr( "&Edit..." ), QIcon( ":/Icons/Icons/toolsedit.png" ), QString::null, tr( "Edit tools..." ) );
		action( "aSeparator1" );
	endGroup();
	menu( "mPlugins", tr( "Plu&gins" ) );
	beginGroup( "mPlugins" );
		action( "aManage", tr( "&Manage..." ), QIcon( ":/Icons/Icons/toolsedit.png" ), QString::null, tr( "Manage plugins..." ) );
		action( "aSeparator1" );
	endGroup();
	menu( "mHelp", tr( "&Help" ) );
	beginGroup( "mHelp" );
		action( "aManual", tr( "&Manual" ), QIcon( ":/Icons/Icons/helpassistant.png" ), QString::null, tr( "Manual" ) );
		action( "aSeparator1" );
		action( "aAbout", tr( "&About..." ), QIcon( ":/Icons/Icons/icon.png" ), QString::null, tr( "About application..." ) );
		action( "aAboutQt", tr( "About &Qt..." ), QIcon( ":/Icons/Icons/helpqt.png" ), QString::null, tr( "About Qt..." ) );
#ifdef __COVERAGESCANNER__
		action( "aTestReport", tr( "&Test Report" ), QIcon( ) , tr( "Pause" ), tr( "Coverage Meter Test Report..." ) );
#endif
	endGroup();
}
