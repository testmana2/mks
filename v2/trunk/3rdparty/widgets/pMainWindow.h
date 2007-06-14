#ifndef PMAINWINDOW_H
#define PMAINWINDOW_H

#include "MonkeyExport.h"

#include <QMainWindow>

class pSettings;
class pMenuBar;
class pDockToolBarManager;
class pDockToolBar;

class Q_MONKEY_EXPORT pMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	pMainWindow( QWidget* = 0, Qt::WindowFlags = 0 );

	pSettings* settings();
	pMenuBar* menuBar();
	pDockToolBarManager* dockToolBarManager();
	pDockToolBar* dockToolBar( Qt::ToolBarArea );

protected:
	void hideEvent( QHideEvent* );

public slots:
	virtual void saveState();
	virtual void restoreState();

};

#endif // PMAINWINDOW_H
