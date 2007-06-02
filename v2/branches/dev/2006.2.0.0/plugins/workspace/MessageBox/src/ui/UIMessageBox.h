#ifndef UIMESSAGEBOX_H
#define UIMESSAGEBOX_H
//
#include "ui_UIMessageBox.h"
//
#include <QPointer>
//
class UIMessageBox : public QDockWidget, public Ui::UIMessageBox
{
	Q_OBJECT
	//
public:
	static UIMessageBox* self( QWidget* = 0 );
	//
private:
	UIMessageBox( QWidget* = 0 );
	static QPointer<UIMessageBox> mSelf;
	//
};
//
#endif // UIMESSAGEBOX_H
