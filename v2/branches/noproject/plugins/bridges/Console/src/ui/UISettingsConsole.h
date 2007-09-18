#ifndef UISETTINGSCONSOLE_H
#define UISETTINGSCONSOLE_H
//
#include "ui_UISettingsConsole.h"
//
class UISettingsConsole : public QWidget, public Ui::UISettingsConsole
{
	Q_OBJECT
	//
public:
	UISettingsConsole( QWidget* = 0 );
	//
private slots:
	void on_bbDialog_clicked( QAbstractButton* );
	//
signals:
	void updateCodec();
	//
};
//
#endif // UISETTINGSCONSOLE_H
