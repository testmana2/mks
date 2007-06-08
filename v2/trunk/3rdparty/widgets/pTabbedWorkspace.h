#ifndef PTABBEDWORKSPACE_H
#define PTABBEDWORKSPACE_H

#include "MonkeyExport.h"

#include <QWidget>
#include <QTabBar>
#include <QList>

class QBoxLayout;
class QStackedLayout;
class QStackedWidget;
class QWorkspace;
class pTabbedWorkspaceCorner;

#define TABBED_DOCUMENT QWidget

class Q_MONKEY_EXPORT pTabbedWorkspace : public QWidget
{
	Q_OBJECT
	Q_ENUMS( TabMode DocumentMode )
	
public:
	enum TabMode { tmSDI = 0, tmMDI, tmTopLevel };
	enum DocumentMode { dmMaximized = 0, dmCascade, dmTile, dmIcons, dmMinimizeAll, dmRestoreAll };

	pTabbedWorkspace( QWidget* = 0, pTabbedWorkspace::TabMode = pTabbedWorkspace::tmMDI );
	~pTabbedWorkspace();
	
	virtual bool eventFilter( QObject*, QEvent* );

	QTabBar* tabBar() const;
	QTabBar::Shape tabShape() const;
	pTabbedWorkspace::TabMode tabMode() const;
	pTabbedWorkspace::DocumentMode documentMode() const;
	int currentIndex() const;
	TABBED_DOCUMENT* currentDocument() const;
	int indexOf( TABBED_DOCUMENT* ) const;
	TABBED_DOCUMENT* document( int ) const;
	int count() const;
	QList<TABBED_DOCUMENT*> documents() const;
	pTabbedWorkspaceCorner* cornerWidget( Qt::Corner = Qt::TopRightCorner ) const;
	int addTab( TABBED_DOCUMENT*, const QString& );
	int addTab( TABBED_DOCUMENT*, const QIcon&, const QString& );
	int insertTab( int, TABBED_DOCUMENT*, const QString& );
	int insertTab( int, TABBED_DOCUMENT*, const QIcon&, const QString& );

public slots:
	void setTabShape( QTabBar::Shape );
	void setTabMode( pTabbedWorkspace::TabMode );
	void setDocumentMode( pTabbedWorkspace::DocumentMode );
	void setCurrentIndex( int );
	void setCurrentDocument( TABBED_DOCUMENT* );
	void setCornerWidget( pTabbedWorkspaceCorner*, Qt::Corner = Qt::TopRightCorner );
	void removeTab( int );
	void removeTab( TABBED_DOCUMENT* );
	void closeCurrentTab();

protected:
	void updateCorners();
	void updateView();
	void addDocument( TABBED_DOCUMENT* d, int = -1 );

	pTabbedWorkspace::TabMode mTabMode;
	pTabbedWorkspace::DocumentMode mDocumentMode;
	// main layout
	QBoxLayout* mLayout;
	QList<TABBED_DOCUMENT*> mDocuments;
	// tab widget
	QTabBar* mTabBar;
	QBoxLayout* mTabLayout;
	// document widget
	QStackedLayout* mStackedLayout;
	QStackedWidget* mStackedWidget;
	QWorkspace* mWorkspaceWidget;

protected slots:
	void workspaceWidget_windowActivated( QWidget* );

signals:
	void tabInserted( int );
	void aboutToRemoveTab( int );
	void tabRemoved( int );
	void currentChanged( int );
	void tabShapeChanged( QTabBar::Shape );
	void tabModeChanged( pTabbedWorkspace::TabMode );
	void documentModeChanged( pTabbedWorkspace::DocumentMode );

};

#endif // PTABBEDWORKSPACE_H
