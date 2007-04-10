#ifndef UIITEMSETTINGS_H
#define UIITEMSETTINGS_H
//
#include "ui_UIItemSettings.h"
//
#include <QPointer>
//
class ProjectItem;
class ProjectItemModel;
//
class UIItemSettings : public QDialog, public Ui::UIItemSettings
{
	Q_OBJECT
	//
public:
	static UIItemSettings* edit( ProjectItemModel*, ProjectItem*, QWidget* = 0 );
	//
protected:
	UIItemSettings( ProjectItemModel*, ProjectItem*, QWidget* = 0 );
	static QPointer<UIItemSettings> mSelf;
	ProjectItem* mItem;
	ProjectItemModel* mProject;
	//
private slots:
	void accept();
	//
};
//
#endif // UIITEMSETTINGS_H
