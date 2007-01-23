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
		action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ) );
		action( "mRecents/aSeparator1" );
		action( "aSeparator1" );
		menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/filesave.png" ) );
		action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/filesave.png" ), tr( "Ctrl+S" ) )->setEnabled( false );
		action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filesaveall.png" ), tr( "Ctrl+Shift+S" ) )->setEnabled( false );
		menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/fileclose.png" ) );
		action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/fileclose.png" ), tr( "Ctrl+W" ) )->setEnabled( false );
		action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filecloseall.png" ), tr( "Ctrl+Shift+W" ) )->setEnabled( false );
		action( "aSeparator2" );
		action( "aSaveAsTemplate", tr( "Save As &Template" ), QIcon( ":/Icons/Icons/filesaveastemplate.png" ), tr( "Ctrl+T" ) )->setEnabled( false );
		action( "aSeparator3" );
		action( "aQuickPrint", tr( "&Quick Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+Q" ) )->setEnabled( false );
		action( "aPrint", tr( "&Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+P" ) )->setEnabled( false );
		action( "aSeparator4" );
		action( "aExit", tr( "&Exit" ), QIcon( ":/Icons/Icons/exit.png" ), tr( "Ctrl+E" ) );
	endGroup();
	menu( "mEdit", tr( "&Edit" ) );
	beginGroup( "mEdit" );
		action( "aSettings", tr( "Settings..." ), QIcon( ":/Icons/Icons/editsettings.png" ) );
		action( "aSeparator1" );
		action( "aUndo", tr( "&Undo" ), QIcon( ":/Icons/Icons/editundo.png" ), tr( "Ctrl+Z" ) )->setEnabled( false );
		action( "aRedo", tr( "&Redo" ), QIcon( ":/Icons/Icons/editredo.png" ), tr( "Ctrl+Y" ) )->setEnabled( false );
		action( "aSeparator2" );
		action( "aCopy", tr( "&Copy" ), QIcon( ":/Icons/Icons/editcopy.png" ), tr( "Ctrl+C" ) )->setEnabled( false );
		action( "aCut", tr( "Cu&t" ), QIcon( ":/Icons/Icons/editcut.png" ), tr( "Ctrl+X" ) )->setEnabled( false );
		action( "aPaste", tr( "&Paste" ), QIcon( ":/Icons/Icons/editpaste.png" ), tr( "Ctrl+V" ) )->setEnabled( false );
		action( "aSeparator3" );
		action( "aSearchReplace", tr( "&Search - Replace..." ), QIcon( ":/Icons/Icons/editreplace.png" ), tr( "Ctrl+F" ) )->setEnabled( false );
		action( "aGoTo", tr( "&Go To..." ), QIcon( ":/Icons/Icons/editgoto.png" ), tr( "Ctrl+G" ) )->setEnabled( false );
	endGroup();
	menu( "mView", tr( "&View" ) );
	beginGroup( "mView" );
		menu( "mStyle", tr( "&Style" ), QIcon( ":/Icons/Icons/viewstyle.png" ) );
		action( "aSeparator1" );
		action( "aProjectsList", tr( "Show/Hide the projects list" ), QIcon( ":/Icons/Icons/helpgreettings.png" ) )->setCheckable( true );
		action( "aComplexProject", tr( "Show/Hide the complex project" ), QIcon( ":/Icons/Icons/helptesters.png" ) )->setCheckable( true );
		action( "aSeparator2" );
		action( "aNext", tr( "&Next tab" ), QIcon( ":/Icons/Icons/buttonnext.png" ), tr( "Alt+Right" ) )->setEnabled( false );
		action( "aPrevious", tr( "&Previous tab" ), QIcon( ":/Icons/Icons/buttonprevious.png" ), tr( "Alt+Left" ) )->setEnabled( false );
	endGroup();
	menu( "mProject", tr( "&Project" ) );
	beginGroup( "mProject" );
		action( "aNew", tr( "&New" ), QIcon( ":/Icons/Icons/projectnew.png" ), tr( "Ctrl+Shift+N" ) );
		action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/projectopen.png" ), tr( "Ctrl+Shift+O" ) );
		menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectsaveall.png" ) );
		menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectcloseall.png" ) );
		menu( "mShow", tr( "S&how" ), QIcon( ":/Icons/Icons/projectshow.png" ) );
		action( "mShow/aSource", tr( "&Source" ), QIcon( ":/Icons/Icons/projectshowfile.png" ) );
		action( "mShow/aToDo", tr( "&ToDo" ), QIcon( ":/Icons/Icons/projectshowfile.png" ) );
		action( "mShow/aChanges", tr( "&Changes" ), QIcon( ":/Icons/Icons/projectshowfile.png" ) );
		menu( "mAdd", tr( "&Add" ), QIcon( ":/Icons/Icons/projectadd.png" ) );
		action( "mAdd/aNewForm", tr( "New F&orm..." ), QIcon( ":/Icons/Icons/projectaddform.png" ) );
		action( "mAdd/aNewFiles", tr( "New &Files..." ), QIcon( ":/Icons/Icons/projectaddfile.png" ) );
		action( "mAdd/aNewTemplates", tr( "New &Templates" ), QIcon( ":/Icons/Icons/projectaddtemplate.png" ) );
		action( "mAdd/aSeparator1" );
		action( "mAdd/aExistingFiles", tr( "&Existing Files..." ), QIcon( ":/Icons/Icons/projectaddfile.png" ) );
		action( "aSeparator2" );
		action( "aSettings", tr( "Set&tings..." ), QIcon( ":/Icons/Icons/projectsettings.png" ) );
		action( "aSeparator3" );
		menu( "mRecents", tr( "&Recents" ), QIcon( ":/Icons/Icons/recentsprojects.png" ) );
		action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ) );
		action( "mRecents/aSeparator1" );
	endGroup();
	menu( "mBuild", tr( "&Build" ) );
	beginGroup( "mBuild" );
		menu( "mBuild", tr( "&Build" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		action( "mBuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		action( "mBuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		menu( "mRebuild", tr( "&Rebuild" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		action( "mRebuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		action( "mRebuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		action( "aSeparator1" );
		action( "aStop", tr( "&Stop" ), QIcon( ":/Icons/Icons/buildstop.png" ) );
		action( "aSeparator2" );
		menu( "mClean", tr( "&Clean" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		action( "mClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		action( "mClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		menu( "mDistClean", tr( "&Dist Clean" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "mDistClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "mDistClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "aSeparator3" );
		action( "aExecute", tr( "&Execute" ), QIcon( ":/Icons/Icons/buildexecute.png" ) );
		action( "aExecuteWithParameters", tr( "Execute with &parameters..." ), QIcon( ":/Icons/Icons/buildexecute.png" ) );
		action( "aSeparator4" );
		action( "aDistCleanBuildExecute", tr( "Dist Clean, Build, E&xecute" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
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
		action( "aEdit", tr( "&Edit..." ), QIcon( ":/Icons/Icons/toolsedit.png" ) );
		action( "aSeparator1" );
		action( "alupdate", tr( "l&update" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
		action( "alrelease", tr( "l&release" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
	endGroup();
	menu( "mPlugins", tr( "Plu&gins" ) );
	beginGroup( "mPlugins" );
		action( "aManage", tr( "&Manage..." ), QIcon( ":/Icons/Icons/toolsedit.png" ) );
		action( "aSeparator1" );
	endGroup();
	menu( "mHelp", tr( "&Help" ) );
	beginGroup( "mHelp" );
		action( "aManual", tr( "&Manual" ), QIcon( ":/Icons/Icons/helpassistant.png" ) );
		action( "aQtDoc", tr( "Qt &Documentation" ), QIcon( ":/Icons/Icons/helpkeyword.png" ) );
		action( "aSeparator1" );
		action( "aAbout", tr( "&About..." ), QIcon( ":/Icons/Icons/icon.png" ) );
		action( "aAboutQt", tr( "About &Qt..." ), QIcon( ":/Icons/Icons/helpqt.png" ) );
	endGroup();
}
