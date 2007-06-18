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
#include "pTabbedMainWindow.h"

class QActionGroup;

class Q_MONKEY_EXPORT UIMain : public pTabbedMainWindow, public QSingleton<UIMain>
{
	Q_OBJECT
	friend class QSingleton<UIMain>;

private:
	UIMain( QWidget* = 0 );
	void initMenuBar();
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
