#include "pRightCorner.h"
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
	{
		mSelf = new pRightCorner( p );
		if ( p )
		{
			connect( mSelf, SIGNAL( requestChangeTabMode( pTabWorkspace::TabMode ) ), p, SLOT( setTabMode( pTabWorkspace::TabMode ) ) );
			connect( mSelf, SIGNAL( requestChangeWindowsMode( pTabWorkspace::WindowsMode ) ), p, SLOT( setWindowsMode( pTabWorkspace::WindowsMode ) ) );
			connect( mSelf, SIGNAL( requestChangeTabShape( QTabBar::Shape ) ), p, SLOT( setTabShape( QTabBar::Shape ) ) );
			connect( p, SIGNAL( tabModeChanged( pTabWorkspace::TabMode ) ), mSelf, SLOT( tabModeChanged( pTabWorkspace::TabMode ) ) );
			connect( p, SIGNAL( tabShapeChanged( QTabBar::Shape ) ), mSelf, SLOT( tabShapeChanged( QTabBar::Shape ) ) );
			
		}
	}
	return mSelf;
}
//
pRightCorner::pRightCorner( pTabWorkspace* p )
	: QToolButton( p )
{
	if ( p )
	{
		setIcon( QIcon( ":/Icons/Icons/viewstyle.png" ) );
		setPopupMode( QToolButton::InstantPopup );
		setMenu( new QMenu( this ) );
		agView = new QActionGroup( menu() );
		agWindows = new QActionGroup( menu() );
		agShape = new QActionGroup( menu() );
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
		connect( agView, SIGNAL( triggered( QAction* ) ), this, SLOT( agView_triggered( QAction* ) ) );
		connect( agWindows, SIGNAL( triggered( QAction* ) ), this, SLOT( agWindows_triggered( QAction* ) ) );
		connect( agShape, SIGNAL( triggered( QAction* ) ), this, SLOT( agShape_triggered( QAction* ) ) );
	}
	else deleteLater();
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
