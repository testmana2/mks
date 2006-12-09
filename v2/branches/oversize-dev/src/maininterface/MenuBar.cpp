/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 01h15
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : MenuBar.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class is the monkey studio menuBar created using the pMenuBar class
 ********************************************************************************************************/
#include "MenuBar.h"
#include "Settings.h"
#include "UIAbout.h"
//
#include <QFileInfo>
#include <QActionGroup>
#include <QStyleFactory>
#include <QApplication>
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
}
//
void MenuBar::initialize()
{
	menu( "mFile", tr( "&File" ) );
	beginGroup( "mFile" );
		action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/fileopen.png" ), tr( "Ctrl+O" ), tr( "Open a file" ) );
		menu( "mRecents", tr( "&Recents" ) );
		action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ) );
		action( "mRecents/aSeparator1" );
		action( "aSeparator1" );
		menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/filesave.png" ) );
		action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/filesave.png" ), tr( "Ctrl+S" ) );
		action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filesaveall.png" ), tr( "Ctrl+Shift+S" ) );
		menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/fileclose.png" ) );
		action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/fileclose.png" ), tr( "Ctrl+W" ) );
		action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/filecloseall.png" ), tr( "Ctrl+Shift+W" ) );
		action( "aSeparator2" );
		action( "aSaveAsTemplate", tr( "Save As &Template" ), QIcon( ":/Icons/Icons/filesaveastemplate.png" ), tr( "Ctrl+T" ) );
		action( "aSeparator3" );
		action( "aQuickPrint", tr( "&Quick Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+Q" ) );
		action( "aPrint", tr( "&Print" ), QIcon( ":/Icons/Icons/fileprint.png" ), tr( "Ctrl+P" ) );
		action( "aSeparator4" );
		action( "aExit", tr( "&Exit" ), QIcon( ":/Icons/Icons/exit.png" ), tr( "Ctrl+E" ) );
	endGroup();
	menu( "mEdit", tr( "&Edit" ) );
	beginGroup( "mEdit" );
		action( "aSettings", tr( "Settings..." ), QIcon( ":/Icons/Icons/editsettings.png" ) );
		/*
		action( "aSeparator1" );
		action( "aUndo", tr( "&Undo" ), QIcon( ":/Icons/Icons/editundo.png" ) );
		action( "aRedo", tr( "&Redo" ), QIcon( ":/Icons/Icons/editredo.png" ) );
		action( "aSeparator2" );
		action( "aCopy", tr( "&Copy" ), QIcon( ":/Icons/Icons/editcopy.png" ) );
		action( "aCut", tr( "Cu&t" ), QIcon( ":/Icons/Icons/editcut.png" ) );
		action( "aPaste", tr( "&Paste" ), QIcon( ":/Icons/Icons/editpaste.png" ) );
		action( "aSeparator3" );
		action( "aSearchReplace", tr( "&Search - Replace..." ), QIcon( ":/Icons/Icons/editreplace.png" ), tr( "Ctrl+F" ) );
		action( "aGoTo", tr( "&Go To..." ), QIcon( ":/Icons/Icons/editgoto.png" ), tr( "Ctrl+G" ) );
		*/
	endGroup();
	menu( "mView", tr( "&View" ) );
	beginGroup( "mView" );
		menu( "mContainer", tr( "&Container" ), QIcon( ":/Icons/Icons/containers.png" ) );
		menu( "mStyle", tr( "&Style" ), QIcon( ":/Icons/Icons/viewstyle.png" ) );
		initializeStyles();
		//
	endGroup();
	menu( "mProject", tr( "&Project" ) );
	beginGroup( "mProject" );
		action( "aNew", tr( "&New" ), QIcon( ":/Icons/Icons/projectnew.png" ), tr( "Ctrl+Shift+N" ) );
		action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/projectopen.png" ), tr( "Ctrl+Shift+O" ) );
		menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectsaveall.png" ) );
		menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		action( "mClose/aClose", tr( "&Current" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		action( "mClose/aCloseAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectcloseall.png" ) );
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
		menu( "mRecents", tr( "&Recents" ) );
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
		menu( "mClean", tr( "&Clean" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		action( "mClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		action( "mClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		menu( "mDistClean", tr( "&Dist Clean" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "mDistClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "mDistClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		action( "aSeparator2" );
		action( "aExecute", tr( "&Execute..." ), QIcon( ":/Icons/Icons/buildexecute.png" ) );
		action( "aSeparator3" );
		action( "aStop", tr( "&Stop" ), QIcon( ":/Icons/Icons/buildstop.png" ) );
		action( "aSeparator4" );
		action( "alupdate", tr( "l&update" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
		action( "alrelease", tr( "l&release" ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
		action( "aDistCleanBuildExecute", tr( "Dist Clean, Build, E&xecute..." ), QIcon( ":/Icons/Icons/buildmisc.png" ) );
	endGroup();
	menu( "mTools", tr( "&Tools" ) );
	beginGroup( "mTools" );
		action( "aEdit", tr( "&Edit..." ), QIcon( ":/Icons/Icons/toolsedit.png" ) );
		action( "aSeparator1" );
	endGroup();
	menu( "mHelp", tr( "&Help" ) );
	beginGroup( "mHelp" );
		action( "aManual", tr( "&Manual" ), QIcon( ":/Icons/Icons/helpassistant.png" ) );
		action( "aQtDoc", tr( "Qt &Documentation" ), QIcon( ":/Icons/Icons/helpkeyword.png" ) );
		action( "aSeparator1" );
		action( "aAbout", tr( "&About..." ), QIcon( ":/Icons/Icons/helpsplashscreen.png" ) );
		action( "aAboutQt", tr( "About &Qt..." ), QIcon( ":/Icons/Icons/helpqt.png" ) );
	endGroup();
	// connections
	connect( menu( "mFile/mRecents" ), SIGNAL( triggered( QAction* ) ), this, SLOT( recentFiles_triggered( QAction* ) ) );
	connect( menu( "mProject/mRecents" ), SIGNAL( triggered( QAction* ) ), this, SLOT( recentProjects_triggered( QAction* ) ) );
	connect( action( "mHelp/aAbout" ), SIGNAL( triggered() ), this, SLOT( aboutApplication_triggered() ) );
	connect( action( "mHelp/aAboutQt" ), SIGNAL( triggered() ), this, SLOT( aboutQt_triggered() ) );
}
//
void MenuBar::initializeStyles()
{
	// create action for styles
	QActionGroup* agStyles = new QActionGroup( menu( "mStyle" ) );
	foreach ( QString s, QStyleFactory::keys() )
	{
		QAction* a = agStyles->addAction( s );
		a->setCheckable( true );
		if ( Settings::current()->value( "Style" ).toString() == s )
			a->setChecked( true );
	}
	menu( "mStyle" )->addActions( agStyles->actions() );
	connect( agStyles, SIGNAL( triggered( QAction* ) ), this, SLOT( agStyles_triggered( QAction* ) ) );
}
//
void MenuBar::recentFiles_triggered( QAction* a )
{
	if ( a->text() != tr( "Clear" ) )
		emit openFileRequested( a->data().toString() );
	else if ( a->text() == tr( "Clear" ) )
	{
		Settings::current()->setValue( "Recents/Files", QStringList() );
		updateRecentFiles();
	}
}
//
void MenuBar::updateRecentFiles()
{
	mRecentFiles.clear();
	QStringList l = Settings::current()->value( "Recents/Files" ).toStringList();
	bool b = false;
	QAction* a;
	for ( int i = 0; i < Settings::current()->maxRecentFiles(); i++ )
	{
		if ( i < l.count() )
		{
			QFileInfo f( l.at( i ).simplified() );
			if ( f.exists() )
			{
				QString s = QString( "&%1 %2" ).arg( i +1 ).arg( f.fileName() );
				a = new QAction( s, this );
				a->setData( l.at( i ) );
				a->setStatusTip( l.at( i ) );
				mRecentFiles.append( a );
				menu( "mFile/mRecents" )->addAction( a );
				b = true;
			}
		}
	}
	menu( "mFile/mRecents" )->setEnabled( b );
}
//
void MenuBar::addRecentFile( const QString& s )
{
	QStringList f = Settings::current()->value( "Recents/Files" ).toStringList();
	f.removeAll( s );
	f.prepend( s );
	while ( f.size() > Settings::current()->maxRecentFiles() )
		f.removeLast();
	Settings::current()->setValue( "Recents/Files", f );
	updateRecentFiles();
}
//
void MenuBar::agStyles_triggered( QAction* a )
{	
	qApp->setStyle( a->text() );
	Settings::current()->setValue( "Style", a->text() );
}
//
void MenuBar::recentProjects_triggered( QAction* a )
{
	if ( a->text() != tr( "Clear" ) )
		emit openProjectRequested( a->data().toString() );
	else if ( a->text() == tr( "Clear" ) )
	{
		Settings::current()->setValue( "Recents/Projects", QStringList() );
		updateRecentProjects();
	}
}
//
void MenuBar::updateRecentProjects()
{
	mRecentProjects.clear();
	QStringList l = Settings::current()->value( "Recents/Projects" ).toStringList();
	bool b = false;
	QAction* a;
	for ( int i = 0; i < Settings::current()->maxRecentProjects(); i++ )
	{
		if ( i < l.count() )
		{
			QFileInfo f( l.at( i ).simplified() );
			if ( f.exists() )
			{
				QString s = QString( "&%1 %2" ).arg( i +1 ).arg( f.fileName() );
				a = new QAction( s, this );
				a->setData( l.at( i ) );
				a->setStatusTip( l.at( i ) );
				mRecentProjects.append( a );
				menu( "mProject/mRecents" )->addAction( a );
				b = true;
			}
		}
	}
	menu( "mProject/mRecents" )->setEnabled( b );
}
//
void MenuBar::addRecentProject( const QString& s )
{
	QStringList f = Settings::current()->value( "Recents/Projects" ).toStringList();
	f.removeAll( s );
	f.prepend( s );
	while ( f.size() > Settings::current()->maxRecentProjects() )
		f.removeLast();
	Settings::current()->setValue( "Recents/Projects", f );
	updateRecentProjects();
}
//
void MenuBar::aboutApplication_triggered()
{
	UIAbout::self( qApp->activeWindow() )->exec();
}
//
void MenuBar::aboutQt_triggered()
{
	qApp->aboutQt();
}
