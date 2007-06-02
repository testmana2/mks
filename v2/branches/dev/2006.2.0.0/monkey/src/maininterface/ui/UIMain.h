/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 15h10
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef UIMAIN_H
#define UIMAIN_H
//
#include "ui_UIMain.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class QActionGroup;
//
class Q_MONKEY_EXPORT UIMain : public QMainWindow, public Ui::UIMain
{
	Q_OBJECT
	//
public:
	static UIMain* self( QWidget* = 0 );
	//
	void restoreState();
	void saveState();
	//
protected:
	void showEvent( QShowEvent* );
	void closeEvent( QCloseEvent* );
	//
private:
	UIMain( QWidget* = 0 );
	void initGui();
	void initConnections();
	//
	static QPointer<UIMain> mSelf;
	bool mInit;
	QActionGroup* agStyles;
	//
public slots:
	void agStyles_triggered( QAction* );
	void aboutApplication_triggered();
	void aboutQt_triggered();
	void testReport_triggered();
	//
};
//
#endif // UIMAIN_H
