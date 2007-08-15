/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : UIDesktopMenu
** FileName  : UIDesktopMenu.h
** Date      : lun. août 13 19:14:07 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIDESKTOPMENU_H
#define UIDESKTOPMENU_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UIDesktopMenu.h"
#include "pDesktopMenuEntry.h"

class Q_MONKEY_EXPORT UIDesktopMenu : public QDialog, public Ui::UIDesktopMenu, public QSingleton<UIDesktopMenu>
{
	Q_OBJECT
	friend class QSingleton<UIDesktopMenu>;

private:
	UIDesktopMenu( QWidget* = QApplication::activeWindow() );
	~UIDesktopMenu();

protected:
	pDesktopMenuEntry mDesktopEntries;

	void showEvent( QShowEvent* );

protected slots:
	void populateList();
	void on_leNameFilter_returnPressed();
	void on_leCategoriesFilters_returnPressed();
	void on_tbRight_clicked();
	void on_tbLeft_clicked();
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void accept();

};

#endif // UIDESKTOPMENU_H
