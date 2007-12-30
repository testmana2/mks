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

#include <QDockWidget>
#include <QModelIndex>
#include <QSortFilterProxyModel>

class pTreeComboBox;
class QLineEdit;
class QListView;
class QDirModel;
class QTreeView;

class pDockFileBrowser : public QDockWidget, public QSingleton<pDockFileBrowser>
{
	Q_OBJECT
	friend class QSingleton<pDockFileBrowser>;
	friend class FileBrowserSettings;

	class FilteredModel: public QSortFilterProxyModel
	{
	protected:
		bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
	public:
		FilteredModel(QObject *parent = 0): QSortFilterProxyModel(parent) {};
		QStringList wildCards;
	};
	
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
	FilteredModel* mFilteredModel;

	QString mPath;

	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );
	
	//negative filter
	QStringList getFilterWildCards ();
	void setFilterWildCards (QStringList l);

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
