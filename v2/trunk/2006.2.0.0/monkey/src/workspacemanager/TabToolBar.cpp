#include "TabToolBar.h"
#include "MenuBar.h"
#include "qscintillaSearch.h"
//
QPointer<TabToolBar> TabToolBar::mSelf = 0L;
//
TabToolBar* TabToolBar::self( QMainWindow* m )
{
	if ( !mSelf )
	{
		Q_ASSERT( m != 0 );
		mSelf = new TabToolBar( m );
	}
	return mSelf;
}
//
TabToolBar::TabToolBar( QMainWindow* p )
	: pTabToolBarManager( p )
{
	initialize();
}
//
void TabToolBar::initialize()
{
	QAction* a;
	// recents
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->menu( "mFile/mRecents" )->menuAction() );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->menu( "mProject/mRecents" )->menuAction() );
	a = new QAction( this );
	a->setSeparator( true );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), a );
	// file action
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mFile/aOpen" ) );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->menu( "mFile/mSave" )->menuAction() );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->menu( "mFile/mClose" )->menuAction() );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mFile/aQuickPrint" ) );
	a = new QAction( this );
	a->setSeparator( true );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), a );
	// edit action
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aUndo" ) );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aRedo" ) );
	a = new QAction( this );
	a->setSeparator( true );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), a );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aCut" ) );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aCopy" ) );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aPaste" ) );
	a = new QAction( this );
	a->setSeparator( true );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), a );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aSearchReplace" ) );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mEdit/aGoTo" ) );
	a = new QAction( this );
	a->setSeparator( true );
	bar( Top )->insertAction( bar( Top )->tabBarAction(), a );
	// help action
	bar( Top )->insertAction( bar( Top )->tabBarAction(), MenuBar::self()->action( "mHelp/aAbout" ) );
	// dock qscintillaSearch
	bar( Bottom )->appendTab( qscintillaSearch::self(), QPixmap( ":/Icons/Icons/tabsearch.png" ), tr( "Search - Replace" ) );
}
