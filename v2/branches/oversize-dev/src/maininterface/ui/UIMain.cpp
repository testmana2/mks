/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 15h10
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "UIMain.h"
#include "MenuBar.h"
#include "Settings.h"
#include "main.h"
//
#include <QCloseEvent>
//
QPointer<UIMain> UIMain::mSelf = 0L;
//
UIMain* UIMain::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIMain( p );
	return mSelf;
}
//
UIMain::UIMain( QWidget* p )
	: QMainWindow( p ), mInit( false )
{
	setupUi( this );
}
//
void UIMain::closeEvent( QCloseEvent* e )
{
	Settings::current()->saveState( this );
	QMainWindow::closeEvent( e );
}
