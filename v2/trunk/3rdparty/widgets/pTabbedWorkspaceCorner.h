#ifndef PTABBEDWORKSPACECORNER_H
#define PTABBEDWORKSPACECORNER_H

#include "MonkeyExport.h"

#include <QWidget>
#include <QBoxLayout>
#include <QToolButton>

class QPaintEvent;
class pTabbedWorkspace;
class QAction;

class Q_MONKEY_EXPORT pTabbedWorkspaceCornerButton : public QToolButton
{
	Q_OBJECT

public:
	pTabbedWorkspaceCornerButton( QWidget* p ) : QToolButton( p ) {}

	void getStyleOption( QStyleOptionToolButton* s ) const { initStyleOption( s ); }

};

class Q_MONKEY_EXPORT pTabbedWorkspaceCorner : public QWidget
{
	Q_OBJECT

public:
	pTabbedWorkspaceCorner( pTabbedWorkspace* );
	~pTabbedWorkspaceCorner();

	virtual bool eventFilter( QObject*, QEvent* );

	QBoxLayout::Direction direction() const;

public slots:
	void clearActions();
	void setDirection( QBoxLayout::Direction );
	void addAction( QAction* );
	void setActions( QList<QAction*> );

protected:
	void drawButton( QToolButton*, QPaintEvent* );
	pTabbedWorkspace* mWorkspace;
	QBoxLayout* mLayout;
	QList<QAction*> mActions;

};

#endif // PTABBEDWORKSPACECORNER_H
