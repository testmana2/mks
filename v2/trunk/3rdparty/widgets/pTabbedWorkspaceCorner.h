#ifndef PTABBEDWORKSPACECORNER_H
#define PTABBEDWORKSPACECORNER_H

#include "MonkeyExport.h"

#include <QWidget>
#include <QBoxLayout>

class QPaintEvent;
class pTabbedWorkspace;
class QAction;

class Q_MONKEY_EXPORT pTabbedWorkspaceCorner : public QWidget
{
	Q_OBJECT

public:
	pTabbedWorkspaceCorner( pTabbedWorkspace* );
	~pTabbedWorkspaceCorner();

	QBoxLayout::Direction direction() const;

public slots:
	void clearActions();
	void setDirection( QBoxLayout::Direction );
	void addAction( QAction* );
	void setActions( QList<QAction*> );

protected:
	pTabbedWorkspace* mWorkspace;
	QBoxLayout* mLayout;
	QList<QAction*> mActions;

};

#endif // PTABBEDWORKSPACECORNER_H
