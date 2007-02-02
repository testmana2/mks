#ifndef UISETTINGSQMAKE_H
#define UISETTINGSQMAKE_H
//
#include "ui_UISettingsQMake.h"
//
class UISettingsQMake : public QWidget, public Ui::UISettingsQMake
{
	Q_OBJECT
	//
public:
	enum Type { BinaryType, PathType };
	enum Role { TypeRole = Qt::UserRole, DataRole };
	UISettingsQMake( QWidget* = 0 );
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
#endif // UISETTINGSQMAKE_H
