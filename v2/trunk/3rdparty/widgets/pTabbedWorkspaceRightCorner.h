#ifndef PTABBEDWORKSPACERIGHTCORNER_H
#define PTABBEDWORKSPACERIGHTCORNER_H

#include "MonkeyExport.h"
#include "pTabbedWorkspaceCorner.h"
#include "QSingleton.h"
#include "pTabbedWorkspace.h"

#include <QTabBar>

class QAction;
class QActionGroup;

class Q_MONKEY_EXPORT pTabbedWorkspaceRightCorner : public pTabbedWorkspaceCorner, public QSingleton<pTabbedWorkspaceRightCorner>
{
	Q_OBJECT
	friend class QSingleton<pTabbedWorkspaceRightCorner>;

protected:
	pTabbedWorkspaceRightCorner( pTabbedWorkspace* );

	QActionGroup* agTabShape;
	QActionGroup* agTabMode;
	QActionGroup* agDocumentMode;

protected slots:
	void agTabShape_triggered( QAction* );
	void agTabMode_triggered( QAction* );
	void agDocumentMode_triggered( QAction* );

	void tabShapeChanged( QTabBar::Shape );
	void tabModeChanged( pTabbedWorkspace::TabMode );
	void documentModeChanged( pTabbedWorkspace::DocumentMode );

signals:
	void requestChangeTabMode( pTabbedWorkspace::TabMode );
	void requestChangeDocumentMode( pTabbedWorkspace::DocumentMode );
	void requestChangeTabShape( QTabBar::Shape );

};

#endif // PTABBEDWORKSPACERIGHTCORNER_H
