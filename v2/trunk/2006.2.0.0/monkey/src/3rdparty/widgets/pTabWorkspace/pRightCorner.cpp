#include "pRightCorner.h"
//
#include "AbstractChild.h"
#include "Workspace.h"
//
#include <QMenu>
#include <QActionGroup>
#include <QMetaEnum>
//
QPointer<pRightCorner> pRightCorner::mSelf = 0L;
//
pRightCorner* pRightCorner::self( pTabWorkspace* p )
{
	if ( !mSelf )
		mSelf = new pRightCorner( p );
	return mSelf;
}
//
pRightCorner::pRightCorner( pTabWorkspace* p )
	: QToolButton( p )
{
	Q_ASSERT( p != 0 );
	setFixedSize( QSize( 36, 24 ) );
	//
	setIcon( QIcon( ":/Icons/Icons/fileclose.png" ) );
	setPopupMode( QToolButton::MenuButtonPopup );
	setMenu( new QMenu( this ) );
	agView = new QActionGroup( menu() );
	agWindows = new QActionGroup( menu() );
	agShape = new QActionGroup( menu() );
	//
	agFiles = new QActionGroup( menu() );
	agLayouts = new QActionGroup( menu() );
	//
	QMetaEnum e;
	// fill agView
	e = p->metaObject()->enumerator( p->metaObject()->indexOfEnumerator( "TabMode" ) );
	for ( int i = 0; i < e.keyCount(); i++ )
	{
		QAction* a = new QAction( e.key( i ), agView );
		a->setCheckable( true );
		if ( p->tabMode() == e.value( i ) )
			a->setChecked( true );
		a->setData( i );
	}
	// fill agWindows
	e = p->metaObject()->enumerator( p->metaObject()->indexOfEnumerator( "WindowsMode" ) );
	for ( int i = 0; i < e.keyCount(); i++ )
	{
		QAction* a = new QAction( e.key( i ), agWindows );
		a->setData( i );
	}
	// fill agShape
	e = p->tabBar()->metaObject()->enumerator( p->tabBar()->metaObject()->indexOfEnumerator( "Shape" ) );
	for ( int i = 0; i < e.keyCount(); i++ )
	{
		QAction* a = new QAction( e.key( i ), agShape );
		a->setCheckable( true );
		if ( p->tabShape() == e.value( i ) )
			a->setChecked( true );
		a->setData( i );
	}
	//
	menu()->addMenu( tr( "View" ) )->addActions( agView->actions() );
	menu()->addMenu( tr( "Windows" ) )->addActions( agWindows->actions() );
	menu()->addMenu( tr( "Shape" ) )->addActions( agShape->actions() );
	mLayouts = menu()->addMenu( tr( "Layouts" ) );
	mFiles = menu()->addMenu( tr( "Files" ) );
	//
	connect( agView, SIGNAL( triggered( QAction* ) ), this, SLOT( agView_triggered( QAction* ) ) );
	connect( agWindows, SIGNAL( triggered( QAction* ) ), this, SLOT( agWindows_triggered( QAction* ) ) );
	connect( agShape, SIGNAL( triggered( QAction* ) ), this, SLOT( agShape_triggered( QAction* ) ) );
	//
	connect( agLayouts, SIGNAL( triggered( QAction* ) ), this, SLOT( agLayouts_triggered( QAction* ) ) );
	connect( agFiles, SIGNAL( triggered( QAction* ) ), this, SLOT( agFiles_triggered( QAction* ) ) );
	connect( menu(), SIGNAL( aboutToShow() ), this, SLOT( menu_aboutToShow() ) );
	//
	connect( this, SIGNAL( requestChangeTabMode( pTabWorkspace::TabMode ) ), p, SLOT( setTabMode( pTabWorkspace::TabMode ) ) );
	connect( this, SIGNAL( requestChangeWindowsMode( pTabWorkspace::WindowsMode ) ), p, SLOT( setWindowsMode( pTabWorkspace::WindowsMode ) ) );
	connect( this, SIGNAL( requestChangeTabShape( QTabBar::Shape ) ), p, SLOT( setTabShape( QTabBar::Shape ) ) );
	connect( this, SIGNAL( clicked() ), p, SLOT( closeCurrentTab() ) );
	connect( p, SIGNAL( tabModeChanged( pTabWorkspace::TabMode ) ), this, SLOT( tabModeChanged( pTabWorkspace::TabMode ) ) );
	connect( p, SIGNAL( tabShapeChanged( QTabBar::Shape ) ), this, SLOT( tabShapeChanged( QTabBar::Shape ) ) );
}
//
void pRightCorner::agView_triggered( QAction* a )
{
	emit requestChangeTabMode( (pTabWorkspace::TabMode)a->data().toInt() );
}
//
void pRightCorner::agWindows_triggered( QAction* a )
{
	emit requestChangeWindowsMode( (pTabWorkspace::WindowsMode)a->data().toInt() );
}
//
void pRightCorner::agShape_triggered( QAction* a )
{
	emit requestChangeTabShape( (QTabBar::Shape)a->data().toInt() );
}
//
void pRightCorner::tabModeChanged( pTabWorkspace::TabMode m )
{
	QList<QAction*> l = agView->actions();
	foreach ( QAction* a, l )
	{
		if ( a->data().toInt() == m )
		{
			if ( !a->isChecked() )
				a->setChecked( true );
			agWindows->setEnabled( m == pTabWorkspace::tmMDI ? true : false );
			return;
		}
	}
}
//
void pRightCorner::tabShapeChanged( QTabBar::Shape s )
{
	QList<QAction*> l = agShape->actions();
	foreach ( QAction* a, l )
	{
		if ( a->data().toInt() == s )
		{
			if ( !a->isChecked() )
				a->setChecked( true );
			return;
		}
	}
}
//
void pRightCorner::menu_aboutToShow()
{
	Workspace* mWorkspace = Workspace::self();
	// clear menu actions
	foreach ( QAction* a, agLayouts->actions() )
		delete a;
	mLayouts->clear();
	foreach ( QAction* a, agFiles->actions() )
		delete a;
	mFiles->clear();
	// if no active child return
	AbstractChild* c;
	if ( !mWorkspace->currentWidget() || !( c = qobject_cast<AbstractChild*>( mWorkspace->currentWidget() ) ) )
		return;
	QFileInfo f;
	QAction* a;
	// got all files this child manage
	foreach ( QString s, c->files() )
	{
		f.setFile( s );
		a = new QAction( f.fileName(), agFiles );
		a->setToolTip( s );
		a->setCheckable( true );
		a->setChecked( c->currentFile() == s );
		agFiles->addAction( a );
	}
	mFiles->addActions( agFiles->actions() );
	//
	AbstractChild::LayoutMode m = c->layoutMode();
	if ( m != AbstractChild::lNone )
	{
		AbstractChild::LayoutMode m = c->layoutMode();
		QMetaEnum e = c->metaObject()->enumerator( c->metaObject()->indexOfEnumerator( "LayoutMode" ) );
		for ( int i = 0; i < e.keyCount(); i++ )
		{
			a = new QAction( e.key( i ), mLayouts );
			a->setCheckable( true );
			if ( c->layoutMode() == e.value( i ) )
				a->setChecked( true );
			a->setData( i );
			if ( m == AbstractChild::lNone )
				a->setEnabled( false  );
			else if ( i == AbstractChild::lNone )
				a->setEnabled( false  );
			agLayouts->addAction( a );
		}
		mLayouts->addActions( agLayouts->actions() );
	}
}
//
void pRightCorner::agFiles_triggered( QAction* a )
{
	Workspace* mWorkspace = Workspace::self();
	AbstractChild* c;
	// if no active child return
	if ( !( c = qobject_cast<AbstractChild*>( mWorkspace->currentWidget() ) ) )
		return;
	c->showFile( a->toolTip() );
}
//
void pRightCorner::agLayouts_triggered( QAction* a )
{
	Workspace* mWorkspace = Workspace::self();
	AbstractChild* c;
	// if no active child return
	if ( !( c = qobject_cast<AbstractChild*>( mWorkspace->currentWidget() ) ) )
		return;
	c->setLayoutMode( (AbstractChild::LayoutMode)a->data().toInt() );
}
