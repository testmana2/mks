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
	: mUniqueId( 0 )
{
	// need docktoolbar manager
	Q_ASSERT( t != 0 );
	Q_UNUSED( t );

	// create button group
	mButtons = new QButtonGroup( this );

	// create button frame
	mFrame = new QFrame;

	// create buttons layout
	mLayout = new QBoxLayout( QBoxLayout::LeftToRight, mFrame );

	// add frame to toolbar
	addWidget( mFrame );

	connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ), this, SLOT( internal_orientationChanged( Qt::Orientation ) ) );
	connect( mButtons, SIGNAL( buttonClicked( int ) ), this, SLOT( internal_buttonClicked( int ) ) );

	setOrientation( o );
}

bool pDockToolBar::eventFilter( QObject* o, QEvent* e )
{
/*
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
*/
	return QToolBar::eventFilter( o, e );
}

int pDockToolBar::addDock( QDockWidget* d, const QString& s, const QPixmap& p )
{
	// cancel if no dock
	if ( !d || mDocks.values().contains( d ) )
		return -1;

	// create button
	QPushButton* pb = new QPushButton( this );
	pb->setCheckable( true );
	pb->setText( s );
	pb->setIcon( p );

	// set dock title
	d->setWindowTitle( s );

	// set object name if needed
	if ( d->objectName().isEmpty() )
		d->setObjectName( QString( "QDockWidget_%1" ).arg( s ).replace( " ", "_" ).trimmed() );

	// add dock to dock list
	mDocks[mUniqueId] = d;

	// add button to layout and button group
	mButtons->addButton( pb, mUniqueId );
	mLayout->addWidget( pb );

	finir qWarning( "window: %d", mManager->mainWindow() );
	// add dock to correct area
	//if ( mManager->mainWindow()->isVisible() )
	{
	//Qt::DockWidgetArea da = mManager->mainWindow()->dockWidgetArea( d );
	//Qt::DockWidgetArea ta = mManager->ToolBarAreaToDockWidgetArea( mManager->mainWindow()->toolBarArea( this ) );
	//if ( da == Qt::NoDockWidgetArea || da != ta )
	{
		//mManager->mainWindow()->addDockWidget( ta, d, orientation() );
		d->setVisible( false );
	}
	}

	// filter the dock
	d->installEventFilter( this );

	// check button according to dock visibility
	pb->setChecked( d->isVisible() );

	// check if we need to hide/show the toolbar
	checkVisibility();

	// connect
	connect( d->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
	connect( d, SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_dockDestroyed( QObject* ) ) );

	// return unique auto increment id of the dock
	mUniqueId++;
	return mUniqueId;
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
	removeEventFilter( d );

	// delete button
	mButtons->button( id( d ) )->deleteLater();

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
	QAction* a = qobject_cast<QAction*>( sender() );
	QDockWidget* d = qobject_cast<QDockWidget*>( a->parent() );
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
	// get dock
	QDockWidget* d = dock( i );

	// if not exists return
	if ( !d )
		return;

	// set dock visibility according to button visibility
	d->setVisible( mButtons->button( i )->isChecked() );

	// emit button clicked
	emit buttonClicked( i );
}
