#include <QApplication>
#include <QDebug>

#include <debuggermanager.h>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	qDebug () << "create manager";
	Debugger::Internal::DebuggerManager manager;
	return app.exec();
}
