#include "CallStackWidget.h"

CallStackWidget::CallStackWidget( QWidget* parent )
	: QListWidget( parent )
{
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
