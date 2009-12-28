#include "SearchResultsDock.h"
#include "SearchResultsModel.h"
#include "SearchThread.h"

#include <pIconManager.h>
#include <pMonkeyStudio.h>

#include <QHBoxLayout>
#include <QTreeView>

SearchResultsDock::SearchResultsDock( SearchThread* searchThread, QWidget* parent )
	: pDockWidget( parent )
{
	Q_ASSERT( searchThread );
	
	setObjectName( metaObject()->className() );
	setWindowTitle( tr( "Search Results" ) );
	setWindowIcon( pIconManager::icon( "SearchAndReplaceV2.png", ":/icons" ) );
	
	QWidget* widget = new QWidget( this );
	mModel = new SearchResultsModel( this );
	mView = new QTreeView( this );
	mView->setHeaderHidden( true );
	mView->setUniformRowHeights( true );
	mView->setModel( mModel );
	mLayout = new QHBoxLayout( widget );
	mLayout->setMargin( 5 );
	mLayout->setSpacing( 5 );
	mLayout->addWidget( mView );
	
	setWidget( widget );
	
	// mac
	pMonkeyStudio::showMacFocusRect( this, false, true );
	pMonkeyStudio::setMacSmallSize( this, true, true );
	
	connect( searchThread, SIGNAL( reset() ), mModel, SLOT( thread_reset() ) );
	connect( searchThread, SIGNAL( resultsAvailable( const QString&, const SearchResultsModel::ResultList& ) ), mModel, SLOT( thread_resultsAvailable( const QString&, const SearchResultsModel::ResultList& ) ) );
}

SearchResultsDock::~SearchResultsDock()
{
}
