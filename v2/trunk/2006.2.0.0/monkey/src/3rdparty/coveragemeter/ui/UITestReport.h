#ifndef UITESTREPORT_H
#define UITESTREPORT_H
//
#include "ui_UITestReport.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class Q_MONKEY_EXPORT UITestReport : public QDialog, public Ui::UITestReport
{
	Q_OBJECT
	//
public:
	static UITestReport* self( QWidget* = 0 );
	static void setTestTitle( const QString& );
	//
private slots:
	void okClicked( );
	void cancelClicked();
	//
private:
	UITestReport( QWidget* = 0 );
	static QPointer<UITestReport> mSelf;
	//
};
//
#endif // UITESTREPORT_H
