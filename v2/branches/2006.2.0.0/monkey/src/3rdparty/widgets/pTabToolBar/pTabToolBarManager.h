#ifndef PTABTOOLBARMANAGER_H
#define PTABTOOLBARMANAGER_H
//
#include <QHash>
//
#include "pTabToolBar.h"
#include "MonkeyExport.h"
//
class QMainWindow;
class QSettings;
//
class Q_MONKEY_EXPORT pTabToolBarManager : public QObject
{
	Q_OBJECT
	//
public:
	enum Align { Left = 1, Right, Top, Bottom };
	//
	pTabToolBarManager( QMainWindow* );
	//
	QMainWindow* mainWindow() const;
	pTabToolBar* bar( pTabToolBarManager::Align );
	pTabToolBarManager::Align dockWidgetAreaToAlign( Qt::DockWidgetArea ) const;
	virtual void restoreState( QSettings* );
	virtual void saveState( QSettings* );
	//
private:
	QMainWindow* mMain;
	QHash<pTabToolBarManager::Align, pTabToolBar*> mBars;
	//
public slots:
	void dockWidgetTabToolBarChanged( QDockWidget*, pTabToolBar* );
	//
};
//
#endif // PTABTOOLBARMANAGER_H
