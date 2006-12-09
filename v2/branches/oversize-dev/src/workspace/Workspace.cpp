#include "Workspace.h"
#include "MenuBar.h"
#include "ProjectManager.h"
#include "MonkeyEditor.h"


//
QPointer<Workspace> Workspace::mSelf = 0L;
//
Workspace* Workspace::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new Workspace( p );
	return mSelf;
}
//
Workspace::Workspace( QWidget* p )
	: pTabWorkspace( p )
{
}
//
Workspace::~Workspace()
{
}
//
void Workspace::initialize()
{
        // project menu
        MenuBar* mb = MenuBar::self();
        mb->beginGroup( "mFile" );
                mb->menu( "mSave" )->setEnabled( false );
                mb->menu( "mClose" )->setEnabled( false );
                mb->action( "aSaveAsTemplate" )->setEnabled( false );
                mb->action( "aQuickPrint" )->setEnabled( false );
                mb->action( "aPrint" )->setEnabled( false );
        mb->endGroup();

        // set mode
        setTabShape( QTabBar::RoundedSouth );
        setTabMode( tmMDI );

        QString s = QString( "Filename" );
        MonkeyEditor* v = new MonkeyEditor;
        v->setWindowTitle( s );
        v->resize( 200, 100 );
        addTab( v, s);

/*
	QString p = QString( "Another" );
	MonkeyEditor* w = new MonkeyEditor;
        w->setCaretLineVisible(true);
        w->setCaretLineBackgroundColor("green");
	w->setWindowTitle( p );
	w->resize( 200, 100 );
	addTab( w, p);
*/

	delete widget( 1 );
}
//
ProjectManager* Workspace::projectManager()
{
	return ProjectManager::self();
}
