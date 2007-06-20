/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef UIMAIN_H
#define UIMAIN_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pMainWindow.h"

class QActionGroup;
class pWorkspace;

class Q_MONKEY_EXPORT UIMain : public pMainWindow, public QSingleton<UIMain>
{
	Q_OBJECT
	friend class QSingleton<UIMain>;

public:
	pWorkspace* workspace();

private:
	UIMain( QWidget* = 0 );
	void initMenuBar();
	void initToolBar();
	void initConnections();
	void initGui();

	QActionGroup* agStyles;

public slots:
	void agStyles_triggered( QAction* );
	void aboutApplication_triggered();
	void aboutQt_triggered();
#ifdef __COVERAGESCANNER__
	void testReport_triggered();
#endif

};

#endif // UIMAIN_H
