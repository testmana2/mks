#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QDebug>

#include <debuggermanager.h>
#include <debuggerplugin.h>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	qDebug () << "create manager";
#if 0
	Debugger::Internal::DebuggerManager manager;
	QMainWindow* mw = manager.mainWindow();
	mw->statusBar()->addWidget (manager.statusLabel());
	
	mw->show();
#endif
	Debugger::Internal::DebuggerPlugin plugin;
	return app.exec();
}
