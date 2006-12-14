#include "LeftCorner.h"
/*
//
#include <QMenu>
#include <QActionGroup>
//
QPointer<LeftCorner> LeftCorner::mSelf = 0L;
//
LeftCorner* LeftCorner::self( pTabWorkspace* p )
{
	if ( !mSelf )
		mSelf = new LeftCorner( p );
	return mSelf;
}
//
LeftCorner::LeftCorner( pTabWorkspace* p )
	: QToolButton( p )
{
	Q_ASSERT( p != 0 );
	setFixedSize( QSize( 24, 24 ) );
	//
	setIcon( QIcon( ":/Icons/Icons/fileopen.png" ) );
	setPopupMode( QToolButton::MenuButtonPopup );
	setMenu( new QMenu( this ) );
	agClassFile = new QActionGroup( menu() );
	QMetaEnum e;
	// fill agClassFile
	e = p->metaObject()->enumerator( p->metaObject()->indexOfEnumerator( "ClassFile" ) );
	for ( int i = 0; i < e.keyCount(); i++ )
	{
		QAction* a = new QAction( e.key( i ), agView );
		a->setCheckable( true );
		//if ( p->classFile() == e.value( i ) )
			//a->setChecked( true );
		a->setData( i );
	}
	//
	menu()->addMenu( tr( "Open" ) )->addActions( agClassFile->actions() );
	connect( agClassFile, SIGNAL( triggered( QAction* ) ), this, SLOT( agClassFile_triggered( QAction* ) ) );
	//
	connect( this, SIGNAL( requestChangeClassFile( Workspace::ClassFile ) ), p, SLOT( setClassFile( pTabWorkspace::TabMode ) ) );
	connect( p, SIGNAL( tabModeChanged( pTabWorkspace::TabMode ) ), this, SLOT( tabModeChanged( pTabWorkspace::TabMode ) ) );
}
//
void LeftCorner::agClassFile_triggered( QAction* a )
{
	emit requestChangeClassFile( (Workspace::ClassFile)a->data().toInt() );
}
//
void LeftCorner::classFileChanged( Workspace::ClassFile )
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
*/
