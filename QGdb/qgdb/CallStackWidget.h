#ifndef CALLSTACKWIDGET_H
#define CALLSTACKWIDGET_H

#include "GnuDebugger.h"

#include <QListWidget>

class CallStackWidget : public QListWidget
{
	Q_OBJECT
	
public:
	CallStackWidget( QWidget* parent = 0 );
	virtual ~CallStackWidget();
public slots:
	void update (const GnuDebugger::CallStack& stack);
};

#endif // CALLSTACKWIDGET_H
