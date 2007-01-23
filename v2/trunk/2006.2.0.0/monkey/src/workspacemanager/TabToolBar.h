#ifndef TABTOOLBAR_H
#define TABTOOLBAR_H
//
#include "pTabToolBarManager.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class Q_MONKEY_EXPORT TabToolBar : public pTabToolBarManager
{
	Q_OBJECT
	//
public:
	static TabToolBar* self( QMainWindow* = 0 );
	//
	QAction* newSeparator();
	//
private:
	TabToolBar( QMainWindow* );
	void initialize();
	//
	static QPointer<TabToolBar> mSelf;
	//
};
//
#endif // TABTOOLBAR_H
