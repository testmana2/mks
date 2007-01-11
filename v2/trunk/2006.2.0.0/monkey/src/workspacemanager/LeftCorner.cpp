#include "LeftCorner.h"
#include "Workspace.h"
#include "AbstractChild.h"
//
#include <QMenu>
#include <QActionGroup>
//
QPointer<LeftCorner> LeftCorner::mSelf = 0L;
//
LeftCorner* LeftCorner::self( Workspace* p )
{
	if ( !mSelf )
		mSelf = new LeftCorner( p );
	return mSelf;
}
//
LeftCorner::LeftCorner( Workspace* p )
	: QToolButton( p )
{
	Q_ASSERT( p != 0 );
	mWorkspace = p;
	setFixedSize( QSize( 24, 24 ) );
	setEnabled( false );
	//
	setIcon( QIcon( ":/Icons/Icons/fileopen.png" ) );
	setPopupMode( QToolButton::InstantPopup );
	setMenu( new QMenu( this ) );
	agFiles = new QActionGroup( menu() );
	//
	connect( menu(), SIGNAL( aboutToShow() ), this, SLOT( menu_aboutToShow() ) );
	connect( agFiles, SIGNAL( triggered( QAction* ) ), this, SLOT( agFiles_triggered( QAction* ) ) );
}
//
void LeftCorner::menu_aboutToShow()
{
	// clear menu actions
	menu()->clear();
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
		a = new QAction( f.fileName(), menu() );
		a->setToolTip( s );
		a->setCheckable( true );
		a->setChecked( c->currentFile() == s );
		agFiles->addAction( a );
	}
	menu()->addActions( agFiles->actions() );
}
//
void LeftCorner::agFiles_triggered( QAction* a )
{
	AbstractChild* c;
	// if no active child return
	if ( !mWorkspace->currentWidget() || !( c = qobject_cast<AbstractChild*>( mWorkspace->currentWidget() ) ) )
		return;
	c->showFile( a->toolTip() );
}
