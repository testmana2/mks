#ifndef UISETTINGS_H
#define UISETTINGS_H
//
#include "ui_UISettings.h"
//
class UISettings : public QWidget, public Ui::UISettings
{
	Q_OBJECT
	//
public:
	UISettings( QWidget* = 0 );
	//
private slots:
	void on_tbBinary_clicked();
	void on_tbPath_clicked();
	void on_bbDialog_clicked( QAbstractButton* );
	//
};
//
#endif // UISETTINGS_H
