#ifndef CALLSTACKWIDGET_H
#define CALLSTACKWIDGET_H

#include "QGdbDriver.h"

#include <QTreeWidget>

class CallStackWidget : public QTreeWidget
{
	Q_OBJECT

public:
	CallStackWidget( QGdb::Driver* debugger, QWidget* parent = 0 );
	virtual ~CallStackWidget();
	
protected slots:
	void update( const QGdb::CallStackFrameList& stack, int selectedLevel );
	void onItemActivated( QTreeWidgetItem* item );

signals:
	void frameSelected( int frame );
};

#endif // CALLSTACKWIDGET_H
