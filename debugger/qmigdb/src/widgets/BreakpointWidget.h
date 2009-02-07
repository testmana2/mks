#ifndef BREAKPOINTWIDGET_H
#define BREAKPOINTWIDGET_H

#include <QTreeWidget>

#include "QGdb-Driver.h"

class BreakpointWidget : public QTreeWidget
{
	Q_OBJECT
	
public:
	BreakpointWidget( QGdb::Driver* driver, QWidget* parent = 0 );
	virtual ~BreakpointWidget();

protected slots:
	void breakpointAdded( const QGdb::Breakpoint& breakpoint );
	void breakpointRemoved( const QGdb::Breakpoint& breakpoint );
	void breakpointUpdated( const QGdb::Breakpoint& breakpoint );
	void breakpointsCleared();

protected:
	void updateItem( QTreeWidgetItem* item, const QGdb::Breakpoint& breakpoint );
};

#endif // BREAKPOINTWIDGET_H
