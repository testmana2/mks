#ifndef CALLSTACKWIDGET_H
#define CALLSTACKWIDGET_H

#include "QGdbDriver.h"

#include <QListWidget>

class CallStackWidget : public QListWidget
{
	Q_OBJECT
	
public:
	CallStackWidget( QGdbDriver* debugger, QWidget* parent = 0 );
	virtual ~CallStackWidget();
	
public slots:
	void update( const QGdbDriver::CallStack& stack );
};

#endif // CALLSTACKWIDGET_H
