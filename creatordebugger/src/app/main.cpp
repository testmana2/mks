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
#include "projectexplorer.h"
#include "coreplugin.h"

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	ExtensionSystem::PluginManager pluginManager;
	// for create instance
	QString error;
	
	Core::Internal::CorePlugin core;
#if 1
	ProjectExplorer::ProjectExplorerPlugin projectExplorer;
	Debugger::Internal::DebuggerPlugin debugger;
#endif	
	core.initialize (QStringList(), &error);
#if 1
	projectExplorer.initialize(QStringList(), &error);
	
	ExtensionSystem::IPlugin* iplugin = static_cast<ExtensionSystem::IPlugin*>(&debugger);
	iplugin->initialize (QStringList(), &error);	
#endif
	core.extensionsInitialized();
	
	return app.exec();
}
