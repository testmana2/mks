#ifndef PTABBEDWORKSPACECORNER_H
#define PTABBEDWORKSPACECORNER_H

#include "MonkeyExport.h"

#include <QWidget>
#include <QBoxLayout>

class pTabbedWorkspace;
class QAction;

class Q_MONKEY_EXPORT pTabbedWorkspaceCorner : public QWidget
{
	Q_OBJECT

public:
	pTabbedWorkspaceCorner( pTabbedWorkspace*, const QSize& = QSize( 24, 24 ) );
	~pTabbedWorkspaceCorner();

	virtual bool eventFilter( QObject*, QEvent* );

	QBoxLayout::Direction direction() const;

public slots:
	void clearActions();
	void setButtonSize( const QSize& );
	void setDirection( QBoxLayout::Direction );
	void addAction( QAction* );
	void setActions( QList<QAction*> );

protected:
	pTabbedWorkspace* mWorkspace;
	QBoxLayout* mLayout;
	QSize mSize;
	QList<QAction*> mActions;

};

#endif // PTABBEDWORKSPACECORNER_H
