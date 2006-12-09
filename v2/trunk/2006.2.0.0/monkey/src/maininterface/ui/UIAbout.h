#ifndef UIABOUT_H
#define UIABOUT_H
//
#include "ui_UIAbout.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class Q_MONKEY_EXPORT UIAbout : public QDialog, public Ui::UIAbout
{
	Q_OBJECT
	//
public:
	static UIAbout* self( QWidget* = 0 );
	//
public slots:
	void anchorClicked( const QUrl& );
	//
private:
	UIAbout( QWidget* = 0 );
	static QPointer<UIAbout> mSelf;
	//
};
//
#endif // UIABOUT_H
