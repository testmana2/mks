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
	void breakpointEdited( const QGdb::Breakpoint& before, const QGdb::Breakpoint& after );
	void breakpointsCleared();
	
	void self_itemDoubleClicked( QTreeWidgetItem* item, int column );

protected:
	void updateItem( QTreeWidgetItem* item, const QGdb::Breakpoint& breakpoint );

signals:
	void break_breakpointEdited( const QGdb::Breakpoint& before, const QGdb::Breakpoint& after );
};

#endif // BREAKPOINTWIDGET_H
