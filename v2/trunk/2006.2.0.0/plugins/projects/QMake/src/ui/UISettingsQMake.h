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
	enum Type { BinaryType, PathType };
	enum Role { TypeRole = Qt::UserRole, DataRole };
	UISettings( QWidget* = 0 );
	//
private slots:
	void loadSettings();
	void on_cbKeywords_currentIndexChanged( int );
	void on_lePath_editingFinished();
	void on_tbBrowse_clicked();
	void on_bbDialog_clicked( QAbstractButton* );
	//
};
//
#endif // UISETTINGS_H
