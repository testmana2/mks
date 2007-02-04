#include "pTabToolBar.h"
#include "pTabToolBarManager.h"
//
#include <QDockWidget>
#include <QMainWindow>
#include <QAction>
#include <QEvent>
//
pTabToolBar::pTabToolBar( pTabToolBarManager* t, KMultiTabBar::KMultiTabBarMode m )
	: mButtonId( -1 ), mTabId( -1 )
{
	Q_ASSERT( t != 0 );
	mManager = t;
	mMultiTabBar = new KMultiTabBar( m, this );
	mAction = addWidget( mMultiTabBar );
	connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ), this, SLOT( internal_orientationChanged( Qt::Orientation ) ) );
}
//
bool pTabToolBar::eventFilter( QObject* o, QEvent* e )
{
	QEvent::Type t = e->type();
	if ( o && o->isWidgetType() )
	{
		QDockWidget* d = qobject_cast<QDockWidget*>( o );
		if ( d )
		{
			if ( t == QEvent::Show )
				setTab( tabIndexOf( d ), true );
			else if ( t == QEvent::Close )
				setTab( tabIndexOf( d ), false );
		}
	}
	return QToolBar::eventFilter( o, e );
}
//
int pTabToolBar::appendButton( const QPixmap& p, QMenu* m, const QString& s )
{
	mButtonId++;
	mMultiTabBar->appendButton( p, mButtonId, m, s );
	connect( button( mButtonId ), SIGNAL( clicked( int ) ), this, SIGNAL( buttonClicked( int ) ) );
	if ( ( buttons()->count() || tabs()->count() ) && !isVisible() )
		show();
	return mButtonId;
}
//
void pTabToolBar::removeButton( int i )
{
	mMultiTabBar->removeButton( i );
	checkVisibility();
}
//
KMultiTabBarButton* pTabToolBar::button( int i ) const
{
	return mMultiTabBar->button( i );
}
//
QList<KMultiTabBarButton*>* pTabToolBar::buttons()
{
	return mMultiTabBar->buttons();
}
//
int pTabToolBar::appendTab( QDockWidget* d, const QPixmap& p, const QString& s )
{
	if ( !d )
		return -1;
	mTabId++;
	mMultiTabBar->appendTab( p, mTabId, s );
	mTabDocks[mTabId] = d;
	if ( d->objectName().isEmpty() )
		d->setObjectName( QString( "QDockWidget_%1" ).arg( s ).replace( " ", "_" ) );
	d->setWindowTitle( s );
	Qt::DockWidgetArea da = mManager->mainWindow()->dockWidgetArea( d );
	Qt::DockWidgetArea ta = (Qt::DockWidgetArea)( ( int ) mManager->mainWindow()->toolBarArea( this ) );
	if ( da == Qt::NoDockWidgetArea || da != ta )
	{
		mManager->mainWindow()->addDockWidget( ta, d, orientation() );
		d->setVisible( false );
	}
	if ( d->isVisible() )
		setTab( mTabId, true );
	d->installEventFilter( this );
	connect( tab( mTabId ), SIGNAL( clicked( int ) ), this, SLOT( internal_tabClicked( int ) ) );
	connect( d->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
	connect( d, SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_childDestroyed( QObject* ) ) );
	if ( ( buttons()->count() || tabs()->count() ) && !isVisible() )
		show();
	return mTabId;
}
//
void pTabToolBar::removeTab( int i )
{
	if ( mTabDocks.contains( i ) )
	{
		dockWidget( i )->removeEventFilter( this );
		disconnect( tab( i ), SIGNAL( clicked( int ) ), this, SLOT( internal_tabClicked( int ) ) );
		disconnect( dockWidget( i )->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
		disconnect( dockWidget( i ), SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_childDestroyed( QObject* ) ) );
		mMultiTabBar->removeTab( i );
		mTabDocks.remove( i );
		checkVisibility();
	}
}
//
void pTabToolBar::setTab( int i, bool b )
{
	mMultiTabBar->setTab( i, b );
}
//
bool pTabToolBar::isTabRaised( int i ) const
{
	return mMultiTabBar->isTabRaised( i );
}
//
KMultiTabBarTab* pTabToolBar::tab( int i ) const
{
	return mMultiTabBar->tab( i );
}
//
QList<KMultiTabBarTab*>* pTabToolBar::tabs()
{
	return mMultiTabBar->tabs();
}
//
void pTabToolBar::removeTab( QDockWidget* d )
{
	foreach ( int i, mTabDocks.keys() )
	{
		if ( mTabDocks.value( i ) == d )
		{
			removeTab( i );
			return;
		}
	}
}
//
int pTabToolBar::tabIndexOf( QDockWidget* d ) const
{
	if ( !d )
		return -1;
	foreach ( int i, mTabDocks.keys() )
		if ( dockWidget( i ) == d )
			return i;
	return -1;
}
//
KMultiTabBarTab* pTabToolBar::tabOf( QDockWidget* d ) const
{
	int i = tabIndexOf( d );
	if ( i != -1 )
		return tab( i );
	return 0;
}
//
QDockWidget* pTabToolBar::dockWidget( int i ) const
{
	if ( !mTabDocks.contains( i ) )
		return 0;
	return mTabDocks.value( i );
}
//
void pTabToolBar::setPosition( KMultiTabBar::KMultiTabBarPosition p )
{
	mMultiTabBar->setPosition( p );
}
//
KMultiTabBar::KMultiTabBarPosition pTabToolBar::position() const
{
	return mMultiTabBar->position();
}
//
void pTabToolBar::setStyle( KMultiTabBar::KMultiTabBarStyle s )
{
	mMultiTabBar->setStyle( s );
}
//
KMultiTabBar::KMultiTabBarStyle pTabToolBar::tabStyle() const
{
	return mMultiTabBar->tabStyle();
}
//
void pTabToolBar::showActiveTabTexts( bool b )
{
	mMultiTabBar->showActiveTabTexts( b );
}
//
QAction* pTabToolBar::tabBarAction()
{
	return mAction;
}
//
void pTabToolBar::checkVisibility()
{
	if ( ( !buttons()->count() && !tabs()->count() && actions().count() == 1 ) && isVisible() )
		hide();
}
//
void pTabToolBar::internal_orientationChanged( Qt::Orientation o )
{
	foreach ( QDockWidget* d, mTabDocks )
		if ( d )
			mManager->mainWindow()->addDockWidget( mManager->mainWindow()->dockWidgetArea( d ), d, o );
}
//
void pTabToolBar::internal_tabClicked( int i )
{
	foreach ( int j, mTabDocks.keys() )
	{
		if ( j != i )
			setTab( j, false );
		QDockWidget* d = dockWidget( j );
		if ( d )
			d->setVisible( isTabRaised( j ) );
	}
	emit tabClicked( i );
}
//
void pTabToolBar::internal_dockChanged()
{
	QAction* a = qobject_cast<QAction*>( sender() );
	QDockWidget* d = qobject_cast<QDockWidget*>( a->parent() );
	if ( !d ||
		 d->isFloating() ||
	     (Qt::ToolBarArea)mManager->mainWindow()->dockWidgetArea( d ) == mManager->mainWindow()->toolBarArea( this )
       )
		return;
	else
		emit dockWidgetTabToolBarChanged( d, this );
}
//
void pTabToolBar::internal_childDestroyed( QObject* o )
{
	// qobject_cast don t work with QDockWidget ?!
	//QDockWidget* d = qobject_cast<QDockWidget*>( o );
	QDockWidget* d = (QDockWidget*)o;
	if ( d )
	{
		int i = tabIndexOf( d );
		if ( mTabDocks.contains( i ) )
		{
			mMultiTabBar->removeTab( i );
			mTabDocks.remove( i );
			if ( ( !buttons()->count() && !tabs()->count() ) && isVisible() )
				hide();
		}
	}
}
