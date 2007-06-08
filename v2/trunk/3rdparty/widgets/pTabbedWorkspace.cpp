#include "pTabbedWorkspace.h"
#include "pTabbedWorkspaceRightCorner.h"

#include <QBoxLayout>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QWorkspace>

pTabbedWorkspace::pTabbedWorkspace( QWidget* p, pTabbedWorkspace::TabMode m )
	: QWidget( p )
{
	/*
	QBoxLayout::LeftToRight
	QBoxLayout::RightToLeft
	QBoxLayout::TopToBottom
	QBoxLayout::BottomToTop
	*/

	// tab widget
	mTabLayout = new QBoxLayout( QBoxLayout::LeftToRight );
	mTabLayout->setSpacing( 3 );
	mTabLayout->setMargin( 0 );
	mTabLayout->addWidget( ( mTabBar = new QTabBar ) );
	mTabLayout->addStretch( 100 );

	// document widget
	mStackedLayout = new QStackedLayout;
	mStackedLayout->addWidget( ( mStackedWidget = new QStackedWidget ) );
	mStackedLayout->addWidget( ( mWorkspaceWidget = new QWorkspace ) );
	mWorkspaceWidget->setScrollBarsEnabled( true );

	// main layout
	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	mLayout->setSpacing( 0 );
	mLayout->setMargin( 0 );
	mLayout->addLayout( mTabLayout );
	mLayout->addLayout( mStackedLayout );

	// connections
	connect( mTabBar, SIGNAL( currentChanged( int ) ), this, SLOT( setCurrentIndex( int ) ) );
	connect( mWorkspaceWidget, SIGNAL( windowActivated( QWidget* ) ), this, SLOT( workspaceWidget_windowActivated( QWidget* ) ) );

	// init view
	mTabBar->setDrawBase( false );
	mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );
	setAttribute( Qt::WA_DeleteOnClose );
	setCornerWidget( pTabbedWorkspaceRightCorner::instance( this ) );
	setTabMode( m );
	setDocumentMode( pTabbedWorkspace::dmMaximized );
}

pTabbedWorkspace::~pTabbedWorkspace()
{
	// close all document
	foreach ( TABBED_DOCUMENT* d, mDocuments )
		d->close();

	// delete all unclose document, if user click cancel
	qDeleteAll( mDocuments );

	// delete containers
	delete mStackedWidget;
	delete mWorkspaceWidget;
}

bool pTabbedWorkspace::eventFilter( QObject* o, QEvent* e )
{
	// get document
	TABBED_DOCUMENT* d = qobject_cast<TABBED_DOCUMENT*>( o );

	// get event type
	QEvent::Type t = e->type();

	// if mode is toplevel and event is activate, activate correct window if needed
	if ( mTabMode == tmTopLevel && t == QEvent::WindowActivate )
	{
		if ( d && d != currentDocument() )
			setCurrentDocument( d );
	}

	// remove document from workspace
	else if ( t == QEvent::Close && d )
	{
		emit aboutToRemoveTab( indexOf( d ) );
		removeTab( qobject_cast<TABBED_DOCUMENT*>( o ) );
		o->deleteLater();
	}

	// return default event filter
	return QWidget::eventFilter( o, e );
}

void pTabbedWorkspace::workspaceWidget_windowActivated( QWidget* w )
{
	setCurrentDocument( w );
}

QTabBar* pTabbedWorkspace::tabBar() const
{
	return mTabBar;
}

QTabBar::Shape pTabbedWorkspace::tabShape() const
{
	return mTabBar->shape();
}

pTabbedWorkspace::TabMode pTabbedWorkspace::tabMode() const
{
	return mTabMode;
}

pTabbedWorkspace::DocumentMode pTabbedWorkspace::documentMode() const
{
	return mDocumentMode;
}

void pTabbedWorkspace::setTabShape( QTabBar::Shape s )
{
	switch ( s )
	{
	case QTabBar::RoundedNorth:
	case QTabBar::TriangularNorth:
		mTabLayout->setDirection( QBoxLayout::LeftToRight );
		mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
		mLayout->setDirection( QBoxLayout::TopToBottom );
		break;
	case QTabBar::RoundedSouth:
	case QTabBar::TriangularSouth:
		mTabLayout->setDirection( QBoxLayout::LeftToRight );
		mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
		mLayout->setDirection( QBoxLayout::BottomToTop );
		break;
	case QTabBar::RoundedWest:
	case QTabBar::TriangularWest:
		mTabLayout->setDirection( QBoxLayout::BottomToTop );
		mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding ) );
		mLayout->setDirection( QBoxLayout::LeftToRight );
		break;
	case QTabBar::RoundedEast:
	case QTabBar::TriangularEast:
		mTabLayout->setDirection( QBoxLayout::TopToBottom );
		mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding ) );
		mLayout->setDirection( QBoxLayout::RightToLeft );
		break;
	}

	// update corners
	updateCorners();

	mTabBar->setShape( s );
	emit tabShapeChanged( s );
}

void pTabbedWorkspace::setTabMode( pTabbedWorkspace::TabMode t )
{
	if ( mTabMode != t )
	{
		int i = currentIndex();
		mTabMode = t;
		updateView();
		setCurrentIndex( i );
		emit tabModeChanged( mTabMode );
	}
}

void pTabbedWorkspace::setDocumentMode( pTabbedWorkspace::DocumentMode d )
{
	if ( mTabMode == pTabbedWorkspace::tmMDI && mDocumentMode != d )
	{
		switch( d )
		{
		case dmMaximized:
			foreach ( TABBED_DOCUMENT* d, documents() )
				d->showMaximized();
			break;
		case dmCascade:
			mWorkspaceWidget->cascade();
			break;
		case dmTile:
			mWorkspaceWidget->tile();
			break;
		case dmIcons:
			mWorkspaceWidget->arrangeIcons();
			break;
		case dmMinimizeAll:
			foreach ( TABBED_DOCUMENT* d, documents() )
				d->showMinimized();
			break;
		case dmRestoreAll:
			foreach ( TABBED_DOCUMENT* d, documents() )
				d->showNormal();
			break;
		}
		mDocumentMode = d;
		emit documentModeChanged( mDocumentMode );
	}
}

int pTabbedWorkspace::currentIndex() const
{
	return mTabBar->currentIndex();
}

void pTabbedWorkspace::setCurrentIndex( int i )
{
	if ( mTabBar->currentIndex() != i )
	{
		mTabBar->setCurrentIndex( i );
	}
	else
	{
		TABBED_DOCUMENT* d = document( i );
		switch ( mTabMode )
		{
		case tmSDI:
			if ( mStackedWidget->currentIndex() != i )
			{
				mStackedWidget->setCurrentIndex( i );
				emit currentChanged( i );
			}
			break;
		case tmMDI:
			if ( mWorkspaceWidget->activeWindow() != d )
			{
				mWorkspaceWidget->setActiveWindow( d );
				emit currentChanged( i );
			}
			break;
		case tmTopLevel:
			if ( d )
			{
				d->raise();
				d->activateWindow();
				emit currentChanged( i );
			}
			break;
		default:
			break;
		}
	}
}

TABBED_DOCUMENT* pTabbedWorkspace::currentDocument() const
{
	return document( currentIndex() );
}

void pTabbedWorkspace::setCurrentDocument( TABBED_DOCUMENT* d )
{
	setCurrentIndex( indexOf( d ) );
}

int pTabbedWorkspace::indexOf( TABBED_DOCUMENT* d ) const
{
	return mDocuments.indexOf( d );
}

TABBED_DOCUMENT* pTabbedWorkspace::document( int i ) const
{
	return mDocuments.value( i );
}

int pTabbedWorkspace::count() const
{
	return mDocuments.count();
}

QList<TABBED_DOCUMENT*> pTabbedWorkspace::documents() const
{
	return mDocuments;
}
//
pTabbedWorkspaceCorner* pTabbedWorkspace::cornerWidget( Qt::Corner c ) const
{
	// if only one it s tabbar, no need to check
	if ( mTabLayout->count() == 1 )
		return 0;

	// get corner
	switch ( c )
	{
	case Qt::TopLeftCorner:
	case Qt::BottomLeftCorner:
		if ( mTabLayout->indexOf( mTabBar ) == 0 )
			return 0;
		return qobject_cast<pTabbedWorkspaceCorner*>( mTabLayout->itemAt( 0 )->widget() );
		break;
	case Qt::TopRightCorner:
	case Qt::BottomRightCorner:
		if ( mTabLayout->indexOf( mTabBar ) == mTabLayout->count() -1 )
			return 0;
		return qobject_cast<pTabbedWorkspaceCorner*>( mTabLayout->itemAt( mTabLayout->count() -1 )->widget() );
		break;
	}

	// shut up gcc warning
	return 0;
}

void pTabbedWorkspace::setCornerWidget( pTabbedWorkspaceCorner* w, Qt::Corner c )
{
	switch ( c )
	{
	case Qt::TopLeftCorner:
	case Qt::BottomLeftCorner:
		if ( mTabLayout->indexOf( mTabBar ) == 1 )
			delete mTabLayout->itemAt( 0 )->widget();
		mTabLayout->insertWidget( 0, w );
		break;
	case Qt::TopRightCorner:
	case Qt::BottomRightCorner:
		if ( mTabLayout->indexOf( mTabBar ) != mTabLayout->count() -1 )
			delete mTabLayout->itemAt( mTabLayout->count() -1 )->widget();
		mTabLayout->addWidget( w );
		break;
	}
}

void pTabbedWorkspace::updateCorners()
{
	pTabbedWorkspaceCorner* c;
	if ( ( c = cornerWidget( Qt::TopLeftCorner ) ) )
	{
		c->setDirection( mTabLayout->direction() );
		c->setEnabled( count() );
	}
	if ( ( c = cornerWidget( Qt::TopRightCorner ) ) )
	{
		c->setDirection( mTabLayout->direction() );
		c->setEnabled( count() );
	}
}

void pTabbedWorkspace::updateView()
{
	// add document to correct workspace
	foreach ( TABBED_DOCUMENT* d, documents() )
	{
		switch ( mTabMode )
		{
		case tmSDI:
			mStackedWidget->addWidget( d );
			break;
		case tmMDI:
			mWorkspaceWidget->addWindow( d );
			switch ( mDocumentMode )
			{
			case dmMaximized:
				if ( !d->isMaximized() )
					d->showMaximized();
				break;
			case dmCascade:
				if ( d == documents().last() )
					mWorkspaceWidget->cascade();
				break;
			case dmTile:
				if ( d == documents().last() )
					mWorkspaceWidget->tile();
				break;
			case dmIcons:
				if ( d == documents().last() )
					mWorkspaceWidget->arrangeIcons();
				break;
			case dmMinimizeAll:
				if ( !d->isMinimized() )
					d->showMinimized();
				break;
			case dmRestoreAll:
				d->showNormal();
				break;
			}
			break;
		case tmTopLevel:
			d->setParent( 0 );
			if ( !d->isVisible() )
				d->show();
			break;
		}	
	}

	// show correct workspace
	switch ( mTabMode )
	{
	case tmSDI:
		mStackedLayout->setCurrentWidget( mStackedWidget );
		break;
	case tmMDI:
		mStackedLayout->setCurrentWidget( mWorkspaceWidget );
		break;
	case tmTopLevel:
		mStackedLayout->setCurrentWidget( 0 );
		break;
	}

	// update corners
	updateCorners();
}

void pTabbedWorkspace::addDocument( TABBED_DOCUMENT* d, int i )
{
	if ( i == -1 )
		i = count();
	d->setAttribute( Qt::WA_DeleteOnClose, false );
	d->installEventFilter( this );
	mDocuments.insert( i, d );
	updateView();
}

int pTabbedWorkspace::addTab( TABBED_DOCUMENT* d, const QString& l )
{
	return insertTab( count(), d, l );
}

int pTabbedWorkspace::addTab( TABBED_DOCUMENT* d, const QIcon& i, const QString& l )
{
	return insertTab( count(), d, i, l );
}

int pTabbedWorkspace::insertTab( int i, TABBED_DOCUMENT* d, const QString& l )
{
	if ( !d || mDocuments.contains( d ) )
		return -1;
	int j = mTabBar->insertTab( i, l );
	addDocument( d, i );
	if ( j != -1 )
		emit tabInserted( j );
	return j;
}

int pTabbedWorkspace::insertTab( int j, TABBED_DOCUMENT* d, const QIcon& i, const QString& l )
{
	int k = insertTab( j, d, l );
	if ( k != -1 )
		mTabBar->setTabIcon( k, i );
	return k;
}

void pTabbedWorkspace::removeTab( int i )
{
	removeTab( document( i ) );
}

void pTabbedWorkspace::removeTab( TABBED_DOCUMENT* d )
{
	if ( !d )
		return;
	int i = indexOf( d );
	if ( i != -1 )
	{
		mDocuments.removeAll( d );
		mTabBar->removeTab( i );
		// remove document from container
		if ( mTabMode == tmSDI )
			mStackedWidget->removeWidget( d );
		updateCorners();
		emit tabRemoved( i );
		setCurrentIndex( currentIndex() );
	}
}

void pTabbedWorkspace::closeCurrentTab()
{
	TABBED_DOCUMENT* d = currentDocument();
	if ( d )
		d->close();
}
