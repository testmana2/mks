#ifndef DEBUGDOCKWIDGET_H
#define DEBUGDOCKWIDGET_H

#include <pDockWidget.h>

class QTreeView;

class DebugDockWidget : public pDockWidget
{
	Q_OBJECT
	
public:
	DebugDockWidget( QWidget* parent = 0 );
	virtual ~DebugDockWidget();

protected:
	QTreeView* tvProjects;

protected slots:
	void currentProjectChanged();
	void showXml();
};

#endif // DEBUGDOCKWIDGET_H
