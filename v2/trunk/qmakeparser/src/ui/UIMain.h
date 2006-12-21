#ifndef UIMAIN_H
#define UIMAIN_H
//
#include "ui_UIMain.h"
//
#include <QPointer>
//
class AbstractProjectProxy;
//
class UIMain : public QWidget, public Ui::UIMain
{
	Q_OBJECT
	//
public:
	static UIMain* self( QWidget* = 0 );
	//
private:
	UIMain( QWidget* = 0 );
	static QPointer<UIMain> _self;
	QPointer<AbstractProjectProxy> mProxy;
	//
private slots:
	void on_pbOpen_clicked();
	void on_pbClose_clicked();
	//
};
//
#endif // UIMAIN_H
