#include "pMainWindow.h"
#include "pSettings.h"
#include "pMenuBar.h"
#include "pDockToolBarManager.h"
#include "pDockToolBar.h"

pMainWindow::pMainWindow( QWidget* w, Qt::WindowFlags f )
	: QMainWindow( w, f )
{
	// init settings
	settings();

	// init menubar
	setMenuBar( menuBar() );

	// init dock toolbar manager
	dockToolBarManager()->setSettings( settings() );

	// init toolbar
	dockToolBar( Qt::TopToolBarArea );
	dockToolBar( Qt::BottomToolBarArea );
	dockToolBar( Qt::LeftToolBarArea );
	dockToolBar( Qt::RightToolBarArea );
}

void pMainWindow::hideEvent( QHideEvent* )
{
	saveState();
}

pSettings* pMainWindow::settings()
{
	return pSettings::instance();
}

pMenuBar* pMainWindow::menuBar()
{
	return pMenuBar::instance( this );
}

pDockToolBarManager* pMainWindow::dockToolBarManager()
{
	return pDockToolBarManager::instance( this );
}

pDockToolBar* pMainWindow::dockToolBar( Qt::ToolBarArea a )
{
	return dockToolBarManager()->bar( a );
}

void pMainWindow::saveState()
{
	dockToolBarManager()->saveState();
	settings()->saveState( this );
}

void pMainWindow::restoreState()
{
	dockToolBarManager()->restoreState();
	settings()->restoreState( this );
}
