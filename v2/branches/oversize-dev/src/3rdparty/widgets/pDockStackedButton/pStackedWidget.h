#ifndef PSTACKEDWIDGET_H
#define PSTACKEDWIDGET_H
//
#include <QStackedWidget>
//
class pStackedWidget : public QStackedWidget
{
	Q_OBJECT
	//
public:
	pStackedWidget( QWidget* = 0 );
	//
protected:
	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );
	//
signals:
	void signalShowEvent( QShowEvent* );
	void signalHideEvent( QHideEvent* );
	//
};
//
#endif // PSTACKEDWIDGET_H
