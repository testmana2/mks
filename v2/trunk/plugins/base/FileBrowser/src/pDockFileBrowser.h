/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Kopats Andei aka hlamer <hlamer@tut.by>
** Project   : FileBrowser Plugin
** FileName  : pDockFileBrowser.h
** Date      : 2007-08-27T23:00:25
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PDOCKFILEBROWSER_H
#define PDOCKFILEBROWSER_H

#include "QSingleton.h"
#include "pDockWidget.h"

#include <QModelIndex>

class pTreeComboBox;
class QLineEdit;
class QListView;
class QDirModel;
class QTreeView;

class pDockFileBrowser : public pDockWidget, public QSingleton<pDockFileBrowser>
{
	Q_OBJECT
	friend class QSingleton<pDockFileBrowser>;

public:
	QString currentPath() const;
	void setCurrentPath( const QString& );

protected:
	bool mShown;
	pTreeComboBox* mCombo;
	QLineEdit* mLineEdit;
	QListView* mList;
	QTreeView* mTree;
	QDirModel* mDirsModel;
	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );

private:
	pDockFileBrowser( QWidget* = 0 );

private slots:
	void tbUp_clicked();
	void tbRefresh_clicked();
	void cb_currentChanged( const QModelIndex& );
	void lv_doubleClicked( const QModelIndex& );
	void tv_doubleClicked( const QModelIndex& );

signals:
	void saveSettings();
	void restoreSettings();

};

#endif // PDOCKFILEBROWSER_H
