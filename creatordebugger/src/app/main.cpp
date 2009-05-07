#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QDebug>

#include <extensionsystem/pluginmanager.h>
#include <iplugin.h>

#include <debuggermanager.h>
#include <debuggerplugin.h>

#include "mainwindow.h"
#include "modemanager.h"

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
#if 0
	Debugger::Internal::DebuggerManager manager;
	QMainWindow* mw = manager.mainWindow();
	mw->statusBar()->addWidget (manager.statusLabel());
	
	mw->show();
#endif
	ExtensionSystem::PluginManager pluginManager;
	
	Core::Internal::MainWindow mw;
	
	Debugger::Internal::DebuggerPlugin plugin;
	QString error;
	ExtensionSystem::IPlugin* iplugin = static_cast<ExtensionSystem::IPlugin*>(&plugin);
	iplugin->initialize (QStringList(), &error);
	mw.updateContext();
	
	mw.setCentralWidget (mw.modeManager()->m_debugmode->widget());
	
	mw.show();
	return app.exec();
}
