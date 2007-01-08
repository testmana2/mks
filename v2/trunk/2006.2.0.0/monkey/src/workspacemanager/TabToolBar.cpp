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
	// recents
	bar( Top )->addAction( MenuBar::self()->menu( "mFile/mRecents" )->menuAction() );
	bar( Top )->addAction( MenuBar::self()->menu( "mProject/mRecents" )->menuAction() );
	bar( Top )->addSeparator();
	// file action
	bar( Top )->addAction( MenuBar::self()->menu( "mFile/mSave" )->menuAction() );
	bar( Top )->addAction( MenuBar::self()->menu( "mFile/mClose" )->menuAction() );
	bar( Top )->addAction( MenuBar::self()->action( "mFile/aQuickPrint" ) );
	bar( Top )->addSeparator();
	// edit action
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aUndo" ) );
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aRedo" ) );
	bar( Top )->addSeparator();
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aCut" ) );
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aCopy" ) );
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aPaste" ) );
	bar( Top )->addSeparator();
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aSearchReplace" ) );
	bar( Top )->addAction( MenuBar::self()->action( "mEdit/aGoTo" ) );
	bar( Top )->addSeparator();
	// help action
	bar( Top )->addAction( MenuBar::self()->action( "mHelp/aAbout" ) );
	// dock qscintillaSearch
	bar( Bottom )->appendTab( qscintillaSearch::self(), QPixmap( ":/Icons/Icons/tabsearch.png" ), tr( "Search - Replace" ) );
}
