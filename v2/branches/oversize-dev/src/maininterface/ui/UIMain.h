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
//
#include <QPointer>
//
class UIMain : public QMainWindow, public Ui::UIMain
{
	Q_OBJECT
	//
public:
	static UIMain* self( QWidget* = 0 );
	//
public slots:
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIMain( QWidget* = 0 );
	//
	static QPointer<UIMain> mSelf;
	bool mInit;
	//
private slots:
	//
};
//
#endif // UIMAIN_H
