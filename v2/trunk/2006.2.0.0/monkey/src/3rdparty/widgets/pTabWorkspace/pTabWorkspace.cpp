#include "pTabWorkspace.h"
#include "pRightCorner.h"
//
#include <QBoxLayout>
#include <QStackedWidget>
#include <QWorkspace>
#include <QVariant>
#include <QIcon>
#include <QApplication>
//
pTabWorkspace::pTabWorkspace( QWidget* p, pTabWorkspace::TabMode m )
	: QFrame( p ), mTabMode( pTabWorkspace::tmUndefined )
{
	mTabBar = new pTabBar;
	mTabBar->setDrawBase( false );
	mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );
	//connect( mTabBar, SIGNAL( tabInserted( int, int ) ), this, SIGNAL( tabInserted( int, int ) ) );
	//connect( mTabBar, SIGNAL( tabRemoved( int, int ) ), this, SIGNAL( tabRemoved( int, int ) ) );
	connect( mTabBar, SIGNAL( currentChanged( int ) ), this, SLOT( tabBar_currentChanged( int ) ) );
	//
	mLayout2 = new QBoxLayout( QBoxLayout::LeftToRight );
	mLayout2->setMargin( 0 );
	mLayout2->setSpacing( 0 );
	mLayout2->addWidget( mTabBar );
	mLayout2->addStretch( 100 );
	//
	mLayout1 = new QBoxLayout( QBoxLayout::TopToBottom, this );
	mLayout1->setMargin( 0 );
	mLayout1->setSpacing( 0 );
	mLayout1->addLayout( mLayout2 );
	mLayout1->setAlignment( mLayout2, Qt::AlignTop );
	//
	setCornerWidget( pRightCorner::self( this ) );
	setTabMode( m );
	//
	cornerWidget()->setEnabled( count() );
}
//
pTabWorkspace::~pTabWorkspace()
{
}
//
bool pTabWorkspace::eventFilter( QObject* o, QEvent* e )
{
	QEvent::Type t = e->type();
	if ( o && o->isWidgetType() )
	{
		QWidget* w = qobject_cast<QWidget*>( o );
		if ( w )
		{
			// in top level mode / mdi activated
			if ( t == QEvent::WindowActivate )
				setCurrentWidget( w );
		}
	}
	return QFrame::eventFilter( o, e );
}
//
void pTabWorkspace::tabBar_currentChanged( int i )
{
	switch ( mTabMode )
	{
	case tmSDI:
		if ( mStacked && mStacked->currentIndex() != i )
			mStacked->setCurrentIndex( i );
		break;
	case tmMDI:
		if ( mWorkspace && mWorkspace->activeWindow() != widget( i ) )
			mWorkspace->setActiveWindow( widget( i ) );
		break;
	case tmTopLevel:
		if ( widget( i ) )
			widget( i )->activateWindow();
		break;
	default:
		break;
	}
	emit currentChanged( i );
}
//
int pTabWorkspace::addTab( QWidget* child, const QString& label )
{
	if ( !child || mDocuments.contains( child ) )
		return -1;
	int i = mTabBar->addTab( label );
	mTabBar->setTabData( i, QVariant( reinterpret_cast<quint64>( child ) ) );
	appendWidget( child );
	if ( cornerWidget() )
		cornerWidget()->setEnabled( count() );
	if ( i != -1 )
		emit tabInserted( i );
	return i;
}
//
int pTabWorkspace::addTab( QWidget* child, const QIcon& icon, const QString& label )
{
	int i = addTab( child, label );
	if ( i != -1 )
		setTabIcon( i, icon );
	return i;
}
//
QWidget* pTabWorkspace::cornerWidget( Qt::Corner corner ) const
{
	if ( mLayout2->count() == 1 )
		return 0;
	switch ( corner )
	{
	case Qt::TopLeftCorner:
	case Qt::BottomLeftCorner:
		if ( mLayout2->indexOf( mTabBar ) == 0 )
			return 0;
		return mLayout2->itemAt( 0 )->widget();
		break;
	case Qt::TopRightCorner:
	case Qt::BottomRightCorner:
		if ( mLayout2->indexOf( mTabBar ) == mLayout2->count() -1 )
			return 0;
		return mLayout2->itemAt( mLayout2->count() -1 )->widget();
		break;
	}
	// shut up gcc warning ...
	return 0;
}
//
int pTabWorkspace::count() const
{
	return mTabBar->count();
}
//
int pTabWorkspace::currentIndex() const
{
	return mTabBar->currentIndex();
}
//
Qt::TextElideMode pTabWorkspace::elideMode() const
{
	return mTabBar->elideMode();
}
//
QSize pTabWorkspace::iconSize() const
{
	return mTabBar->iconSize();
}
//
QWidget* pTabWorkspace::currentWidget() const
{
	return widget( currentIndex() );
}
//
int pTabWorkspace::indexOf( QWidget* w ) const
{
	quint64 p = reinterpret_cast<quint64>( w );
	for ( int i = 0; i < mTabBar->count(); i++ )
		if ( mTabBar->tabData( i ).toUInt() == p )
			return i;
	return -1;
}
//
int pTabWorkspace::insertTab( int index, QWidget* widget, const QString& label )
{
	if ( !widget || mDocuments.contains( widget ) )
		return -1;
	int i = mTabBar->insertTab( index, label );
	mTabBar->setTabData( i, QVariant( reinterpret_cast<quint64>( widget ) ) );
	appendWidget( widget );
	if ( cornerWidget() )
		cornerWidget()->setEnabled( count() );
	if ( i != -1 )
		emit tabInserted( i );
	return i;
}
//
int pTabWorkspace::insertTab( int index, QWidget* widget, const QIcon& icon, const QString& label )
{
	int i = insertTab( index, widget, label );
	if ( i != -1 )
		setTabIcon( i, icon );
	return i;
}
//
bool pTabWorkspace::isTabEnabled( int index ) const
{
	return mTabBar->isTabEnabled( index );
}
//
void pTabWorkspace::removeTab( int i )
{
	removeTab( widget( i ) );
}
//
void pTabWorkspace::removeTab( QWidget* w )
{
	if ( !w )
		return;
	int i = indexOf( w );
	mDocuments.removeAll( w );
	mTabBar->removeTab( i );
	if ( cornerWidget() )
		cornerWidget()->setEnabled( count() );
	emit tabRemoved( i );
}
//
void pTabWorkspace::setCornerWidget( QWidget* widget, Qt::Corner corner )
{
	switch ( corner )
	{
	case Qt::TopLeftCorner:
	case Qt::BottomLeftCorner:
		if ( mLayout2->indexOf( mTabBar ) == 1 )
			delete mLayout2->itemAt( 0 )->widget();
		mLayout2->insertWidget( 0, widget );
		break;
	case Qt::TopRightCorner:
	case Qt::BottomRightCorner:
		if ( mLayout2->indexOf( mTabBar ) != mLayout2->count() -1 )
			delete mLayout2->itemAt( mLayout2->count() -1 )->widget();
		mLayout2->addWidget( widget );
		break;
	}
}
//
void pTabWorkspace::setElideMode( Qt::TextElideMode e )
{
	mTabBar->setElideMode( e );
}
//
void pTabWorkspace::setIconSize( const QSize& s )
{
	mTabBar->setIconSize( s );
}
//
void pTabWorkspace::setTabEnabled( int index, bool enable )
{
	return mTabBar->setTabEnabled( index, enable );
}
//
void pTabWorkspace::setTabIcon( int index, const QIcon& icon )
{
	mTabBar->setTabIcon( index, icon );
}
//
void pTabWorkspace::setTabShape( QTabBar::Shape s )
{
	switch ( s )
	{
	case QTabBar::RoundedNorth:
	case QTabBar::TriangularNorth:
		mLayout1->setDirection( QBoxLayout::TopToBottom );
		mLayout2->setDirection( QBoxLayout::LeftToRight );
		break;
	case QTabBar::RoundedSouth:
	case QTabBar::TriangularSouth:
		mLayout1->setDirection( QBoxLayout::BottomToTop );
		mLayout2->setDirection( QBoxLayout::LeftToRight );
		break;
	case QTabBar::RoundedWest:
	case QTabBar::TriangularWest:
		mLayout1->setDirection( QBoxLayout::LeftToRight );
		mLayout2->setDirection( QBoxLayout::TopToBottom );
		break;
	case QTabBar::RoundedEast:
	case QTabBar::TriangularEast:
		mLayout1->setDirection( QBoxLayout::RightToLeft );
		mLayout2->setDirection( QBoxLayout::TopToBottom );
		break;
	}
	mTabBar->setShape( s );
	emit tabShapeChanged( s );
}
//
void pTabWorkspace::setTabText( int index, const QString& label )
{
	mTabBar->setTabText( index, label );
}
//
void pTabWorkspace::setTabToolTip( int index, const QString& tip )
{
	mTabBar->setTabToolTip( index, tip );
}
//
void pTabWorkspace::setTabWhatsThis( int index, const QString& text )
{
	mTabBar->setTabWhatsThis( index, text );
}
//
void pTabWorkspace::setUsesScrollButtons( bool useButtons )
{
	mTabBar->setUsesScrollButtons( useButtons );
}
//
QIcon pTabWorkspace::tabIcon( int index ) const
{
	return mTabBar->tabIcon( index );
}
//
QTabBar::Shape pTabWorkspace::tabShape() const
{
	return mTabBar->shape();
}
//
QString pTabWorkspace::tabText( int index ) const
{
	return mTabBar->tabText( index );
}
//
QString pTabWorkspace::tabToolTip( int index ) const
{
	return mTabBar->tabToolTip( index );
}
//
QString pTabWorkspace::tabWhatsThis( int index ) const
{
	return mTabBar->tabWhatsThis( index );
}
//
bool pTabWorkspace::usesScrollButtons() const
{
	return mTabBar->usesScrollButtons();
}
//
pTabWorkspace::TabMode pTabWorkspace::tabMode() const
{
	return mTabMode;
}
//
QTabBar* pTabWorkspace::tabBar() const
{
	return mTabBar;
}
//
QWidget* pTabWorkspace::widget( int index ) const
{
	return reinterpret_cast<QWidget*>( mTabBar->tabData( index ).toUInt() );
}
//
void pTabWorkspace::appendWidget( QWidget* w )
{
	w->setAttribute( Qt::WA_DeleteOnClose );
	w->installEventFilter( this );
	mDocuments.append( w );
	updateView( mTabMode );
}
//
bool pTabWorkspace::updateView( pTabWorkspace::TabMode to )
{
	bool b = false;
	QWidgetList wc;
	switch ( to )
	{
	case tmSDI:
		if ( !mStacked )
			mStacked = new QStackedWidget;
		for ( int i = 0; i < mTabBar->count(); i++ )
			mStacked->insertWidget( i, widget( i ) );
		mLayout1->addWidget( mStacked );
		setCurrentIndex( currentIndex() );
		if ( mTabMode != to )
			clearLayout();
		b = true;
		break;
	case tmMDI:
		if ( !mWorkspace )
		{
			mWorkspace = new QWorkspace;
			mWorkspace->setScrollBarsEnabled( true );
			connect( mWorkspace, SIGNAL( windowActivated( QWidget* ) ), this, SLOT( setCurrentWidget( QWidget* ) ) );
		}
		wc = mWorkspace->windowList();
		foreach ( QWidget* w, mDocuments )
		{
			if ( !wc.contains( w ) )
			{
				mWorkspace->addWindow( w );
				if ( !w->isVisible() )
					w->show();
			}
		}
		mLayout1->addWidget( mWorkspace );
		if ( count() )
			currentWidget()->showMaximized();
		if ( mTabMode != to )
			clearLayout();
		b = true;
		break;
	case tmTopLevel:
		for ( int i = 0; i < mTabBar->count(); i++ )
		{
			QWidget* w = widget( i );
			w->setParent( this, Qt::Tool | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint );
			if ( !w->isVisible() )
				w->show();
		}
		setCurrentIndex( currentIndex() );
		if ( mTabMode != to )
			clearLayout();
		b = true;
		break;
	default:
		break;
	}
	return b;
}
//
void pTabWorkspace::clearLayout()
{
	switch ( mTabMode )
	{
	case tmSDI:
		if ( mStacked )
			delete mStacked;
		break;
	case tmMDI:
		if ( mWorkspace )
			delete mWorkspace;
		break;
	case tmTopLevel:
		break;
	default:
		break;
	}
}
//
void pTabWorkspace::setCurrentIndex( int index )
{
	if ( index == -1 )
		return;
	mTabBar->setCurrentIndex( index );
	switch ( mTabMode )
	{
	case tmSDI:
		if ( mStacked && mStacked->currentIndex() != index )
			mStacked->setCurrentIndex( index );
		break;
	case tmMDI:
		if ( mWorkspace && mWorkspace->activeWindow() != widget( index ) )
			mWorkspace->setActiveWindow( widget( index ) );
		break;
	case tmTopLevel:
		widget( index )->activateWindow();
		break;
	default:
		break;
	}
}
//
void pTabWorkspace::setCurrentWidget( QWidget* widget )
{
	setCurrentIndex( indexOf( widget ) );
}
//
void pTabWorkspace::setTabMode( pTabWorkspace::TabMode t )
{
	if ( mTabMode != t )
	{
		int index = currentIndex();
		if ( updateView( t ) )
			mTabMode = t;
		setCurrentIndex( index );
		emit tabModeChanged( mTabMode );
	}
}
//
void pTabWorkspace::setWindowsMode( pTabWorkspace::WindowsMode w )
{
	if ( mWorkspace )
	{
		switch( w )
		{
		case wmCascade:
			mWorkspace->cascade();
			break;
		case wmTile:
			mWorkspace->tile();
			break;
		case wmIcons:
			mWorkspace->arrangeIcons();
			break;
		case wmMinimizeAll:
			foreach ( QWidget* w, mWorkspace->windowList() )
				w->showMinimized();
			break;
		case wmRestoreAll:
			foreach ( QWidget* w, mWorkspace->windowList() )
				w->showNormal();
			break;
		}
	}
}
//
void pTabWorkspace::closeCurrentTab()
{
	currentWidget()->close();
}
