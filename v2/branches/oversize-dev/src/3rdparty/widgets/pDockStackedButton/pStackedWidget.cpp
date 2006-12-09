#include "pStackedWidget.h"
//
pStackedWidget::pStackedWidget( QWidget* p )
	: QStackedWidget( p )
{}
//
void pStackedWidget::showEvent( QShowEvent* e )
{
	emit signalShowEvent( e );
}
//
void pStackedWidget::hideEvent( QHideEvent* e )
{
	emit signalHideEvent( e );
}
