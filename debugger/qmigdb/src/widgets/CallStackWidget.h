#ifndef CALLSTACKWIDGET_H
#define CALLSTACKWIDGET_H

#include "QGdbDriver.h"

#include <QTreeWidget>

class CallStackWidget : public QTreeWidget
{
	Q_OBJECT
	
public:
	CallStackWidget( QGdbDriver* debugger, QWidget* parent = 0 );
	virtual ~CallStackWidget();
	
protected slots:
	void update( const QGdbDriver::CallStack& stack, int selectedLevel );
	void onItemActivated (QTreeWidgetItem*);
signals:
	void frameSelected (const QString& file, int line);
};

#endif // CALLSTACKWIDGET_H
