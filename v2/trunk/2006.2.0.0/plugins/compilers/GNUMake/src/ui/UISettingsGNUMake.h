#ifndef UISETTINGSGNUMAKE_H
#define UISETTINGSGNUMAKE_H
//
#include "ui_UISettingsGNUMake.h"
//
class UISettingsGNUMake : public QWidget, public Ui::UISettingsGNUMake
{
	Q_OBJECT
	//
public:
	UISettingsGNUMake( QWidget* = 0 );
	//
private slots:
	void on_tbBinary_clicked();
	void on_tbPath_clicked();
	void on_bbDialog_clicked( QAbstractButton* );
	//
};
//
#endif // UISETTINGSGNUMAKE_H
