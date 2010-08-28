#ifndef DEBUGDOCKWIDGET_H
#define DEBUGDOCKWIDGET_H

#include "ui_DebugDockWidget.h"

class DebugDockWidget : public pDockWidget, public Ui::DebugDockWidget
{
	Q_OBJECT
	
public:
	DebugDockWidget( QWidget* parent = 0 );
	virtual ~DebugDockWidget();

public slots:
	void qtMessageHandler( QtMsgType type, const char* msg );

protected slots:
	void currentProjectChanged();
	void showXml();
};

#endif // DEBUGDOCKWIDGET_H
