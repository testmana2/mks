#ifndef UISETTINGSGNUDEBUGGER_H
#define UISETTINGSGNUDEBUGGER_H
//
#include "ui_UISettingsGNUDebugger.h"
//
#include <QPointer>
//
class UISettingsGNUDebugger : public QWidget, public Ui::UISettingsGNUDebugger
{
	Q_OBJECT
	//
public:
	static UISettingsGNUDebugger* self( QWidget* = 0 );
	UISettingsGNUDebugger( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	static QPointer<UISettingsGNUDebugger> _self;
	//
};
//
#endif // UISETTINGSGNUDEBUGGER_H
