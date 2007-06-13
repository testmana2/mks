#include "pDockToolBar.h"
#include "pDockToolBarManager.h"

#include <QButtonGroup>
#include <QFrame>
#include <QBoxLayout>
#include <QEvent>
#include <QDockWidget>
#include <QPushButton>
#include <QAction>
#include <QMainWindow>

pDockToolBar::pDockToolBar( pDockToolBarManager* t, Qt::Orientation o )
	: mManager( t ), mUniqueId( 0 ), mExclusive( true )
{
	// need docktoolbar manager
	Q_ASSERT( t != 0 );
	Q_UNUSED( t );

	// create button frame
	mFrame = new QFrame;

	// create buttons layout
	mLayout = new QBoxLayout( QBoxLayout::LeftToRight, mFrame );
	mLayout->setMargin( 0 );
	mLayout->setSpacing( 0 );

	// add frame to toolbar
	addWidget( mFrame );

	connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ), this, SLOT( internal_orientationChanged( Qt::Orientation ) ) );

	// set toolbar/layout orientation
	setOrientation( o );
}

bool pDockToolBar::eventFilter( QObject* o, QEvent* e )
{
	// get event type
	QEvent::Type t = e->type();
	// try casting object to dockwidget
	QDockWidget* d = qobject_cast<QDockWidget*>( o );

	// if it s a dock widget
	if ( d && ( t == QEvent::Show || t == QEvent::Close || t == QEvent::Hide ) )
	{
		button( d )->setChecked( t == QEvent::Show );
	}

	// deturn default event filter
	return QToolBar::eventFilter( o, e );
}

int pDockToolBar::addDock( QDockWidget* d, const QString& s, const QPixmap& p )
{
	// cancel if no dock
	if ( !d || mDocks.values().contains( d ) )
		return -1;

	// check if this dock is already in another bar, and remove it
	pDockToolBar* tb = mManager->bar( mManager->dockWidgetAreaToToolBarArea( mManager->mainWindow()->dockWidgetArea( d ) ) );
	if ( tb && tb->docks().contains( d ) )
		tb->removeDock( d );

	// set dock title
	if ( !s.isEmpty() )
		d->setWindowTitle( s );

	// set object name if needed
	if ( d->objectName().isEmpty() )
		d->setObjectName( QString( "QDockWidget_%1" ).arg( s ).replace( " ", "_" ).trimmed() );

	// set dock icon
	if ( !p.isNull() )
		d->setWindowIcon( p );

	// create button
	QPushButton* pb = new QPushButton( this );
	pb->setCheckable( true );
	pb->setText( d->windowTitle() );
	pb->setIcon( p );

	// add button to layout
	mLayout->addWidget( pb );

	// add dock to correct area
	Qt::DockWidgetArea da = mManager->mainWindow()->dockWidgetArea( d );
	Qt::DockWidgetArea ta = mManager->ToolBarAreaToDockWidgetArea( mManager->mainWindow()->toolBarArea( this ) );
	if ( da != ta )
		mManager->mainWindow()->addDockWidget( ta, d, orientation() );

	// if exclusive, hide current dock
	/*
	if ( mExclusive )
	{
		QDockWidget* dw = dock( mButtons->checkedId() );
		if ( dw )
			dw->hide();
	}
	*/

	// set dock visibility according to window visibility
	d->setVisible( window()->isVisible() );

	// check button according to dock visibility
	pb->setChecked( d->isVisible() );

	// add dock/button to list
	mButtons[mUniqueId] = pb;
	mDocks[mUniqueId] = d;

	// filter the dock
	d->installEventFilter( this );

	// connect
	connect( d->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
	connect( d, SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_dockDestroyed( QObject* ) ) );
	connect( pb, SIGNAL( clicked( bool ) ), this, SLOT( internal_buttonClicked( bool ) ) );

	// check if we need to hide/show the toolbar
	checkVisibility();

	// return unique auto increment id of the dock
	return mUniqueId++;
}

void pDockToolBar::removeDock( QDockWidget* d )
{
	// cancel if dock is not acutally own by this toolbar
	if ( !mDocks.values().contains( d ) )
		return;

	// disconnect
	disconnect( d->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
	disconnect( d, SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_dockDestroyed( QObject* ) ) );

	// remove filter event
	d->removeEventFilter( this );

	// delete button
	QAbstractButton* b = button( d );
	mButtons.remove( id( d ) );
	b->deleteLater();

	// remove dock from list
	mDocks.remove( id( d ) );

	// check if we need to hide/show the toolbar
	checkVisibility();
}

void pDockToolBar::removeDock( int i )
{
	removeDock( dock( i ) );
}

bool pDockToolBar::isDockVisible( QDockWidget* d ) const
{
	// cancel if no dock
	if ( !d )
		return false;

	// if dock is in internal list
	if ( mDocks.values().contains( d ) )
		return mButtons->button( id( d ) )->isChecked();

	// else return widget visibility
	return d->isVisible();
}

bool pDockToolBar::isDockVisible( int i ) const
{
	return isDockVisible( dock( i ) );
}

int pDockToolBar::id( QDockWidget* d ) const
{
	return mDocks.values().contains( d ) ? mDocks.key( d ) : -1;
}

QDockWidget* pDockToolBar::dock( int i ) const
{
	return mDocks.value( i );
}

QDockWidget* pDockToolBar::dock( QAbstractButton* b ) const
{
	return mDocks.value( mButtons->id( b ) );
}

QList<QDockWidget*> pDockToolBar::docks() const
{
	return mDocks.values();
}

QAbstractButton* pDockToolBar::button( QDockWidget* d ) const
{
	return mButtons->button( id( d ) );
}

QList<QAbstractButton*> pDockToolBar::buttons() const
{
	return mButtons->buttons();
}

int pDockToolBar::count() const
{
	return mButtons->buttons().count();
}

void pDockToolBar::checkVisibility()
{
	if ( !isVisible() && count() )
		show();
	else if ( isVisible() && !count() )
		hide();
}

void pDockToolBar::setExclusive( bool b )
{
	if ( mExclusive == b )
		return;
	mExclusive = b;

	// need hide all except last checked
}

bool pDockToolBar::exclusive() const
{
	return mExclusive;
}

void pDockToolBar::internal_orientationChanged( Qt::Orientation o )
{
	// change layout direction
	switch ( o )
	{
		case Qt::Horizontal:
			mLayout->setDirection( QBoxLayout::LeftToRight );
			break;
		case Qt::Vertical:
			mLayout->setDirection( QBoxLayout::TopToBottom );
			break;
	}

	// move docks
	foreach ( QDockWidget* d, mDocks )
		mManager->mainWindow()->addDockWidget( mManager->mainWindow()->dockWidgetArea( d ), d, o );
}

void pDockToolBar::internal_dockChanged()
{
	// get dock action
	QAction* a = qobject_cast<QAction*>( sender() );

	// get dock
	QDockWidget* d = qobject_cast<QDockWidget*>( a->parent() );

	// no position changed, return
	if ( !d || d->isFloating() || pDockToolBarManager::dockWidgetAreaToToolBarArea( mManager->mainWindow()->dockWidgetArea( d ) ) == mManager->mainWindow()->toolBarArea( this ) )
		return;
	else
		emit dockWidgetAreaChanged( d, this );
}

void pDockToolBar::internal_dockDestroyed( QObject* o )
{
	removeDock( qobject_cast<QDockWidget*>( o ) );
}

void pDockToolBar::internal_buttonClicked( int i )
{
qWarning( "clicked 1: id %d, old id: %d, d: %d", i, mOldId, mButtons->button( i )->isChecked() );

	if ( mButtons->exclusive() )
	{
		if ( mOldId == i && mButtons->button( i )->isChecked() )
		{
qWarning( "forcing button change state" );
			mButtons->setExclusive( false );
			mButtons->button( i )->setChecked( !mButtons->button( i )->isChecked() );
			mButtons->setExclusive( true );

		}
	}

qWarning( "clicked 2: id %d, old id: %d, d: %d", i, mOldId, mButtons->button( i )->isChecked() );

qWarning( "button checked: %s", mButtons->checkedButton() ? qPrintable( mButtons->checkedButton()->text() ) : "none" );

mOldId = i;

return;

	// if last button was same, need switch current visibility
	if ( mButtons->exclusive() && mOldId == i )
	{
		dock( i )->hide();
		return;
	}

	// set dock visibility according to button visibility
	qWarning( "e: %d", mButtons->button( i )->isChecked() );
	dock( i )->setVisible( mButtons->button( i )->isChecked() );

	// if exclusive, hide current dock
	if ( mButtons->exclusive() && mOldId != i )
	{
	/*
		QDockWidget* d = dock( mOldId );
		if ( d )
			d->hide();
	*/
	}

	mOldId = i;

	// emit button clicked
	emit buttonClicked( i );
}
