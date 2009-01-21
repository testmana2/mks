#include "CallStackWidget.h"

CallStackWidget::CallStackWidget( QGdbDriver* debugger, QWidget* parent )
	: QListWidget( parent )
{
	connect (debugger, SIGNAL (callStackUpdate (const QGdbDriver::CallStack&)),
			 this, SLOT (update (const QGdbDriver::CallStack&)));

}

CallStackWidget::~CallStackWidget()
{
}

void CallStackWidget::update (const QGdbDriver::CallStack& stack)
{
	clear();
	foreach (QGdbDriver::Frame frame, stack)
	{
		QString item = "#" + QString::number (frame.level);
		item += " ";
		item += frame.function;
		item += " (";
		bool first = true;
		foreach (QGdbDriver::FunctionArgument argument, frame.arguments)
		{
			if (!first)
				item += ", ";
			item += argument.name;
			item += " = ";
			item += argument.value;
			first = false;
		}
		item += ") ";
		item += "at ";
		item += frame.file;
		item += ":";
		item += QString::number(frame.line);
		addItem (item);
	}
}
