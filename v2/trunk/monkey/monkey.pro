# Monkey Studio 2 project file - 2005 - 2010

# include config file
include( ../config.pri )

TEMPLATE    = app
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets printsupport
TARGET  = $$PACKAGE_TARGET
VERSION = $$QMAKE_TARGET_VERSION
DEFINES *= MONKEY_CORE_BUILD
DESTDIR = $$PACKAGE_DESTDIR

# monkey sources paths // added at first to avoid conflict with possible same name files in subprojects (like main.h from ctags)
MONKEY_SOURCES_PATHS    = $$getFolders( ./src, resources )
INCLUDEPATH *= src
#INCLUDEPATH    *= $${MONKEY_SOURCES_PATHS}
DEPENDPATH  *= $${MONKEY_SOURCES_PATHS}

LIBS    *= -L$${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()
mac:*-g++*:LIBS *= -Wl,-all_load # import all symbols as the not used ones too
else:*-g++*:LIBS    *= -Wl,--whole-archive # import all symbols as the not used ones too
mac:*-g++*:LIBS *= -dynamic
else:unix:*-g++*:LIBS   *= -rdynamic

# include qscintilla framework
include( ../qscintilla/qscintilla.pri )

# include fresh framework
include( ../fresh/fresh.pri )

# include qCtagsSense framework
include( ../qCtagsSense/qCtagsSense.pri )

win32-*g++*:LIBS    *= -Wl,--out-implib,$${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()/$${TARGET}.lib
win32-msvc*:LIBS    *= /IMPLIB:$${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()/$${TARGET}.lib -lshell32

mac:*-g++*:LIBS *= -Wl,-noall_load # stop importing all symbols
else:*-g++*:LIBS    *= -Wl,--no-whole-archive # stop importing all symbols

include( autoGenerateFile.pri )
autoGenerateFile( "main.h.in", "src/main.h" )

mac:ICON    = src/resources/icons/application/monkey2.icns
win32:RC_FILE   *= monkey.rc

RESOURCES   *= src/resources/resources.qrc

FORMS   *= src/maininterface/ui/UIAbout.ui \
    src/maininterface/ui/UISettings.ui \
    src/abbreviationsmanager/ui/UIAddAbbreviation.ui \
    src/templatesmanager/ui/UITemplatesWizard.ui \
    src/pluginsmanager/ui/UIPluginsSettings.ui \
    src/pluginsmanager/ui/UIPluginsSettingsElement.ui \
    src/pluginsmanager/ui/UIPluginsSettingsAbout.ui \
    src/xupmanager/gui/UIXUPFindFiles.ui \
    src/xupmanager/gui/XUPProjectManager.ui \
    src/pluginsmanager/ui/UICLIToolSettings.ui \
    src/workspace/pOpenedFileExplorer.ui \
    src/xupmanager/gui/CommandsEditor.ui \
    src/xupmanager/gui/VariablesEditor.ui \
    src/xupmanager/gui/UIXUPEditor.ui \
    src/xupmanager/gui/FilesEditor.ui \
    src/xupmanager/gui/MainEditor.ui \
    src/consolemanager/CommandEditor.ui

HEADERS *= src/maininterface/ui/UIAbout.h \
    src/maininterface/ui/UISettings.h \
    src/recentsmanager/pRecentsManager.h \
    src/workspace/pAbstractChild.h \
    src/qscintillamanager/pEditor.h \
    src/qscintillamanager/qSciShortcutsManager.h \
    src/workspace/pChild.h \
    src/workspace/UISaveFiles.h \
    src/workspace/pFileManager.h \
    src/workspace/pWorkspace.h \
    src/maininterface/UIMain.h \
    src/abbreviationsmanager/pAbbreviationsManager.h \
    src/abbreviationsmanager/ui/UIAddAbbreviation.h \
    src/variablesmanager/VariablesManager.h \
    src/templatesmanager/pTemplatesManager.h \
    src/templatesmanager/ui/UITemplatesWizard.h \
    src/pMonkeyStudio.h \
    src/consolemanager/pConsoleManager.h \
    src/consolemanager/AbstractCommandParser.h \
    src/consolemanager/CommandParser.h \
    src/consolemanager/pCommand.h \
    src/pluginsmanager/BasePlugin.h \
    src/pluginsmanager/XUPPlugin.h \
    src/pluginsmanager/ChildPlugin.h \
    src/pluginsmanager/PluginsManager.h \
    src/pluginsmanager/ui/UIPluginsSettings.h \
    src/settingsmanager/Settings.h \
    src/coremanager/MonkeyCore.h \
    src/statusbar/StatusBar.h \
    src/pluginsmanager/ui/UIPluginsSettingsElement.h \
    src/pluginsmanager/ui/UIPluginsSettingsAbout.h \
    src/xupmanager/core/XUPFilteredProjectModel.h \
    src/xupmanager/core/XUPItem.h \
    src/xupmanager/core/XUPProjectItem.h \
    src/xupmanager/core/ProjectTypesIndex.h \
    src/xupmanager/core/XUPProjectModel.h \
    src/xupmanager/gui/UIXUPFindFiles.h \
    src/xupmanager/gui/XUPProjectManager.h \
    src/xupmanager/core/XUPProjectModelProxy.h \
    src/shared/MkSFileDialog.h \
    src/pluginsmanager/ui/UICLIToolSettings.h \
    src/commandlinemanager/CommandLineManager.h \
    src/shellmanager/MkSShellConsole.h \
    src/shellmanager/MkSShellInterpreter.h \
    src/pluginsmanager/PluginsMenu.h \
    src/workspace/pOpenedFileExplorer.h \
    src/workspace/pOpenedFileModel.h \
    src/consolemanager/EnvironmentVariablesManager.h \
    src/consolemanager/pConsoleManagerStep.h \
    src/xupmanager/core/XUPDynamicFolderItem.h \
    src/xupmanager/gui/XUPOpenedProjectsModel.h \
    src/xupmanager/gui/CommandsEditor.h \
    src/xupmanager/gui/VariablesEditor.h \
    src/xupmanager/gui/UIXUPEditor.h \
    src/xupmanager/core/XUPProjectItemHelper.h \
    src/xupmanager/core/XUPItemVariableEditorModel.h \
    src/xupmanager/gui/FilesEditor.h \
    src/xupmanager/gui/MainEditor.h \
    src/xupmanager/gui/XUPPageEditor.h \
    src/xupmanager/gui/CommandsEditorModel.h \
    src/xupmanager/core/XUPProjectItemCache.h \
    src/shared/FileSystemModel.h \
    src/consolemanager/CommandEditor.h \
    src/workspace/ExMdiArea.h

SOURCES *= src/maininterface/ui/UIAbout.cpp \
    src/maininterface/ui/UISettings.cpp \
    src/recentsmanager/pRecentsManager.cpp \
    src/qscintillamanager/pEditor.cpp \
    src/qscintillamanager/qSciShortcutsManager.cpp \
    src/workspace/pChild.cpp \
    src/workspace/UISaveFiles.cpp \
    src/workspace/pFileManager.cpp \
    src/workspace/pWorkspace.cpp \
    src/maininterface/UIMain.cpp \
    src/abbreviationsmanager/pAbbreviationsManager.cpp \
    src/abbreviationsmanager/ui/UIAddAbbreviation.cpp \
    src/variablesmanager/VariablesManager.cpp \
    src/templatesmanager/pTemplatesManager.cpp \
    src/templatesmanager/ui/UITemplatesWizard.cpp \
    src/pMonkeyStudio.cpp \
    src/consolemanager/pConsoleManager.cpp \
    src/consolemanager/CommandParser.cpp \
    src/pluginsmanager/PluginsManager.cpp \
    src/pluginsmanager/BasePlugin.cpp \
    src/pluginsmanager/ui/UIPluginsSettings.cpp \
    src/main.cpp \
    src/settingsmanager/Settings.cpp \
    src/coremanager/MonkeyCore.cpp \
    src/statusbar/StatusBar.cpp \
    src/pluginsmanager/ui/UIPluginsSettingsElement.cpp \
    src/pluginsmanager/ui/UIPluginsSettingsAbout.cpp \
    src/xupmanager/core/XUPFilteredProjectModel.cpp \
    src/xupmanager/core/XUPItem.cpp \
    src/xupmanager/core/XUPProjectItem.cpp \
    src/xupmanager/core/ProjectTypesIndex.cpp \
    src/xupmanager/core/XUPProjectModel.cpp \
    src/xupmanager/gui/UIXUPFindFiles.cpp \
    src/xupmanager/gui/XUPProjectManager.cpp \
    src/xupmanager/core/XUPProjectModelProxy.cpp \
    src/shared/MkSFileDialog.cpp \
    src/pluginsmanager/ui/UICLIToolSettings.cpp \
    src/commandlinemanager/CommandLineManager.cpp \
    src/shellmanager/MkSShellInterpreter.cpp \
    src/shellmanager/MkSShellConsole.cpp \
    src/pluginsmanager/PluginsMenu.cpp \
    src/workspace/pOpenedFileExplorer.cpp \
    src/workspace/pOpenedFileModel.cpp \
    src/pluginsmanager/CLIToolPlugin.cpp \
    src/consolemanager/EnvironmentVariablesManager.cpp \
    src/consolemanager/pConsoleManagerStep.cpp \
    src/xupmanager/core/XUPDynamicFolderItem.cpp \
    src/xupmanager/gui/XUPOpenedProjectsModel.cpp \
    src/xupmanager/gui/CommandsEditor.cpp \
    src/xupmanager/gui/VariablesEditor.cpp \
    src/xupmanager/gui/UIXUPEditor.cpp \
    src/xupmanager/core/XUPProjectItemHelper.cpp \
    src/xupmanager/core/XUPItemVariableEditorModel.cpp \
    src/xupmanager/gui/FilesEditor.cpp \
    src/xupmanager/gui/MainEditor.cpp \
    src/xupmanager/gui/XUPPageEditor.cpp \
    src/xupmanager/gui/CommandsEditorModel.cpp \
    src/xupmanager/core/XUPProjectItemCache.cpp \
    src/shared/FileSystemModel.cpp \
    src/consolemanager/CommandEditor.cpp \
    src/workspace/ExMdiArea.cpp

TRANSLATIONS    *= ../datas/translations/monkeystudio_fr.ts \
    ../datas/translations/monkeystudio_be.ts \
    ../datas/translations/monkeystudio_ru.ts \
    ../datas/translations/monkeystudio_it.ts \
    ../datas/translations/monkeystudio_ar.ts \
    ../datas/translations/monkeystudio_es.ts \
    ../datas/translations/monkeystudio_ca.ts
