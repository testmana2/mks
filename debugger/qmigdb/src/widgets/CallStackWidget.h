#ifndef CALLSTACKWIDGET_H
#define CALLSTACKWIDGET_H

#include "QGdbDriver.h"

#include <QTreeWidget>

class CallStackWidget : public QTreeWidget
{
		Q_OBJECT

public:
	CallStackWidget( QGdb::Driver* driver, QWidget* parent = 0 );
	virtual ~CallStackWidget();
	
protected slots:
	void update( const QGdb::CallStackFrameList& stack, int selectedLevel );
	void onItemActivated( QTreeWidgetItem* item );

signals:
	void frameActivated( const QGdb::CallStackFrame& frame );
};

#endif // CALLSTACKWIDGET_H
