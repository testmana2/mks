#include "UISettingsGNUDebugger.h"
//
#include <QCloseEvent>
//
QPointer<UISettingsGNUDebugger> UISettingsGNUDebugger::_self = 0L;
//
UISettingsGNUDebugger* UISettingsGNUDebugger::self( QWidget* parent )
{
	if ( !_self )
		_self = new UISettingsGNUDebugger( parent );
	return _self;
}
//
UISettingsGNUDebugger::UISettingsGNUDebugger( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
}
//
void UISettingsGNUDebugger::closeEvent( QCloseEvent* e )
{
	e->accept();
}
