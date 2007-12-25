#ifndef PDOCKWIDGET_H
#define PDOCKWIDGET_H

#include <QDockWidget>

class pDockWidget : public QDockWidget
{
	Q_OBJECT
	
public:
	pDockWidget( const QString&, QWidget* = 0, Qt::WindowFlags = 0 );
	pDockWidget( QWidget* = 0, Qt::WindowFlags = 0 );
	QSize sizeHint() const;

public slots:    void setVisible( bool );

protected:
	QSize contentsSize() const;
	QSize mSize;

};

#endif // PDOCKWIDGET_H
