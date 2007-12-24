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

#include <QWidget>
#include <QList>
#include <QDockWidget>
#include <QMenu>

#include "MonkeyExport.h"
#include "pTabBar.h"
#include "pFilesListWidget.h"

class QBoxLayout;
class QStackedLayout;
class QStackedWidget;
class QWorkspace;
class pAction;

class Q_MONKEY_EXPORT pTabbedWorkspace : public QWidget
{
	Q_OBJECT
	Q_ENUMS( TabMode DocumentMode )
	
public:
	enum DocumentMode { dmSDI = 0, dmMDI, dmTopLevel };
	//enum DocumentSize { dmMaximized = 0, dmCascade, dmTile, dmIcons, dmMinimizeAll, dmRestoreAll};

	pTabbedWorkspace( QMainWindow* , pTabbedWorkspace::DocumentMode = pTabbedWorkspace::dmSDI );
	~pTabbedWorkspace();
	
	virtual bool eventFilter( QObject*, QEvent* );

	//getters
	pTabBar* tabBar() const;
	pFilesListWidget* listWidget() const;
	
	pTabbedWorkspace::DocumentMode docMode() const;
	QTabBar::Shape tabShape () const;

	QWidgetList documents() const;	
	QWidget* document( int ) const;
	int count() const;
	
	int currentIndex() const;
	QWidget* currentDocument() const;
	int indexOf( QWidget* ) const;

	void setBackground( const QPixmap& );
	void setBackground( const QString& );
	
	int addDocument( QWidget*, const QString&,  const QIcon& = QIcon());
	int insertDocument( int, QWidget*, const QString&, const QIcon& = QIcon());
	
	//used for generating custom menu for file
	virtual void addFileActions (QMenu*) {};
	
public slots:
	void setDocMode( pTabbedWorkspace::DocumentMode );
	void setTabShape( QTabBar::Shape );

	void setCurrentIndex( int );
	void setCurrentDocument( QWidget* );

	virtual void closeDocument(QWidget* doc);
	virtual void closeDocument(int pos);
	virtual void closeAllDocuments ();
	virtual void closeCurrentDocument ();

	void activateNextDocument();
	void activatePreviousDocument();

    void setSDI ();
    void setMDI ();
    void setTopLevel ();
    void cascade ();
    void tile ();
    void minimize ();
    void restore ();

protected:

	// workspace properties
	pTabbedWorkspace::DocumentMode mDocMode;

	QList<QWidget*> mDocuments;
	int mCurrIndex;

	// main layout
	QBoxLayout* mLayout;
	// tab widget
	pTabBar* mTabBar;
	QBoxLayout* mTabLayout;
	//list widget
	pFilesListWidget* mFilesList;
	// document widget
	QWorkspace* mWorkspaceWidget;

signals:
	void documentInserted( int, QString, QIcon );
	void documentClosed( int );
	void currentChanged( int );
	void tabShapeChanged( QTabBar::Shape );
	void docModeChanged( pTabbedWorkspace::DocumentMode );
	void modifiedChanged (int, bool);
	
//	void aboutToCloseDocument (int, QCloseEvent*);
//	void aboutToCloseAll ();
    
};

#endif // PTABBEDWORKSPACE_H
