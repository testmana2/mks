#ifndef PTABBEDMAINWINDOW_H
#define PTABBEDMAINWINDOW_H

#include "MonkeyExport.h"
#include "pMainWindow.h"

class pTabbedWorkspace;

class Q_MONKEY_EXPORT pTabbedMainWindow : public pMainWindow
{
	Q_OBJECT

public:
	pTabbedMainWindow( QWidget* = 0, Qt::WindowFlags = 0 );

	pTabbedWorkspace* tabbedWorkspace();

private:
	pTabbedWorkspace* mWorkspace;

public slots:
	virtual void saveState();
	virtual void restoreState();

};

#endif // PTABBEDMAINWINDOW_H
