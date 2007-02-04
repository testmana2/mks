/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 15h10
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "UIMain.h"
#include "Settings.h"
#include "TabToolBar.h"
#include "MenuBar.h"
#include "Workspace.h"
#include "StatusBar.h"
#include "UIAbout.h"
#include "UITestReport.h"
//
#include <QStyleFactory>
//
#include <QCloseEvent>
//
QPointer<UIMain> UIMain::mSelf = 0L;
//
UIMain* UIMain::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIMain( p );
	return mSelf;
}
//
UIMain::UIMain( QWidget* p )
	: QMainWindow( p ), mInit( false )
{
	setupUi( this );
	initGui();
	initConnections();
}
//
void UIMain::showEvent( QShowEvent* e )
{
	QMainWindow::showEvent( e );
	restoreState();
}
//
void UIMain::closeEvent( QCloseEvent* e )
{
	saveState();
	QMainWindow::closeEvent( e );
}
//
void UIMain::restoreState()
{
	Settings::current()->restoreState( this );
	TabToolBar::self()->restoreState( Settings::current() );
}
//
void UIMain::saveState()
{
	Settings::current()->saveState( this );
	TabToolBar::self()->saveState( Settings::current() );
}
//
void UIMain::initGui()
{
	// set icon size for toolbar
	setIconSize( QSize( 16, 16 ) );
	// set menu bar
	setMenuBar( MenuBar::self() );
	// create action for styles
	agStyles = new QActionGroup( MenuBar::self()->menu( "mView/mStyle" ) );
	foreach ( QString s, QStyleFactory::keys() )
	{
		QAction* a = agStyles->addAction( s );
		a->setCheckable( true );
		if ( Settings::current()->value( "Style" ).toString() == s )
			a->setChecked( true );
	}
	MenuBar::self()->menu( "mView/mStyle" )->addActions( agStyles->actions() );
	TabToolBar::self( this );
	setCentralWidget( Workspace::self() );
	setStatusBar( StatusBar::self() );
}
//
void UIMain::initConnections()
{
	connect( agStyles, SIGNAL( triggered( QAction* ) ), this, SLOT( agStyles_triggered( QAction* ) ) );
	connect( MenuBar::self()->action( "mHelp/aAbout" ), SIGNAL( triggered() ), this, SLOT( aboutApplication_triggered() ) );
	connect( MenuBar::self()->action( "mHelp/aAboutQt" ), SIGNAL( triggered() ), this, SLOT( aboutQt_triggered() ) );
#ifdef __COVERAGESCANNER__
	connect( MenuBar::self()->action( "mHelp/aTestReport" ), SIGNAL( triggered() ), this, SLOT( testReport_triggered() ) );
#endif

}
//
void UIMain::agStyles_triggered( QAction* a )
{	
	qApp->setStyle( a->text() );
	Settings::current()->setValue( "Style", a->text() );
}
//
void UIMain::aboutApplication_triggered()
{
	UIAbout::self( this )->exec();
}
//
void UIMain::testReport_triggered()
{
  UITestReport::self()->exec();
}
//
void UIMain::aboutQt_triggered()
{
	qApp->aboutQt();
}
