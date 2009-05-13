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
#include "projectexplorer/projectexplorer.h"
#include "coreplugin/coreplugin.h"
#include "find/findplugin.h"
#include "quickopen/quickopenplugin.h"
#include "texteditor/texteditorplugin.h"
#include "cppeditor/cppplugin.h"

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	ExtensionSystem::PluginManager pluginManager;
	// for create instance
	QString error;
	
	Core::Internal::CorePlugin *core = new Core::Internal::CorePlugin();
	ProjectExplorer::ProjectExplorerPlugin *projectExplorer = new ProjectExplorer::ProjectExplorerPlugin();
	Debugger::Internal::DebuggerPlugin *debugger = new Debugger::Internal::DebuggerPlugin();
	Find::Internal::FindPlugin *find = new Find::Internal::FindPlugin();
	//QuickOpen::Internal::QuickOpenPlugin quickOpen = new QuickOpen::Internal::QuickOpenPlugin();
	TextEditor::Internal::TextEditorPlugin *textEditor = new TextEditor::Internal::TextEditorPlugin();
	CppEditor::Internal::CppPlugin *cpp = new CppEditor::Internal::CppPlugin();
	
	core->initialize (QStringList(), &error);
	projectExplorer->initialize(QStringList(), &error);
	ExtensionSystem::IPlugin* i_debugger_plugin = static_cast<ExtensionSystem::IPlugin*>(debugger);
	i_debugger_plugin->initialize (QStringList(), &error);	
	find->initialize (QStringList(), &error);
	//quickOpen.initialize (QStringList(), &error);
	textEditor->initialize (QStringList(), &error);
	cpp->initialize (QStringList(), &error);
	
	core->extensionsInitialized();
	
	int res = app.exec();
	
	delete cpp;
	delete textEditor;
	//delete quickOpen;
	delete find;
	i_debugger_plugin->shutdown();
	delete debugger;
	delete projectExplorer;
	delete core;
	
	return res;
}
