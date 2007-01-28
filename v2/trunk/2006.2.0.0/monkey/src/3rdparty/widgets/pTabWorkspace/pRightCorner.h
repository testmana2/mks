#ifndef PRIGHTCORNER_H
#define PRIGHTCORNER_H
//
#include <QToolButton>
#include <QPointer>
//
#include "pTabWorkspace.h"
#include "MonkeyExport.h"
//
class QAction;
class QActionGroup;
//
class Q_MONKEY_EXPORT pRightCorner : public QToolButton
{
	Q_OBJECT
	//
public:
	static pRightCorner* self( pTabWorkspace* = 0 );
	//
private:
	pRightCorner( pTabWorkspace* = 0 );
	static QPointer<pRightCorner> mSelf;
	QActionGroup* agView;
	QActionGroup* agWindows;
	QActionGroup* agShape;
	//
	QMenu* mLayouts, *mFiles;
	QActionGroup* agLayouts, *agFiles;
	//
private slots:
	void agView_triggered( QAction* );
	void agWindows_triggered( QAction* );
	void agShape_triggered( QAction* );
	void tabModeChanged( pTabWorkspace::TabMode );
	void tabShapeChanged( QTabBar::Shape );
	//
	void menu_aboutToShow();
	void agLayouts_triggered( QAction* );
	void agFiles_triggered( QAction* );
	//
signals:
	void requestChangeTabMode( pTabWorkspace::TabMode );
	void requestChangeWindowsMode( pTabWorkspace::WindowsMode );
	void requestChangeTabShape( QTabBar::Shape );
	//
};
//
#endif // PRIGHTCORNER_H
