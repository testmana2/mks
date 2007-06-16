#include "pMainWindow.h"
#include "pSettings.h"
#include "pActionManager.h"
#include "pMenuBar.h"
#include "pDockToolBarManager.h"
#include "pDockToolBar.h"

pMainWindow::pMainWindow( QWidget* w, Qt::WindowFlags f )
	: QMainWindow( w, f )
{
	// init settings
	settings();

	// init action manager
	pActionManager::instance()->setSettings( settings() );

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
#if QT_VERSION >= 0x040300
	// there is a bug when restoring visible dock with qt 4.3.0
	foreach ( QDockWidget* d, findChildren<QDockWidget*>() )
		d->hide();
#endif
	dockToolBarManager()->saveState();
	settings()->saveState( this );
}

void pMainWindow::restoreState()
{
	dockToolBarManager()->restoreState();
	settings()->restoreState( this );
}
