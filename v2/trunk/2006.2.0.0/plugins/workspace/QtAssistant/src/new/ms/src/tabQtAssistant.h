/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : vendredi 11 août 2006 - 17h34
 * AUTHOR       : Azevedo Filipe aka Nox PasNox ( pasnox at gmail dot com )
 * FILENAME     : tabQtAssistant.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef TABQTASSISTANT_H
#define TABQTASSISTANT_H
//
#include "abstractTab.h"
//
class QToolBar;
class QAction;
class QComboBox;
class QListWidget;
class QListWidgetItem;
class QTextBrowser;
class assistantSearch;
//
class tabQtAssistant : public abstractTab
{
	Q_OBJECT
	//
public:
	static tabQtAssistant* self( QWidget* = 0 );
	virtual QString fileFilter() const;
	virtual bool isCopyAvailable() const;
	virtual bool isSearchAvailable() const;
	QUrl homePage() const;
	//
protected:
	void showEvent( QShowEvent* );
	void closeEvent( QCloseEvent* );
	//
public slots:
	virtual void undo() {};
	virtual void redo() {};
	virtual void copy();
	virtual void cut() {};
	virtual void paste() {};
	virtual void search();
	virtual void goTo() {};
	virtual void applySettings() {};
	virtual void openUrl( const QUrl& );
	virtual void saveUrl() {};
	void on_aAdd_triggered();
	void on_aRemove_triggered();
	void on_lwBookmarks_itemDoubleClicked( QListWidgetItem* );
	void on_aHome_triggered();
	void on_cbZoom_currentIndexChanged( int );
	//
private:
	tabQtAssistant( QWidget* = 0 );
	//
	static QPointer<tabQtAssistant> mSelf;
	QUrl uHome;
	QToolBar* tbActions;
	QAction* aAdd;
	QAction* aRemove;
	QAction* aBack;
	QAction* aNext;
	QAction* aHome;
	QAction* aZoomIn;
	QAction* aZoomOut;
	QComboBox* cbZoom;
	QListWidget* lwBookmarks;
	QTextBrowser* tbSource;
	assistantSearch* mSearch;
	QAction* aSearch;
	QAction* aSearchPrevious;
	QAction* aSearchNext;
	bool mInit;
	//
};
//
#endif // TABQTASSISTANT_H
