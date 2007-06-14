#include "pTabbedMainWindow.h"
#include "pTabbedWorkspace.h"

pTabbedMainWindow::pTabbedMainWindow( QWidget* w, Qt::WindowFlags f )
	: pMainWindow( w, f ), mWorkspace( new pTabbedWorkspace )
{
	// init tabbed workspace
	setCentralWidget( mWorkspace );
}

pTabbedWorkspace* pTabbedMainWindow::tabbedWorkspace()
{
	return mWorkspace;
}

void pTabbedMainWindow::saveState()
{
	pMainWindow::saveState();
}

void pTabbedMainWindow::restoreState()
{
	pMainWindow::restoreState();
}
