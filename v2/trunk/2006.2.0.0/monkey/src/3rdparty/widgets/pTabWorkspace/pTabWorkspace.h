#ifndef PTABWORKSPACE_H
#define PTABWORKSPACE_H
//
#include <QFrame>
#include <QPointer>
#include <QTabBar>
//
#include "MonkeyExport.h"
//
class QBoxLayout;
class QStackedWidget;
class QWorkspace;
//
class Q_MONKEY_EXPORT pTabBar : public QTabBar
{
	Q_OBJECT
protected:
	virtual void tabInserted( int i )
	{ QTabBar::tabInserted( i );
	  emit tabInserted( currentIndex(), i ); }
	virtual void tabRemoved( int i )
	{ QTabBar::tabRemoved( i );
	  emit tabRemoved( currentIndex(), i ); }
	//
signals:
	void tabInserted( int, int );
	void tabRemoved( int, int );
	//
};
//
class Q_MONKEY_EXPORT pTabWorkspace : public QFrame
{
	Q_OBJECT
	Q_ENUMS( TabMode )
	Q_ENUMS( WindowsMode )
	//
public:
	enum TabMode { tmUndefined = 0, tmSDI, tmMDI, tmTopLevel };
	enum WindowsMode { wmCascade = 0, wmTile, wmIcons, wmMinimizeAll, wmRestoreAll };
	pTabWorkspace( QWidget* = 0, pTabWorkspace::TabMode = pTabWorkspace::tmUndefined );
	~pTabWorkspace();
	//
	virtual bool eventFilter( QObject*, QEvent* );
	//
	int addTab( QWidget*, const QString& );
	int addTab( QWidget*, const QIcon&, const QString& );
	QWidget* cornerWidget( Qt::Corner = Qt::TopRightCorner ) const;
	int count() const;
	int currentIndex() const;
	QWidget* currentWidget() const;
	Qt::TextElideMode elideMode() const;
	QSize iconSize() const;
	int indexOf( QWidget* ) const;
	int insertTab( int, QWidget*, const QString& );
	int insertTab( int, QWidget*, const QIcon&, const QString& );
	bool isTabEnabled( int ) const;
	void removeTab( int );
	void setCornerWidget( QWidget*, Qt::Corner = Qt::TopRightCorner );
	void setElideMode( Qt::TextElideMode );
	void setIconSize( const QSize& );
	void setTabEnabled( int, bool );
	void setTabIcon( int, const QIcon& );
	void setTabText( int, const QString& );
	void setTabToolTip( int, const QString& );
	void setTabWhatsThis( int, const QString& );
	void setUsesScrollButtons( bool );
	QIcon tabIcon( int ) const;
	QTabBar::Shape tabShape() const;
	QString tabText( int ) const;
	QString tabToolTip( int ) const;
	QString tabWhatsThis ( int ) const;
	bool usesScrollButtons() const;
	QWidget* widget( int ) const;
	pTabWorkspace::TabMode tabMode() const;
	QTabBar* tabBar() const;
	//
	QList<QWidget*> documents() const { return mDocuments; }
	//
private:
	pTabWorkspace::TabMode mTabMode;
	QBoxLayout* mLayout1;
	QTabBar* mTabBar;
	QBoxLayout* mLayout2;
	QPointer<QStackedWidget> mStacked;
	QPointer<QWorkspace> mWorkspace;
	QList<QWidget*> mDocuments;
	//
protected:
	void appendWidget( QWidget* );
	bool updateView( pTabWorkspace::TabMode );
	void clearLayout();
	//
public slots:
	void setTabShape( QTabBar::Shape );
	void setCurrentIndex( int );
	void setCurrentWidget( QWidget* );
	void setTabMode( pTabWorkspace::TabMode );
	void setWindowsMode( pTabWorkspace::WindowsMode );
	void closeCurrentTab();
	//
private slots:
	void tabBar_currentChanged( int );
	void application_focusChanged( QWidget*, QWidget* );
	void childDestroyed( QObject* );
	//
signals:
	void tabInserted( int );
	void tabRemoved( int );
	void currentChanged( int );
	void tabModeChanged( pTabWorkspace::TabMode );
	void tabShapeChanged( QTabBar::Shape );
	//
};
//
#endif // PTABWORKSPACE_H
