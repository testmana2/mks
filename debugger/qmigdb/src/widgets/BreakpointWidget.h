#ifndef BREAKPOINTWIDGET_H
#define BREAKPOINTWIDGET_H

#include <QTreeWidget>

#include "QGdbDriver.h"

class BreakpointWidget : public QTreeWidget
{
	Q_OBJECT
	
public:
	BreakpointWidget( QGdbDriver* driver, QWidget* parent = 0 );
	virtual ~BreakpointWidget();

protected slots:
	void breakpointAdded( const QGdbDriver::Breakpoint& breakpoint );
	void breakpointRemoved( const QGdbDriver::Breakpoint& breakpoint );
	void breakpointUpdated( const QGdbDriver::Breakpoint& breakpoint );
	void breakpointsCleared();

protected:
	void updateItem( QTreeWidgetItem* item, const QGdbDriver::Breakpoint& breakpoint );
};

#endif // BREAKPOINTWIDGET_H
