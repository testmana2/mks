/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 15h09
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pDockStackedButton.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PDOCKSTACKEDBUTTON_H
#define PDOCKSTACKEDBUTTON_H
//
#include <QDockWidget>
#include <QPixmap>
#include <QButtonGroup>
//
class pStackedWidget;
class QToolButton;
class QBoxLayout;
//
class pDockStackedButton : public QDockWidget
{
	Q_OBJECT
	//
public:
	pDockStackedButton( QWidget* = 0, Qt::Orientation = Qt::Horizontal );
	~pDockStackedButton();
	//
	void addWidget( QWidget*, const QString&, const QPixmap& = QPixmap() );
	void removeWidget( QWidget* );
	QWidget* currentWidget() const;
	QWidget* widget( int ) const;
	int currentIndex() const;
	int indexOf( QWidget* ) const;
	int count() const;
	Qt::Orientation orientation() const;
	QString makeTitle( const QString& );
	//
private:
	QBoxLayout* layout1;
	QBoxLayout* layout2;
	pStackedWidget* mStacked;
	QButtonGroup mGroup;
	Qt::Orientation mOrientation;
	QSizePolicy mPolicy;
	QSize mSize;
	//
public slots:
	void stackedShowEvent( QShowEvent* );
	void stackedHideEvent( QHideEvent* );
	void setCurrentWidget( QWidget* );
	void setCurrentIndex( int );
	void setOrientation( Qt::Orientation );
	//
private slots:
	void buttonClicked( int );
	//
signals:
	void currentChanged( int );
	void widgetRemoved( int );
	//
};
//
#endif // PDOCKSTACKEDBUTTON_H
