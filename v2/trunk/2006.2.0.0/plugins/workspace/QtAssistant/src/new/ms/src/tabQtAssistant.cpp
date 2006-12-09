/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : vendredi 11 août 2006 - 17h35
 * AUTHOR       : Azevedo Filipe aka Nox PasNox ( pasnox at gmail dot com )
 * FILENAME     : tabQtAssistant.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "tabQtAssistant.h"
#include "assistantSearch.h"
#include "Settings.h"
//
#include <QVBoxLayout>
#include <QSplitter>
#include <QIcon>
#include <QAction>
#include <QComboBox>
#include <QToolBar>
#include <QListWidget>
#include <QLibraryInfo>
#include <QTextBrowser>
#include <QTextCursor>
#include <QClipboard>
//
QPointer<tabQtAssistant> tabQtAssistant::mSelf = 0L;
//
tabQtAssistant* tabQtAssistant::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new tabQtAssistant( p );
	return mSelf;
}
//
tabQtAssistant::tabQtAssistant( QWidget* p )
	: abstractTab( p ), uHome( "index.html" ), tbActions( new QToolBar ), cbZoom( new QComboBox ),
	lwBookmarks( new QListWidget ), tbSource( new QTextBrowser ),
	mSearch( new assistantSearch( tbSource ) ), mInit( false )
{
	setType( ttQtAssistant );
	setWindowIcon( QIcon( ":/Icons/Icons/helpkeyword.png" ) );
	// layout
	QVBoxLayout* l = new QVBoxLayout( this );
	l->setMargin( 0 );
	l->setSpacing( 0 );
	// splitter
	QSplitter* s = new QSplitter;
	QSizePolicy sp = s->sizePolicy();
	sp.setVerticalStretch( 1 );
	s->setSizePolicy( sp );
	// fill tool bar
	aAdd = new QAction( QIcon( ":/Icons/Icons/editadd.png" ), tr( "&Add" ), tbActions );
	aAdd->setShortcut( QKeySequence( "Ctrl+Insert" ) );
	tbActions->addAction( aAdd );
	connect( aAdd, SIGNAL ( triggered() ), this, SLOT( on_aAdd_triggered() ) );
	//
	aRemove = new QAction( QIcon( ":/Icons/Icons/editremove.png" ), tr( "&Remove" ), tbActions );
	aRemove->setShortcut( QKeySequence( "Ctrl+Delete" ) );
	tbActions->addAction( aRemove );
	connect( aRemove, SIGNAL ( triggered() ), this, SLOT( on_aRemove_triggered() ) );
	//
	tbActions->addSeparator();
	//
	aHome = new QAction( QIcon( ":/Icons/Icons/helphome.png" ), tr( "&Home" ), tbActions );
	aHome->setShortcut( QKeySequence( "Alt+Home" ) );
	tbActions->addAction( aHome );
	connect( aHome, SIGNAL( triggered() ), this, SLOT( on_aHome_triggered() ) );
	//
	aBack = new QAction( QIcon( ":/Icons/Icons/helpprev.png" ), tr( "&Back" ), tbActions );
	aBack->setShortcut( QKeySequence( "Alt+Left" ) );
	tbActions->addAction( aBack );
	connect( tbSource, SIGNAL( backwardAvailable( bool ) ), aBack, SLOT( setEnabled( bool ) ) );
	connect( aBack, SIGNAL( triggered() ), tbSource, SLOT( backward() ) );
	//
	aNext = new QAction( QIcon( ":/Icons/Icons/helpnext.png" ), tr( "&Next" ), tbActions );
	aNext->setShortcut( QKeySequence( "Alt+Right" ) );
	tbActions->addAction( aNext );
	connect( tbSource, SIGNAL( forwardAvailable( bool ) ), aNext, SLOT( setEnabled( bool ) ) );
	connect( aNext, SIGNAL ( triggered() ), tbSource, SLOT( forward() ) );
	//
	tbActions->addSeparator();
	//
	aZoomIn = new QAction( QIcon( ":/Icons/Icons/zoomin.png" ), tr( "&Zoom in" ), tbActions );
	aZoomIn->setShortcut( QKeySequence( "Ctrl++" ) );
	tbActions->addAction( aZoomIn );
	connect( aZoomIn, SIGNAL( triggered() ), tbSource, SLOT( zoomIn() ) );
	//
	aZoomOut = new QAction( QIcon( ":/Icons/Icons/zoomout.png" ), tr( "&Zoom out" ), tbActions );
	aZoomOut->setShortcut( QKeySequence( "Ctrl+-" ) );
	tbActions->addAction( aZoomOut );
	connect( aZoomOut, SIGNAL( triggered() ), tbSource, SLOT( zoomOut() ) );
	//
	cbZoom->addItem( tr( "Smaller text" ), -5 );
	cbZoom->addItem( tr( "Bigger text" ), 5 );
	tbActions->addWidget( cbZoom );
	connect( cbZoom, SIGNAL( currentIndexChanged( int ) ), this, SLOT( on_cbZoom_currentIndexChanged( int ) ) );
	//
	QListWidgetItem* i = new QListWidgetItem( tr( "Bookmarks" ), lwBookmarks );
	i->setFlags( 0 );
	lwBookmarks->setContextMenuPolicy( Qt::ActionsContextMenu );
	lwBookmarks->addAction( aAdd );
	lwBookmarks->addAction( aRemove );
	connect( lwBookmarks, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), this, SLOT( on_lwBookmarks_itemDoubleClicked( QListWidgetItem* ) ) );
	tbSource->setContextMenuPolicy( Qt::ActionsContextMenu );
	//
	tbSource->setSearchPaths( QStringList() << QLibraryInfo::location( QLibraryInfo::DocumentationPath ) + QString( "/html" ) );
	//
	aSearch = new QAction( QIcon( ":/Icons/Icons/editreplace.png" ), tr( "Search" ), tbSource );
	tbSource->addAction( aSearch );
	connect( aSearch, SIGNAL( triggered() ), this, SLOT( search() ) );
	//
	aSearchPrevious = new QAction( QIcon( ":/Icons/Icons/helpprev.png" ), tr( "Search Previous" ), tbSource );
	tbSource->addAction( aSearchPrevious );
	connect( aSearchPrevious, SIGNAL( triggered() ), mSearch->tbPrevious, SIGNAL( clicked() ) );
	//
	aSearchNext = new QAction( QIcon( ":/Icons/Icons/helpnext.png" ), tr( "Search Next" ), tbSource );
	tbSource->addAction( aSearchNext );
	connect( aSearchNext, SIGNAL( triggered() ), mSearch->tbNext, SIGNAL( clicked() ) );
	// fill layout & splitter
	l->addWidget( tbActions );
	s->addWidget( lwBookmarks );
	s->addWidget( tbSource );
	s->setStretchFactor( 1, 1 );
	l->addWidget( s );
	l->addWidget( mSearch );
	// connections
	connect( tbSource, SIGNAL( copyAvailable ( bool ) ), this, SIGNAL( tabCopyAvailable( bool ) ) );
	connect( tbSource, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( setUrl( const QUrl& ) ) );
	// hide search widget
	mSearch->hide();
}
//
void tabQtAssistant::showEvent( QShowEvent* e )
{
	if ( !mInit )
	{
		mInit = true;
		foreach ( QString s, Settings::current()->value( "Bookmarks/QtAssistant" ).toStringList() )
			lwBookmarks->addItem( s );
		openUrl( uHome );
	}
	abstractTab::showEvent( e );
}
//
void tabQtAssistant::closeEvent( QCloseEvent* e )
{
	QStringList l;
	for ( int i = 1; i < lwBookmarks->count(); i++ )
		l << lwBookmarks->item( i )->text();
	Settings::current()->setValue( "Bookmarks/QtAssistant", l );
	abstractTab::closeEvent( e );
}
//
QString tabQtAssistant::fileFilter() const
{
	return tr( "HTML Files (*.html *.htm)" );
}
//
bool tabQtAssistant::isCopyAvailable() const
{
	return tbSource->textCursor().hasSelection();
};
//
bool tabQtAssistant::isSearchAvailable() const
{
	return true;
};
//
QUrl tabQtAssistant::homePage() const
{
	return uHome;
}
//
void tabQtAssistant::copy()
{
	tbSource->copy();
}
//
void tabQtAssistant::search()
{
	if ( !mSearch->isVisible() )
		mSearch->show();
	mSearch->leSearch->setFocus();
	mSearch->leSearch->selectAll();
}
//
void tabQtAssistant::openUrl( const QUrl& s )
{
	tbSource->setSource( s );
}
//
void tabQtAssistant::on_aAdd_triggered()
{
	QString s = tbSource->source().toString();
	if ( s.isEmpty() )
		return;
	if ( !lwBookmarks->findItems( s, Qt::MatchExactly ).count() )
		lwBookmarks->addItem( s );
}
//
void tabQtAssistant::on_aRemove_triggered()
{
	if ( !lwBookmarks->currentItem() || lwBookmarks->currentRow() == 0 )
		return;
	delete lwBookmarks->currentItem();
}
//
void tabQtAssistant::on_lwBookmarks_itemDoubleClicked( QListWidgetItem* i )
{
	if ( i && !lwBookmarks->currentRow() == 0 )
		tbSource->setSource( QUrl( i->text() ) );
}
//
void tabQtAssistant::on_aHome_triggered()
{
	tbSource->setSource( uHome );
}
//
void tabQtAssistant::on_cbZoom_currentIndexChanged( int i )
{
	tbSource->zoomIn( 0 );
	tbSource->zoomIn( cbZoom->itemData( i ).toInt() );
}

