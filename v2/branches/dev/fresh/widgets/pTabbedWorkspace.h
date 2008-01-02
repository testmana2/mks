/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedWorkspace.h
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a workspace that can at run time be switched in SDI / MDI or Top Level
 ********************************************************************************************************/
#ifndef PTABBEDWORKSPACE_H
#define PTABBEDWORKSPACE_H

#include "MonkeyExport.h"
#include "pTabBar.h"

#include <QWidget>
#include <QList>
#include <QIcon>

class QBoxLayout;
class QStackedLayout;
class QStackedWidget;
class QMdiArea;
class QMdiSubWindow;
class pAction;
class pFilesListWidget;

class Q_MONKEY_EXPORT pTabbedWorkspace : public QWidget
{
	Q_OBJECT
	Q_ENUMS( TabMode DocumentMode )
	
public:
	enum DocumentMode { dmSDI = 0, dmMDI, dmTopLevel };

	pTabbedWorkspace( QWidget* , pTabbedWorkspace::DocumentMode = pTabbedWorkspace::dmSDI );
	~pTabbedWorkspace();
	
	virtual bool eventFilter( QObject*, QEvent* );

	//getters
	pTabBar* tabBar() const;
	pFilesListWidget* listWidget() const;
	
	pTabbedWorkspace::DocumentMode docMode() const;
	QTabBar::Shape tabShape() const;

	QWidgetList documents() const;	
	QWidget* document( int ) const;
	int count() const;
	
	int currentIndex() const;
	QWidget* currentDocument() const;
	int indexOf( QWidget* ) const;

	void setBackground( const QPixmap& );
	void setBackground( const QString& );
	
	int addDocument( QWidget*, const QString&,  const QIcon& = QIcon() );
	int insertDocument( int, QWidget*, const QString&, const QIcon& = QIcon() );
	
public slots:
	void setDocMode( pTabbedWorkspace::DocumentMode );
	void setTabShape( QTabBar::Shape );

	void setCurrentIndex( int );
	void setCurrentDocument( QWidget* );

	/*
	Do not make this functions virtual!!
	closeAllDocuments must not call functions of child classes
	*/
	void closeDocument( QWidget* document );
	void closeDocument( int index );
	void closeAllDocuments();
	void closeCurrentDocument();

	void activateNextDocument();
	void activatePreviousDocument();

    void setSDI();
    void setMDI();
    void setTopLevel();
    void cascade();
    void tile();
    void minimize();
    void restore();

protected:
	// workspace properties
	pTabbedWorkspace::DocumentMode mDocMode;

	QList<QWidget*> mDocuments;

	// main layout
	QBoxLayout* mLayout;
	// tab widget
	pTabBar* mTabBar;
	QBoxLayout* mTabLayout;
	//list widget
	pFilesListWidget* mFilesList;
	// document widget
	QStackedLayout* mStackedLayout;
	/* Stacked widget used for SDI mode, because we can't use maximized windows on QMdiArea on Mac*/
	QStackedWidget* mStackedWidget; 
	QMdiArea* mMdiAreaWidget;

protected slots:
	void setCurrentDocument( QMdiSubWindow* );

signals:
	void documentInserted( int, const QString&, const QIcon& );
	void documentClosed( int );
	// -1 if last file was closed
	void currentChanged( int );
	void tabShapeChanged( QTabBar::Shape );
	void docModeChanged( pTabbedWorkspace::DocumentMode );
	void modifiedChanged( int, bool );
	void docTitleChanged( int, const QString& );
	
//	void aboutToCloseDocument (int, QCloseEvent*);
//	void aboutToCloseAll ();
    
};

#endif // PTABBEDWORKSPACE_H
