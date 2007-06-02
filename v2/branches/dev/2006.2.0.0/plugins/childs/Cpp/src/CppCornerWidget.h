#ifndef CPPCORNERWIDGET_H
#define CPPCORNERWIDGET_H
//
#include <QWidget>
#include <QPointer>
//
class QAction;
class CppChild;
//
class CppCornerWidget : public QWidget
{
	Q_OBJECT
	//
public:
	static CppCornerWidget* self( QWidget* = 0 );
	//
private:
	CppCornerWidget( QWidget* = 0 );
	//
	static QPointer<CppCornerWidget> mSelf;
	QPointer<CppChild> mChild;
	QAction *aForm, *aHeader, *aSource;
	//
public slots:
	void setChild( CppChild* );
	//
private slots:
	void aForm_triggered();
	void ag_triggered( QAction* );
	//
};
//
#endif // CPPCORNERWIDGET_H
