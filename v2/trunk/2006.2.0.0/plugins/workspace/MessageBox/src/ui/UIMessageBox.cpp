#include "UIMessageBox.h"
//
QPointer<UIMessageBox> UIMessageBox::mSelf = 0L;
//
UIMessageBox* UIMessageBox::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIMessageBox( p );
	return mSelf;
}
//
UIMessageBox::UIMessageBox( QWidget* p )
	: QDockWidget( p )
{
	setupUi( this );
	setMinimumHeight( 100 );
}
