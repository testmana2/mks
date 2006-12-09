#include "MenuTools.h"
#include "MenuBar.h"
#include "UIToolsEdit.h"
#include "Settings.h"
//
#include <QProcess>
//
QPointer<MenuTools> MenuTools::mSelf = 0L;
//
MenuTools* MenuTools::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new MenuTools( p );
	return mSelf;
}
//
MenuTools::MenuTools( QObject* p )
	: QObject( p )
{
	connect( MenuBar::self()->menu( "mTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( triggered( QAction* ) ) );
}
//
void MenuTools::initialize()
{
	// got menu
	QMenu* m = MenuBar::self()->menu( "mTools" );
	// create action
	QSettings* s = Settings::current();
	int n = s->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		s->setArrayIndex( i );
		QAction* ac = new QAction( QIcon( s->value( "Icon" ).toString() ), s->value( "Caption" ).toString(), m );
		ac->setStatusTip( s->value( "Command" ).toString() );
		m->addAction( ac );
	}
	s->endArray();
}
//
void MenuTools::triggered( QAction* a )
{
	if ( a != MenuBar::self()->action( "mTools/aEdit" ) )
	{
		if ( !QProcess::startDetached( a->statusTip() ) )
			qWarning( qPrintable( a->statusTip().prepend( tr( "can't start: " ) ) ) );
	}
	else if ( UIToolsEdit::self()->exec() )
	{
		// got menu
		QMenu* m = MenuBar::self()->menu( "mTools" );
		// got all menu action
		QList<QAction*> l = m->actions();
		// got action to not delete
		QAction* ae = MenuBar::self()->action( "mTools/aEdit" );
		QAction* as = MenuBar::self()->action( "mTools/aSeparator1" );
		// delete unneeded action
		foreach ( QAction* ac, l )
		{
			if ( ac != ae && ac != as )
				delete ac;
		}
		// initialize
		initialize();
	}
}
