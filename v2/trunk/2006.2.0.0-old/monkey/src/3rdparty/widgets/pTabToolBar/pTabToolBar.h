#ifndef PTABTOOLBAR_H
#define PTABTOOLBAR_H
//
#include <QToolBar>
#include <QHash>
//
#include "kmultitabbar.h"
#include "MonkeyExport.h"
//
class QDockWidget;
class QAction;
class pTabToolBarManager;
class KMultiTabBarButton;
class KMultiTabBarTab;
//
class Q_MONKEY_EXPORT pTabToolBar : public QToolBar
{
	Q_OBJECT
	//
public:
	pTabToolBar( pTabToolBarManager*, KMultiTabBar::KMultiTabBarMode );
	//
	virtual bool eventFilter( QObject*, QEvent* );
	//
	int appendButton( const QPixmap&, QMenu* = 0, const QString& = QString() );
	void removeButton( int );
	KMultiTabBarButton* button( int ) const;
	QList<KMultiTabBarButton*>* buttons();
	//
	int appendTab( QDockWidget*, const QPixmap&, const QString& = QString() );
	void removeTab( int );
	void setTab( int, bool );
	bool isTabRaised( int ) const;
	KMultiTabBarTab* tab( int ) const;
	QList<KMultiTabBarTab*>* tabs();
	//
	void removeTab( QDockWidget* );
	int tabIndexOf( QDockWidget* ) const;
	KMultiTabBarTab* tabOf( QDockWidget* ) const;
	QDockWidget* dockWidget( int ) const;
	//
	void setPosition( KMultiTabBar::KMultiTabBarPosition );
	KMultiTabBar::KMultiTabBarPosition position() const;
	//
	void setStyle( KMultiTabBar::KMultiTabBarStyle );
	KMultiTabBar::KMultiTabBarStyle tabStyle() const;
	//	
	void showActiveTabTexts( bool = true );
	//
	QAction* tabBarAction();
	void checkVisibility();
	//
private:
	KMultiTabBar* mMultiTabBar;
	QAction* mAction;
	pTabToolBarManager* mManager;
	QHash<int, QDockWidget*> mTabDocks;
	int mButtonId;
	int mTabId;
	//
private slots:
	void internal_orientationChanged( Qt::Orientation );
	void internal_tabClicked( int );
	void internal_dockChanged();
	void internal_childDestroyed( QObject* );
	//
signals:
	void buttonClicked( int );
	void tabClicked( int );
	void dockWidgetTabToolBarChanged( QDockWidget*, pTabToolBar* );
	//
};
//
#endif // PTABTOOLBAR_H
