/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 01h15
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : MenuBar.h
 * LICENSE      : GPL
 * COMMENTARY   : This class is the monkey studio menuBar created using the pMenuBar class
 ********************************************************************************************************/
#ifndef MenuBar_H
#define MenuBar_H
//
#include "pMenuBar.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT MenuBar : public pMenuBar
{
	Q_OBJECT
	//
public:
	static MenuBar* self( QWidget* = 0 );
	//
private:
	MenuBar( QWidget* = 0 );
	void initialize();
	//
	static QPointer<MenuBar> mSelf;
	//
};
//
#endif // MenuBar_H
