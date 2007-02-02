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
	void on_bbDialog_clicked( QAbstractButton* );
	//
signals:
	void updateCodec();
	//
};
//
#endif // UISETTINGS_H
