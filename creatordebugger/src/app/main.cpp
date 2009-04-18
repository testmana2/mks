#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QDebug>

#include <iplugin.h>
#include <debuggermanager.h>
#include <debuggerplugin.h>

#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
#if 0
	Debugger::Internal::DebuggerManager manager;
	QMainWindow* mw = manager.mainWindow();
	mw->statusBar()->addWidget (manager.statusLabel());
	
	mw->show();
#endif
	Core::Internal::MainWindow mw;
	
	Debugger::Internal::DebuggerPlugin plugin;
	QString error;
	ExtensionSystem::IPlugin* iplugin = static_cast<ExtensionSystem::IPlugin*>(&plugin);
	iplugin->initialize (QStringList(), &error);
	
	mw.show();
	return app.exec();
}
