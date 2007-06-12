#ifndef PDOCKTOOLBARMANAGER_H
#define PDOCKTOOLBARMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QToolBar>
#include <QHash>

class QMainWindow;
class pDockToolBar;
class QSettings;
class QDockWidget;

class Q_MONKEY_EXPORT pDockToolBarManager : public QObject, public QSingleton<pDockToolBarManager>
{
	Q_OBJECT
	friend class QSingleton<pDockToolBarManager>;

public:
	QMainWindow* mainWindow() const;
	pDockToolBar* bar( Qt::ToolBarArea );
	static Qt::ToolBarArea dockWidgetAreaToToolBarArea( Qt::DockWidgetArea );
	static Qt::DockWidgetArea ToolBarAreaToDockWidgetArea( Qt::ToolBarArea );
	virtual void restoreState( QSettings* );
	virtual void saveState( QSettings* );

private:
	pDockToolBarManager( QMainWindow* );

	QMainWindow* mMain;
	QHash<Qt::ToolBarArea, pDockToolBar*> mBars;

public slots:
	void dockWidgetAreaChanged( QDockWidget*, pDockToolBar* );

};

#endif // PDOCKTOOLBARMANAGER_H
