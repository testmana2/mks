#include <QApplication>

#include <debuggermanager.h>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	
	Debugger::Internal::DebuggerManager manager;
	return app.exec();
}
